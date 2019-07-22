﻿#include "Security\Crc.h"
#include "Security\RC4.h"

#include "TinyConfig.h"
#include "TinyController.h"

//#define MSG_DEBUG DEBUG
#define MSG_DEBUG 0
#if MSG_DEBUG
	#define msg_printf debug_printf
#else
	#define msg_printf(format, ...)
#endif

/*================================ 微网控制器 ================================*/
void SendTask(void* param);
void StatTask(void* param);

// 构造控制器
TinyController::TinyController() : Controller()
{
	MinSize	= TinyMessage::MinSize;

	// 初始化一个随机地址
	Address = Sys.ID[0];
	// 如果地址为0，则使用时间来随机一个
	// 节点地址范围2~254，网关专用0x01，节点让步
	while(Address < 2 || Address > 254)
	{
		Sys.Delay(30);
		Address = (byte)Sys.Ms();
	}

	// 接收模式。0只收自己，1接收自己和广播，2接收所有。默认0
	Mode		= 0;
	Interval	= 20;
	Timeout		= 200;
	auto cfg	= TinyConfig::Create();
	if(cfg)
	{
		//bool flag	= false;
		// 调整重发参数
		if(cfg->Interval == 0 || cfg->Interval > 1000 || cfg->Timeout > 10000)
		{
			cfg->Interval	= Interval;
			cfg->Timeout	= Timeout;
		//	flag	= true;
		}

		if(cfg->Address	== 0)
		{
			cfg->Address	= Address;
		//	flag	= true;
		}
		//if(flag) cfg->Save();
	}

	_taskID		= 0;
	_Queue		= nullptr;
	QueueLength	= 8;

	// 默认屏蔽心跳日志
	Buffer(NoLogCodes, sizeof(NoLogCodes)).Clear();
	NoLogCodes[0] = 0x03;
}

TinyController::~TinyController()
{
	Sys.RemoveTask(_taskID);

	delete[] _Queue;
	_Queue	= nullptr;
}

void TinyController::ApplyConfig()
{
	auto cfg	= TinyConfig::Create();
	if(cfg)
	{
		// 调整参数
		Interval	= cfg->Interval;
		Timeout		= cfg->Timeout;
		Address		= cfg->Address;
	}
}

void TinyController::Open()
{
	if(Opened) return;

	assert(Port, "还没有传输口呢");

#if MSG_DEBUG
	debug_printf("TinyNet::Inited Address=%d (0x%02X) 使用传输接口 ", Address, Address);
	auto obj	= dynamic_cast<Object*>(Port);
	if(obj) obj->Show(true);
	//Port->Show(true);
	debug_printf("\t间隔: %dms\r\n", Interval);
	debug_printf("\t超时: %dms\r\n", Timeout);
	debug_printf("\t模式: %d ", Mode);
#endif

	// 接收模式。0只收自己，1接收自己和广播，2接收所有。默认0
	switch(Mode)
	{
		case 0:
			debug_printf("仅本地址");
			break;
		case 1:
			debug_printf("本地址和广播");
			break;
		case 2:
			debug_printf("接收所有");
			break;
	}
	debug_printf("\r\n");

	Controller::Open();

	// 初始化发送队列
	_Queue	= new MessageNode[QueueLength];
	//Buffer(_Queue, sizeof(*_Queue)).Clear();
	Buffer(_Queue, sizeof(MessageNode) * QueueLength).Clear();

	if(!_taskID)
	{
		_taskID = Sys.AddTask(SendTask, this, 0, 1, "微网队列");
		// 默认禁用，有数据要发送才开启
		Sys.SetTask(_taskID, false);
	}

#if MSG_DEBUG
	Sys.AddTask([](void* p){ return ((TinyController*)p)->ShowStat(); }, this, 1000, 15000, "微网统计");
#endif
}

