﻿#include "TinyIP.h"
#include "Arp.h"

#include "Kernel\Task.h"

#include "Tcp.h"
#include "Udp.h"

#define NET_DEBUG DEBUG

/******************************** TinyIP ********************************/
TinyIP::TinyIP() : _Arr(0) { Init(); }

TinyIP::TinyIP(ITransport* port) : _Arr(0)
{
	Init();
	Init(port);
}

TinyIP::~TinyIP()
{
	delete _port;
    _port = nullptr;

	delete Arp;
	Arp = nullptr;
}

void TinyIP::Init()
{
	_port = nullptr;
	_StartTime = 0;

	// 以太网缓冲区先初始化为0，然后再调整大小
	_Arr.SetLength(1500);

	Mask = 0x00FFFFFF;
	Gateway = DNSServer = IP = 0;

	Arp = nullptr;
}

void TinyIP::Init(ITransport* port)
{
	_port = port;
	_StartTime = Sys.Ms();

	InitConfig();
	LoadConfig();
}

// 循环调度的任务
uint TinyIP::Fetch(Stream& ms)
{
	// 获取缓冲区的包
	Buffer bs(ms.Current(), ms.Remain());
	int len = _port->Read(bs);
	// 如果缓冲器里面没有数据则转入下一次循环
	if(len < (int)sizeof(ETH_HEADER)) return 0;

	// 位置指针后移
	//ms.Seek(-len);

	// 获取第一个结构体，不要移动指针
	auto eth	= ms.Retrieve<ETH_HEADER>(false);
	UInt64 v = eth->DestMac.Value();
	// 广播地址有效，直接返回
	if(!v || v == 0xFFFFFFFFFFFFFFFFull) return len;

	// 只处理发给本机MAC的数据包。此时进行目标Mac地址过滤，可能是广播包
	MacAddress dest = v;
	if(dest != Mac && !dest.IsBroadcast()) return 0;

	return len;
}

void TinyIP::Process(Stream& ms)
{
	auto eth	= ms.Retrieve<ETH_HEADER>();
	if(!eth) return;

	// 只处理发给本机MAC的数据包。此时不能进行目标Mac地址过滤，因为可能是广播包
	MacAddress mac = eth->SrcMac.Value();
#if NET_DEBUG >= 3
	mac.Show();
	debug_printf("=>");
	MacAddress(eth->DestMac.Value()).Show();
	if(eth->Type == ETH_ARP)
		debug_printf(" Type=ARP\r\n");
	else if(eth->Type == ETH_IP)
		debug_printf(" Type=IP\r\n");
	else
		debug_printf(" Type=0x%04X\r\n", eth->Type);
#endif

	// 处理ARP
	if(eth->Type == ETH_ARP)
	{
		IP_HEADER header;
		if(Arp && Arp->Enable) Arp->Process(header, ms);

		return;
	}

#if NET_DEBUG
	if(eth->Type != ETH_IP)
	{
		debug_printf("Unkown EthernetType 0x%02X From ", eth->Type);
		mac.Show();
		debug_printf("\r\n");
	}
#endif

	auto ip	= ms.Retrieve<IP_HEADER>();
	if(!ip) return;

	// 记录远程信息
	IPAddress remote = ip->SrcIP;
	IPAddress local = ip->DestIP;

#if NET_DEBUG >= 2
	remote.Show();
	debug_printf("=>");
	local.Show();

	cstring name = "Unkown";
	switch(ip->Protocol)
	{
		case IP_ICMP: { name = "ICMP"; break; }
		case IP_IGMP: { name = "IGMP"; break; }
		case IP_TCP: { name = "TCP"; break; }
		case IP_UDP: { name = "UDP"; break; }
	}
	debug_printf(" Type=%s\r\n", name);
#endif

	// 是否发给本机。
	if(local != IP && !IsBroadcast(local)) return;

	// 移交给ARP处理，为了让它更新ARP表
	if(Arp)
	{
		ArpSocket* arp = (ArpSocket*)Arp;
		arp->Add(remote, mac);
	}

	FixPayloadLength(*ip, ms);

	// 各处理器有可能改变数据流游标，这里备份一下
	uint p = ms.Position();
	// 考虑到可能有通用端口处理器，也可能有专用端口处理器（一般在后面），这里偷懒使用倒序处理
	for(int i=Sockets.Count()-1; i>=0; i--)
	{
		auto socket = (TinySocket*)Sockets[i];
		if(socket && socket->Enable)
		{
			// 必须类型匹配
			if(socket->Type == ip->Protocol)
			{
				// 如果处理成功，则中断遍历
				if(socket->Process(*ip, ms)) return;
				ms.SetPosition(p);
			}
		}
	}

#if NET_DEBUG
	/*debug_printf("IP Unkown Protocol=%d ", ip->Protocol);
	ShowIP(ip->SrcIP);
	debug_printf(" => ");
	ShowIP(ip->DestIP);
	debug_printf("\r\n");*/
#endif
}

// 修正IP包负载数据的长度。物理层送来的长度可能有误，一般超长
void TinyIP::FixPayloadLength(IP_HEADER& ip, Stream& ms)
{
	// 前面的len不准确，必须以这个为准
	uint size = _REV16(ip.TotalLength) - (ip.Length << 2);
	ms.Length = ms.Position() + size;
	//len = size;
	//buf += (ip->Length << 2);
	ms.Seek((ip.Length << 2) - sizeof(IP_HEADER));
}

// 任务函数
void TinyIP::Work(void* param)
{
	TinyIP* tip = (TinyIP*)param;
	if(tip)
	{
		// 注意，此时指针位于0，而内容长度为缓冲区长度
		Stream ms(tip->_Arr);
		uint len = tip->Fetch(ms);
		if(len)
		{
			tip->Process(ms);
		}
	}
}

bool TinyIP::OnOpen()
{
	debug_printf("\r\nTinyIP::Open...\r\n");

	if(!_port->Open())
	{
		debug_printf("TinyIP::Open Failed!\r\n");
		return false;
	}

	// 必须有Arp，否则无法响应别人的IP询问
	if(!Arp) Arp = new ArpSocket(this);
	Arp->Enable = true;

	ShowConfig();

	// 添加到系统任务，马上开始，尽可能多被调度
	//debug_printf("TinyIP::以太网轮询 ");
    uint tid = Sys.AddTask(Work, this, 0, 1000, "以太网");
	auto task = Task::Get(tid);
	task->MaxDeepth = 2;	// 以太网允许重入，因为有时候在接收里面等待下一次接收

#if NET_DEBUG
	int us = (int)(Sys.Ms() - _StartTime);
	debug_printf("TinyIP Ready! Cost:%dms\r\n\r\n", us / 1000);
#endif

	return true;
}

void TinyIP::OnClose()
{
	delete Arp;

	_port->Close();
}

bool TinyIP::Config()
{
	//ShowConfig();
	return true;
}

bool TinyIP::SendEthernet(ETH_TYPE type, const MacAddress& remote, const byte* buf, uint len)
{
	auto eth	= (ETH_HEADER*)(buf - sizeof(ETH_HEADER));
	assert(IS_ETH_TYPE(type), "这个不是以太网类型");

	eth->Type	= type;
	eth->DestMac = remote;
	eth->SrcMac  = Mac;

	len += sizeof(ETH_HEADER);
	//if(len < 60) len = 60;	// 以太网最小包60字节

	/*auto name = "Unkown";
	switch(type)
	{
		case ETH_ARP: { name = "ARP"; break; }
		case ETH_IP: { name = "IP"; break; }
		case ETH_IPv6: { name = "IPv6"; break; }
	}
	debug_printf("SendEthernet: type=0x%04x %s, len=%d(0x%x) ", type, name, len, len);
	Mac.Show();
	debug_printf(" => ");
	remote.Show();
	debug_printf("\r\n");*/
	/*ByteArray((byte*)eth->Next(), len).Show(true);*/

	return _port->Write(Buffer(eth, len));
}