void TinyController::ShowMessage(const TinyMessage& msg, bool send, const ITransport* port)
{
	if(msg.Ack) return;

	for(int i=0; i<ArrayLength(NoLogCodes); i++)
	{
		if(msg.Code == NoLogCodes[i]) return;
		if(NoLogCodes[i] == 0) break;
	}

#if MSG_DEBUG
	String name ="R24";
	auto obj	= dynamic_cast<Object*>(Port);
	if(obj)
	  name	= obj->ToString();

	msg_printf("%s", name.GetBuffer());
#endif
	int blank = 6;
	if(send && !msg.Reply)
	{
		msg_printf("::Send ");
		blank -= 5;
	}
	else
	{
		msg_printf("::");
	}

	if(msg.Error)
	{
		msg_printf("Error ");
		blank -= 6;
	}
	else if(msg.Ack)
	{
		msg_printf("Ack ");
		blank -= 4;
	}
	else if(msg.Reply)
	{
		msg_printf("Reply ");
		blank -= 6;
	}
	else if(!send)
	{
		msg_printf("Recv ");
		blank -= 5;
	}
	if(blank > 0)
	{
		String str(' ', blank);
		str.Show();
	}

#if MSG_DEBUG
	// 显示目标地址
	auto st	= msg.State;
	if(st)
	{
		msg_printf("Mac=");
		if(name == "R24")
			ByteArray(st, 5).Show();
		else if(name == "ShunCom")
			ByteArray(st, 2).Show();
		else
			ByteArray(st, 6).Show();
		msg_printf(" ");
	}
#endif

	msg.Show();
}

//加密。组网不加密，退网不加密
static bool Encrypt(Message& msg,  Buffer& pass)
{
	// 加解密。组网不加密，退网不加密
	if(msg.Length > 0 && pass.Length() > 0 && !(msg.Code == 0x01 || msg.Code == 0x02))
	{
		Buffer bs(msg.Data, msg.Length);
		RC4::Encrypt(bs, pass);
		return true;
	}
	return false;
}

bool TinyController::Dispatch(Stream& ms, Message* pmsg, void* param)
{
	/*byte* buf	= ms.Current();
	// 前移一个字节，确保不是自己的消息时，数据流能够移动
	ms.Seek(1);

	// 代码为0是非法的
	if(!buf[2]) return true;
	// 没有源地址是很不负责任的
	if(!buf[1]) return true;
	// 非广播包时，源地址和目的地址相同也是非法的
	if(buf[0] == buf[1]) return false;
	// 源地址是自己不要接收
	if(buf[1] == Address) return true;
	// 只处理本机消息或广播消息
	//if(tmsg.Dest != Address && tmsg.Dest != 0) return false;
	// 不是自己的消息，并且没有设置接收全部
	if(Mode < 2 && buf[0] != Address)
	{
		// 如果不是广播或者不允许广播
		if(Mode != 1 || buf[0] != 0) return true;
	}

	// 后移一个字节来弥补
	ms.Seek(-1);*/
	TinyMessage msg;
	return Controller::Dispatch(ms, &msg, param);
}

// 收到消息校验后调用该函数。返回值决定消息是否有效，无效消息不交给处理器处理
bool TinyController::Valid(const Message& _msg)
{
	auto& msg = (TinyMessage&)_msg;

	// 代码为0是非法的
	if(!msg.Code) return false;
	// 没有源地址是很不负责任的
	if(!msg.Src) return false;
	//if(msg!=Address) return false;
	// 非广播包时，源地址和目的地址相同也是非法的
	if(msg.Dest == msg.Src) return false;
	// 源地址是自己不要接收
	if(msg.Src == Address) return false;
	// 只处理本机消息或广播消息
	//if(msg.Dest != Address && msg.Dest != 0) return false;
	// 不是自己的消息，并且没有设置接收全部
	if(Mode < 2 && msg.Dest != Address)
	{
		// 如果不是广播或者不允许广播
		if(Mode != 1 || msg.Dest != 0) return false;
	}

	TS("TinyController::Valid");

#if MSG_DEBUG
	// 调试版不过滤序列号为0的重复消息
	if(msg.Seq != 0)
#endif
	{
		// Ack的包有可能重复，不做处理。正式响应包跟前面的Ack有相同的源地址和序列号
		if(!msg.Ack)
		{
			// 处理重复消息。加上来源地址，以免重复
			ushort seq = (msg.Src << 8) | msg.Seq;
			if(_Ring.Check(seq))
			{
				// 对方可能多次发同一个请求过来，都要做响应
				if(!msg.Reply && AckResponse(msg)) return false;

				//msg_printf("重复消息 Src=0x%02x Code=0x%02X Seq=0x%02X Retry=%d Reply=%d Ack=%d\r\n", msg.Src, msg.Code, msg.Seq, msg.Retry, msg.Reply, msg.Ack);
				return false;
			}
			_Ring.Push(seq);
		}
	}

	// 广播的响应消息也不要
	if(msg.Dest == 0 && msg.Reply) return false;

	// 仅处理本节点的确认消息
	if(msg.Dest == Address)
	{
		// 如果是确认消息或响应消息，及时更新请求队列
		if(msg.Ack)
		{
			AckRequest(msg);
			// 如果只是确认消息，不做处理
			return false;
		}
		// 响应消息消除请求，请求消息要求重发响应
		if(msg.Reply)
			AckRequest(msg);
		else
		{
			if(AckResponse(msg))
			{
				debug_printf("匹配请求 ");
				return false;
			}
		}
	}

	if(msg.Dest == Address)
	{
		ByteArray  key(0);
		GetKey(msg.Src, key);
		if(key.Length() > 0) Encrypt(msg, key);
	}

#if MSG_DEBUG
	// 尽量在Ack以后再输出日志，加快Ack处理速度
	ShowMessage(msg, false, Port);
#endif

	Total.Receive++;

	return true;
}