bool TinyIP::SendIP(IP_TYPE type, const IPAddress& remote, const byte* buf, uint len)
{
	IP_HEADER* ip = (IP_HEADER*)(buf - sizeof(IP_HEADER));
	assert(IS_IP_TYPE(type), "这个不是IP消息类型");

	ip->DestIP = remote.Value;
	ip->SrcIP = IP.Value;

	ip->Version = 4;
	//ip->TypeOfService = 0;
	ip->Length = sizeof(IP_HEADER) / 4;	// 暂时不考虑附加数据
	ip->TotalLength = _REV16(sizeof(IP_HEADER) + len);
	//ip->Flags = 0x40;
	//ip->FragmentOffset = 0;
	//ip->TTL = 64;
	ip->Protocol = type;

	// 报文唯一标识。用于识别重组等
	static ushort g_Identifier = 1;
	ip->Identifier = _REV16(g_Identifier++);

	// 网络序是大端
	ip->Checksum	= 0;
	ip->Checksum	= _REV16(CheckSum(nullptr, (byte*)ip, sizeof(IP_HEADER), 0));

	auto arp	= (ArpSocket*)Arp;
	MacAddress mac;
	if(!arp->Resolve(remote, mac))
	{
#if NET_DEBUG
		debug_printf("No Mac For ");
		remote.Show();
		debug_printf("\r\n");
#endif
		return false;
	}

	/*auto name = "Unkown";
	switch(type)
	{
		case IP_ICMP: { name = "ICMP"; break; }
		case IP_IGMP: { name = "IGMP"; break; }
		case IP_TCP: { name = "TCP"; break; }
		case IP_UDP: { name = "UDP"; break; }
	}
	debug_printf("SendIP: type=0x%04x %s, len=%d(0x%x) ", type, name, len, len);
	IP.Show();
	debug_printf(" => ");
	remote.Show();
	debug_printf("\r\n");*/

	return SendEthernet(ETH_IP, mac, (byte*)ip, sizeof(IP_HEADER) + len);
}

Socket* TinyIP::CreateSocket(NetType type)
{
	switch(type)
	{
		case NetType::Tcp:
			return new TcpSocket(this);

		case NetType::Udp:
			return new UdpSocket(this);

		default:
			return nullptr;
	}
}

#define TinyIP_HELP
#ifdef TinyIP_HELP
ushort TinyIP::CheckSum(IPAddress* remote, const byte* buf, uint len, byte type)
{
    // type 0=ip
    //      1=udp
    //      2=tcp
    uint sum = 0;

	// !!谨记网络是大端
    if(type == 1 || type == 2)
	{
        // UDP/TCP的校验和需要计算UDP首部加数据荷载部分，但也需要加上UDP伪首部。
		// 这个伪首部指，源地址、目的地址、UDP数据长度、协议类型（0x11），协议类型就一个字节，但需要补一个字节的0x0，构成12个字节。
		// 源地址。其实可以按照4字节累加，反正后面会把高位移位到低位累加，但是得考虑溢出的问题。
		sum += _REV16(IP.Value & 0xFFFF);
		sum += _REV16(IP.Value >> 16);
		sum += _REV16(remote->Value & 0xFFFF);
		sum += _REV16(remote->Value >> 16);

		// 数据长度
		sum += len;

		// 加上协议类型
		if(type == 1)
			sum += IP_UDP;
		else if(type == 2)
			sum += IP_TCP;
	}
    // 按16位字计算和
    while(len > 1)
    {
        sum += 0xFFFF & (*buf << 8 | *(buf + 1));
        buf += 2;
        len -= 2;
    }
    // 如果字节个数不是偶数个，这里会剩余1，后面补0
    if (len)
    {
        sum += (0xFF & *buf) << 8;
    }
    // 现在计算sum字节的和，直到只有16位长
    while (sum >> 16)
    {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    // 取补码
    return (ushort)(sum ^ 0xFFFF);
}

bool TinyIP::IsBroadcast(const IPAddress& ip)
{
	// 全网广播
	if(ip.IsBroadcast()) return true;

	if(ip.Value == (IP.Value | ~Mask.Value)) return true;

	return false;
}
#endif

/******************************** TinySocket ********************************/
TinySocket::TinySocket(TinyIP* tip, IP_TYPE type)
{
	assert(tip, "空的Tip");

	Tip		= tip;
	Type	= type;
	Enable	= false;

	// 除了ARP以外，加入到列表
	if(type != IP_NONE) tip->Sockets.Add(this);
}

TinySocket::~TinySocket()
{
	assert(Tip, "空的Tip");

	Enable = false;
	// 从TinyIP中删除当前Socket
	Tip->Sockets.Remove(this);
}