// 处理收到的响应包或Ack包，消除请求
void TinyController::AckRequest(const TinyMessage& msg)
{
	if(!msg.Ack && !msg.Reply) return;

	for(int i=0; i<QueueLength; i++)
	{
		auto& node = _Queue[i];
		if(node.Using && node.Seq == msg.Seq)
		{
			int cost = (int)(Sys.Ms() - node.StartTime);
			if(cost < 0) cost = -cost;

			Total.Cost	+= cost;
			Total.Success++;
			Total.Bytes	+= node.Length;

			// 该传输口收到响应，从就绪队列中删除
			node.Using = 0;

			/*if(msg.Ack)
				msg_printf("收到确认 ");
			else
				msg_printf("响应确认 ");

			msg_printf("Src=0x%02x Code=0x%02X Seq=0x%02X Retry=%d Cost=%dms \r\n", msg.Src, msg.Code, msg.Seq, msg.Retry, cost);*/
			return;
		}
	}

#if MSG_DEBUG
	if(msg.Ack)
	{
		msg_printf("无效确认 ");
		ShowMessage(msg, false, Port);
	}
#endif
}

// 处理对方发出的请求，如果已响应则重发响应
bool TinyController::AckResponse(const TinyMessage& msg)
{
	if(msg.Reply) return false;
	// 广播消息不要给确认
	if(msg.Dest == 0) return false;

	TS("TinyController::AckResponse");

	for(int i=0; i<QueueLength; i++)
	{
		auto& node = _Queue[i];
		if(node.Using && node.Seq == msg.Seq)
		{
			// 马上重发这条响应
			node.Next	= 0;

			Sys.SetTask(_taskID, true, 0);

			//msg_printf("重发响应 ");
			//msg_printf("Src=0x%02x Code=0x%02X Seq=0x%02X Retry=%d \r\n", msg.Src, msg.Code, msg.Seq, msg.Retry);
			return true;
		}
	}

	return false;
}

static byte _Sequence	= 0;
// 发送消息，传输口参数为空时向所有传输口发送消息
bool TinyController::Send(Message& _msg)
{
	auto& msg = (TinyMessage&)_msg;

	// 附上自己的地址
	msg.Src = Address;

	// 附上序列号。响应消息保持序列号不变
	if(!msg.Reply) msg.Seq = ++_Sequence;

	ByteArray  key;
	GetKey(msg.Dest, key);
	if(key.Length() > 0) Encrypt(msg, key);

#if MSG_DEBUG
	ShowMessage(msg, true, Port);
#endif

	return Post(msg, -1);
}

bool TinyController::Reply(Message& _msg)
{
	auto& msg = (TinyMessage&)_msg;

	// 回复信息，源地址变成目的地址
	if(msg.Dest == Address && msg.Src != Address) msg.Dest = msg.Src;
	msg.Reply = 1;

	return Send(msg);
}

// 广播消息，不等待响应和确认
bool TinyController::Broadcast(TinyMessage& msg)
{
	TS("TinyController::Broadcast");

	msg.NoAck	= true;
	msg.Src		= Address;
	msg.Dest	= 0;

	// 附上序列号。响应消息保持序列号不变
	if(!msg.Reply) msg.Seq = ++_Sequence;

#if MSG_DEBUG
	ShowMessage(msg, true, Port);
#endif

	if(!Port->Open()) return false;

	Total.Broadcast++;

	return Controller::SendInternal(msg);
}

// 放入发送队列，超时之前，如果对方没有响应，会重复发送，-1表示采用系统默认超时时间Timeout
bool TinyController::Post(const TinyMessage& msg, int msTimeout)
{
	TS("TinyController::Post");

	// 如果没有传输口处于打开状态，则发送失败
	if(!Port->Open()) return false;

	if(msTimeout < 0) msTimeout = Timeout;
	// 如果确定不需要响应，则改用Post
	if(msTimeout <= 0 || msg.NoAck || msg.Ack)
	{
		Total.Broadcast++;
		return Controller::SendInternal(msg);
	}

	// 针对Zigbee等不需要Ack确认的通道
	if(Timeout < 0)
	{
		Total.Broadcast++;
		return Controller::SendInternal(msg);
	}

	// 准备消息队列
	auto now	= Sys.Ms();
	int idx		= -1;
	for(int i=0; i<QueueLength; i++)
	{
		auto& node = _Queue[i];
		// 未使用，或者即使使用也要抢走已过期的节点
		if(!node.Using || node.EndTime < now)
		{
			node.Seq	= 0;
			node.Using	= 1;
			idx	= i;
			break;
		}
	}
	// 队列已满
	if(idx < 0)
	{
		debug_printf("TinyController::Post 发送队列已满！ \r\n");
		return false;
	}

	_Queue[idx].Set(msg, msTimeout);

	if(msg.Reply)
		Total.Reply++;
	else
		Total.Msg++;

	Sys.SetTask(_taskID, true, 0);

	return true;
}

void SendTask(void* param)
{
	auto control = (TinyController*)param;
	control->Loop();
}

typedef struct{
	byte Retry:4;	// 重发次数。
	//byte TTL:1;		// 路由TTL。最多3次转发
	byte NoAck:1;	// 是否不需要确认包
	byte Ack:1;		// 确认包
	byte _Error:1;	// 是否错误
	byte _Reply:1;	// 是否响应
} TFlags;

void TinyController::Loop()
{
	TS("TinyController::Loop");

	/*
	队列机制：
	1，定时发送请求消息，直到被响应消去或者超时
	2，发送一次响应消息，直到超时，中途有该序列的请求则直接重发响应
	3，不管请求还是响应，同样方式处理超时，响应的下一次时间特别长
	4，如果某个请求序列对应的响应存在，则直接将其下一次时间置为0，让其马上重发
	5，发送响应消息，不好统计速度和成功率
	6，发送次数不允许超过5次
	*/

	int count = 0;
	for(int i=0; i<QueueLength; i++)
	{
		auto& node = _Queue[i];
		if(!node.Using || node.Seq == 0) continue;

		auto flag = (TFlags*)&node.Data[3];
		bool reply	= flag->_Reply;
		// 可用请求消息数，需要继续轮询
		if(!reply) count++;

		// 检查时间。至少发送一次
		if(node.Times > 0)
		{
			UInt64 now = Sys.Ms();

			// 已过期则删除
			if(node.EndTime < now || node.Times > 50)
			{
				//if(!reply) msg_printf("消息过期 Dest=0x%02X Seq=0x%02X Times=%d\r\n", node.Data[0], node.Seq, node.Times);
				node.Using	= 0;
				node.Seq	= 0;

				continue;
			}

			// 下一次发送时间还没到，跳过
			if(node.Next > now) continue;

			//msg_printf("重发消息 Dest=0x%02X Seq=0x%02X Times=%d\r\n", node.Data[0], node.Seq, node.Times + 1);
		}

		node.Times++;

		// 发送消息
		Buffer bs(node.Data, node.Length);
		if(node.Length <= 0 || node.Length > 32)
		{
			debug_printf("node=%p Length=%d Seq=0x%02X Times=%d Next=%d EndTime=%d\r\n", (uint)&node, node.Length, node.Seq, node.Times, (uint)node.Next, (uint)node.EndTime);
		}
		if(node.Mac[0])
			Port->Write(bs, &node.Mac[1]);
		else
			Port->Write(bs);

		// 递增重试次数
		flag->Retry++;

		// 请求消息列入统计
		if(!reply)
		{
			// 增加发送次数统计
			Total.Send++;

			// 分组统计
			if(Total.Send >= 1000)
			{
				Last	= Total;
				Total.Clear();
			}
		}

		// 计算下一次重发时间
		{
			UInt64 now	= Sys.Ms();
			//node.LastSend	= now;

			// 随机延迟。随机数1~5。每次延迟递增
			//byte rnd	= (uint)now % 3;
			//node.Next	= now + (rnd + 1) * Interval;
			if(!reply)
				node.Next	= now + Interval;
			else
				node.Next	= now + 60000;
		}
	}

	// 没有可用请求时，停止轮询
	if(count == 0) Sys.SetTask(_taskID, false);
}

// 显示统计信息
void TinyController::ShowStat() const
{
#if MSG_DEBUG
	static uint lastSend = 0;
	static uint lastReceive = 0;

	int tsend	= Total.Send;
	if(tsend == lastSend && Total.Receive == lastReceive) return;
	lastSend	= tsend;
	lastReceive	= Total.Receive;

	uint rate	= 100;
	uint tack	= Last.Success + Total.Success;
	uint tmsg	= Last.Msg + Total.Msg;
	uint tcost	= Last.Cost + Total.Cost;
	tsend		+= Last.Send;
	if(tsend > 0)
		rate	= tack * 100 / tmsg;
	uint cost	= 0;
	if(tack > 0)
		cost	= tcost / tack;
	uint speed	= 0;
	if(tcost > 0)
		speed	= (Last.Bytes + Total.Bytes) * 1000 / tcost;
	uint retry	= 0;
	if(tmsg > 0)
		retry	= tsend * 100 / tmsg;
	msg_printf("Tiny::State 成功=%d%% %d/%d/%d 平均=%dms 速度=%d Byte/s 次数=%d.%02d 接收=%d 响应=%d 广播=%d \r\n", rate, tack, tmsg, tsend, cost, speed, retry/100, retry%100, Last.Receive + Total.Receive, Last.Reply + Total.Reply, Last.Broadcast + Total.Broadcast);
#endif
}

/*================================ 信息节点 ================================*/
void MessageNode::Set(const TinyMessage& msg, int msTimeout)
{
	Times		= 0;
	//LastSend	= 0;

	UInt64 now	= Sys.Ms();
	StartTime	= now;
	EndTime		= now + msTimeout;

	Next		= 0;
	// 响应消息只发一次，然后长时间等待
	if(msg.Reply) Next	= now + 60000;

	// 注意，此时指针位于0，而内容长度为缓冲区长度
	Stream ms(Data, ArrayLength(Data));
	msg.Write(ms);
	Length		= ms.Position();
	if(Length > 32) debug_printf("Length=%d \r\n", Length);

	Buffer mcs(Mac, ArrayLength(Mac));
	mcs[0]	= 0;
	if(msg.State)
	{
		mcs[0]	= mcs.Length();
		mcs.Copy(1, msg.State, -1);
	}
	Seq			= msg.Seq;
}

/*================================ 环形队列 ================================*/
RingQueue::RingQueue()
{
	Index	= 0;
	Buffer(Arr, sizeof(Arr)).Clear();
	Expired	= 0;
}

void RingQueue::Push(ushort item)
{
	Arr[Index++] = item;
	if(Index == ArrayLength(Arr)) Index = 0;

	// 更新过期时间，10秒
	Expired = Sys.Ms() + 10000;
}

int RingQueue::Find(ushort item) const
{
	for(int i=0; i < ArrayLength(Arr); i++)
	{
		if(Arr[i] == item) return i;
	}

	return -1;
}

bool RingQueue::Check(ushort item)
{
	// Expired为0说明还没有添加任何项
	if(!Expired) return false;

	// 首先检查是否过期。如果已过期，说明很长时间都没有收到消息
	if(Expired < Sys.Ms())
	{
		//msg_printf("环形队列过期，清空 \r\n");
		// 清空队列，重新开始
		Index	= 0;
		Buffer(Arr, sizeof(Arr)).Clear();
		Expired	= 0;

		return false;
	}

	// 再检查是否存在
	return Find(item) >= 0;
}

TinyStat::TinyStat()
{
	Clear();
}

TinyStat& TinyStat::operator=(const TinyStat& ts)
{
	Buffer(this, sizeof(this[0]))	= &ts;

	return *this;
}

void TinyStat::Clear()
{
	Buffer(this, sizeof(this[0])).Clear();
}
