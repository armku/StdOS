#ifndef _Ethernet_H_
#define _Ethernet_H_

#include "Kernel\Sys.h"
#include "..\Net\MacAddress.h"

// TCP/IPЭ��ͷ���ṹ��


// �ֽ���
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN   1
#endif

#pragma pack(push)	// �������״̬
// ǿ�ƽṹ����շ���ռ�
#pragma pack(1)

// IP��ַ
typedef uint IPAddr;

// Mac��ַ���ṹ����඼����
//typedef struct _MacAddress MacAddr;
//struct _MacAddress
class MacAddr
{
public:
	// ������תΪMac��ַ��ȡ�ڴ�ǰ6�ֽڡ���Ϊ��С�ֽ�����Ҫv4��ǰ��v2�ں�
	// ����Mac��ַ12-34-56-78-90-12��v4��12-34-56-78��v2��90-12��ulong��0x0000129078563412
	uint v4;
	ushort v2;

	MacAddr(UInt64 v = 0)
	{
		v4 = v & 0xFFFFFFFF;
		v2 = (v >> 32) & 0xFFFF;
	}

	// �Ƿ�㲥��ַ��ȫ0��ȫ1
	bool IsBroadcast() { return (!v4 && !v2) || (v4 == 0xFFFFFFFF && v2 == 0xFFFF); }

	MacAddr& operator=(UInt64 v)
	{
		v4 = v & 0xFFFFFFFF;
		v2 = (v >> 32) & 0xFFFF;

		return *this;
	}
	UInt64 Value()
	{
		UInt64 v = v4;
		v |= ((UInt64)v2) << 32;
		return v;
	}

	MacAddr& operator=(const MacAddress& v) { *this = v.Value; return *this; }

	friend bool operator==(MacAddr& addr1, MacAddr& addr2)
	{
		return addr1.v4 == addr2.v4 && addr1.v2 == addr2.v2;
	}
	friend bool operator!=(MacAddr& addr1, MacAddr& addr2)
	{
		return addr1.v4 != addr2.v4 || addr1.v2 != addr2.v2;
	}
};
//}MacAddr;

#define IP_FULL 0xFFFFFFFF
#define MAC_FULL 0xFFFFFFFFFFFFFFFFull

// ��̫��Э������
typedef enum
{
	ETH_ARP = 0x0608,
	ETH_IP = 0x0008,
	ETH_IPv6 = 0xDD86,
}ETH_TYPE;

#define IS_ETH_TYPE(type) (type == ETH_ARP || type == ETH_IP || type == ETH_IPv6)

//Macͷ�����ܳ���14�ֽ�
typedef struct _ETH_HEADER
{
	MacAddr		DestMac;	// Ŀ��mac��ַ
	MacAddr		SrcMac;		// Դmac��ַ
	ETH_TYPE	Type;		// ��̫������

	uint Size() { return sizeof(this[0]); }
	uint Offset() { return Size(); }
	byte* Next() { return (byte*)this + Size(); }
}ETH_HEADER;

// IPЭ������
typedef enum
{
	IP_NONE = 0,
	IP_ICMP = 1,
	IP_IGMP = 2,
	IP_TCP = 6,
	IP_UDP = 17,
}IP_TYPE;

#define IS_IP_TYPE(type) (type == IP_ICMP || type == IP_IGMP || type == IP_TCP || type == IP_UDP)

// IPͷ�����ܳ���20=0x14�ֽڣ�ƫ��14=0x0E����������п�ѡ���ݣ�Length����ͷ���ܳ��ȣ�4�ı�����
typedef struct _IP_HEADER
{
#if LITTLE_ENDIAN
	byte	Length : 4;		// �ײ�����
	byte	Version : 4;		// �汾
#else
	byte	Version : 4;		// �汾
	byte	Length : 4;		// �ײ����ȡ�ÿ����λ4���ֽ�
#endif
	byte	TypeOfService;	// ��������
	ushort	TotalLength;	// �ܳ���
	ushort	Identifier;		// ��־
	byte	Flags;			// ��ʶ�Ƿ�����ݰ����зֶ�
	byte	FragmentOffset;	// ��¼�ֶε�ƫ��ֵ�������߻�������ֵ�������ݰ����������
	byte	TTL;			// ����ʱ��
	IP_TYPE	Protocol;		// Э��
	ushort	Checksum;		// �����
	IPAddr	SrcIP;			// ԴIP��ַ
	IPAddr	DestIP;			// Ŀ��IP��ַ

	void Init(IP_TYPE type, bool recursion = false)
	{
		Version = 4;
		Length = sizeof(this[0]) >> 2;
		TypeOfService = 0;
		Flags = 0;
		FragmentOffset = 0;
		TTL = 64;
		Protocol = type;

		if (recursion) Prev()->Type = ETH_IP;
	}

	uint Size() { return (Length <= 5) ? sizeof(this[0]) : (Length << 2); }
	uint Offset() { return Prev()->Offset() + Size(); }
	ETH_HEADER* Prev() { return (ETH_HEADER*)((byte*)this - sizeof(ETH_HEADER)); }
	//byte* Next()		{ return (byte*)this + sizeof(&this[0]); }
	byte* Next() { return (byte*)this + Size(); }
}IP_HEADER;

typedef enum
{
	TCP_FLAGS_FIN = 1,		// �������������־λ��Ϊ1��ʾ���������������ݰ�
	TCP_FLAGS_SYN = 2,		// ���������־λ��Ϊ1��ʾ�������ӵ��������ݰ�
	TCP_FLAGS_RST = 4,
	TCP_FLAGS_PUSH = 8,		// ��־λ��Ϊ1��ʾ�����ݰ�Ӧ�������д���
	TCP_FLAGS_ACK = 0x10,		// Ӧ���־λ��Ϊ1��ʾȷ�ϣ����ݰ�ΪӦ�����ݰ�
	TCP_FLAGS_URG = 0x20,
}TCP_FLAGS;

//TCPͷ�����ܳ���20=0x14�ֽڣ�ƫ��34=0x22����������п�ѡ���ݣ�Length����ͷ���ܳ��ȣ�4�ı�����
typedef struct _TCP_HEADER
{
	ushort	SrcPort;		// Դ�˿ں�
	ushort	DestPort;    	// Ŀ�Ķ˿ں�
	uint	Seq;			// ���к�
	uint	Ack;	        // ȷ�Ϻ�
#if LITTLE_ENDIAN
	byte	reserved_1 : 4;	// ����6λ�е�4λ�ײ�����
	byte	Length : 4;		// tcpͷ������
	byte	Flags : 6;		// 6λ��־
	byte	reserved_2 : 2;	// ����6λ�е�2λ
#else
	byte	Length : 4;		// tcpͷ������
	byte	reserved_1 : 4;	// ����6λ�е�4λ�ײ�����
	byte	reserved_2 : 2;	// ����6λ�е�2λ
	byte	Flags : 6;		// 6λ��־
#endif
	ushort	WindowSize;		// 16λ���ڴ�С
	ushort	Checksum;		// 16λTCP�����
	ushort	urgt_p;			// 16Ϊ����ָ��

	void Init(bool recursion = false)
	{
		Length = sizeof(this[0]) >> 2;

		reserved_1 = 0;
		reserved_2 = 0;
		//WindowSize = _REV16(8192);
		WindowSize = _REV16(1024);
		urgt_p = 0;

		if (recursion) Prev()->Init(IP_TCP, recursion);
	}

	uint Size() { return (Length <= 5) ? sizeof(this[0]) : (Length << 2); }
	uint Offset() { return Prev()->Offset() + Size(); }
	IP_HEADER* Prev() { return (IP_HEADER*)((byte*)this - sizeof(IP_HEADER)); }
	byte* Next() { return (byte*)this + Size(); }
}TCP_HEADER;

//UDPͷ�����ܳ���8�ֽڣ�ƫ��34=0x22
typedef struct _UDP_HEADER
{
	ushort SrcPort;		// Զ�˿ں�
	ushort DestPort;	// Ŀ�Ķ˿ں�
	ushort Length;      // ͷ�����ϸ��ص��ܳ���
	ushort Checksum;	// �����

	void Init(bool recursion = false)
	{
		Length = sizeof(this[0]);

		if (recursion) Prev()->Init(IP_UDP, recursion);
	}

	uint Size() { return sizeof(this[0]); }
	uint Offset() { return Prev()->Offset() + Size(); }
	IP_HEADER* Prev() { return (IP_HEADER*)((byte*)this - sizeof(IP_HEADER)); }
	byte* Next() { return (byte*)this + Size(); }
}UDP_HEADER;

//ICMPͷ�����ܳ���8�ֽڣ�ƫ��34=0x22
typedef struct _ICMP_HEADER
{
	byte	Type;			// ����
	byte	Code;			// ����
	ushort	Checksum;		// 16λ�����
	ushort	Identifier;		// ��ʶ��������Ping
	ushort	Sequence;		// ���кţ�������Ping

	void Init(bool recursion = false)
	{
		Type = 8;
		Code = 0;

		if (recursion) Prev()->Init(IP_ICMP, recursion);
	}

	uint Size() { return sizeof(this[0]); }
	uint Offset() { return Prev()->Offset() + Size(); }
	IP_HEADER* Prev() { return (IP_HEADER*)((byte*)this - sizeof(IP_HEADER)); }
	byte* Next() { return (byte*)this + Size(); }
}ICMP_HEADER;

// ARPͷ�����ܳ���28=0x1C�ֽڣ�ƫ��14=0x0E�����ܼ�18�ֽ����
typedef struct _ARP_HEADER
{
	ushort	HardType;		// Ӳ������
	ushort	NetType;	// Э������
	byte	HardLength;		// Ӳ����ַ����
	byte	ProtocolLength;	// Э���ַ����
	ushort	Option;			// ѡ��
	MacAddr	SrcMac;
	IPAddr	SrcIP;			// ԴIP��ַ
	MacAddr	DestMac;
	IPAddr	DestIP;			// Ŀ��IP��ַ
	//byte	Padding[18];	// ���չ�60�ֽ�

	void Init(bool recursion = false)
	{
		HardType = 0x0100;
		NetType = 0x08;
		HardLength = 6;
		ProtocolLength = 4;

		if (recursion) Prev()->Type = ETH_ARP;
	}

	uint Size() { return sizeof(this[0]); }
	uint Offset() { return Prev()->Offset() + Size(); }
	ETH_HEADER* Prev() { return (ETH_HEADER*)((byte*)this - sizeof(ETH_HEADER)); }
	byte* Next() { return (byte*)this + Size(); }
}ARP_HEADER;

// DHCPͷ�����ܳ���240=0xF0�ֽڣ�ƫ��42=0x2A�������ѡ����ƫ��282=0x11A
typedef struct _DHCP_HEADER
{
	byte	MsgType;		// ����client�͸�server�ķ������Ϊ1������Ϊ2
	byte	HardType;		// ��̫��1
	byte	HardLength;		// ��̫��6
	byte	Hops;			// �����ݰ��辭��router���ͣ�ÿվ��1������ͬһ���ڣ�Ϊ0
	uint	TransID;		// ����ID���Ǹ�����������ڿͻ��ͷ�����֮��ƥ���������Ӧ��Ϣ
	ushort	Seconds;		// ���û�ָ����ʱ�䣬ָ��ʼ��ַ��ȡ�͸��½��к��ʱ��
	ushort	Flags;			// ��0-15bits������һbitΪ1ʱ��ʾserver���Թ㲥��ʽ���ͷ���� client��������δʹ��
	IPAddr	ClientIP;		// �ͻ���IP
	IPAddr	YourIP;			// ���IP
	IPAddr	ServerIP;		// ������IP
	IPAddr	RelayIP;		// �м̴���IP
	byte	ClientMac[16];	// �ͻ���Ӳ����ַ
	byte	ServerName[64];	// ��������
	byte	BootFile[128];	// �����ļ���
	uint	Magic;			// ����0x63825363��С��0x63538263

	void Init(uint dhcpid, bool recursion = false)
	{
		// Ϊ�˰�ȫ�����һ��
		Buffer(this, sizeof(this[0])).Clear();

		MsgType = 1;
		HardType = 1;
		HardLength = 6;
		Hops = 0;
		TransID = _REV(dhcpid);
		//Flags = 0x80;	// ��0-15bits������һbitΪ1ʱ��ʾserver���Թ㲥��ʽ���ͷ���� client��������δʹ��
		SetMagic();

		if (recursion) Prev()->Init(recursion);
	}

	uint Size() { return sizeof(this[0]); }
	uint Offset() { return Prev()->Offset() + Size(); }
	UDP_HEADER* Prev() { return (UDP_HEADER*)((byte*)this - sizeof(UDP_HEADER)); }
	byte* Next() { return (byte*)this + Size(); }

	void SetMagic() { Magic = 0x63538263; }
	bool Valid() { return Magic == 0x63538263; }
}DHCP_HEADER;

// DHCP�����ѡ���ݸ�ʽΪ������+����+���ݡ�

typedef enum
{
	DHCP_OPT_Mask = 1,
	DHCP_OPT_Router = 3,
	DHCP_OPT_TimeServer = 4,
	DHCP_OPT_NameServer = 5,
	DHCP_OPT_DNSServer = 6,
	DHCP_OPT_LOGServer = 7,
	DHCP_OPT_HostName = 12,
	DHCP_OPT_MTU = 26,				// 0x1A
	DHCP_OPT_StaticRout = 33,		// 0x21
	DHCP_OPT_ARPCacheTimeout = 35,	// 0x23
	DHCP_OPT_NTPServer = 42,		// 0x2A
	DHCP_OPT_RequestedIP = 50,		// 0x32
	DHCP_OPT_IPLeaseTime = 51,		// 0x33
	DHCP_OPT_MessageType = 53,		// 0x35
	DHCP_OPT_DHCPServer = 54,		// 0x36
	DHCP_OPT_ParameterList = 55,	// 0x37
	DHCP_OPT_Message = 56,			// 0x38
	DHCP_OPT_MaxMessageSize = 57,	// 0x39
	DHCP_OPT_Vendor = 60,			// 0x3C
	DHCP_OPT_ClientIdentifier = 61,	// 0x3D
	DHCP_OPT_End = 255,
}DHCP_OPTION;

typedef enum
{
	DHCP_TYPE_Discover = 1,
	DHCP_TYPE_Offer = 2,
	DHCP_TYPE_Request = 3,
	DHCP_TYPE_Decline = 4,
	DHCP_TYPE_Ack = 5,
	DHCP_TYPE_Nak = 6,
	DHCP_TYPE_Release = 7,
	DHCP_TYPE_Inform = 8,
}DHCP_MSGTYPE;

typedef struct _DHCP_OPT
{
	DHCP_OPTION	Option;	// ����
	byte		Length;	// ����
	byte		Data;	// ����

	struct _DHCP_OPT* Next() { return (struct _DHCP_OPT*)((byte*)this + 2 + Length); }

	struct _DHCP_OPT* SetType(DHCP_MSGTYPE type)
	{
		Option = DHCP_OPT_MessageType;
		Length = 1;
		Data = type;

		return this;
	}

	struct _DHCP_OPT* SetData(DHCP_OPTION option, const Buffer& bs)
	{
		Option = option;
		Length = bs.Length();
		bs.CopyTo(0, &Data, -1);

		return this;
	}

	/*struct _DHCP_OPT* SetData(DHCP_OPTION option, String& str)
	{
		ByteArray bs(str);
		return SetData(option, bs);
	}*/

	struct _DHCP_OPT* SetData(DHCP_OPTION option, uint value)
	{
		Option = option;
		Length = 4;
		Buffer(&Data, 4) = &value;

		return this;
	}

	struct _DHCP_OPT* SetClientId(MacAddress& mac)
	{
		Option = DHCP_OPT_ClientIdentifier;
		Length = 1 + 6;
		Data = 1;	// ����ETHERNET=1
		mac.CopyTo(&Data + 1);

		return this;
	}

	struct _DHCP_OPT* End()
	{
		Option = DHCP_OPT_End;

		return this;
	}
}DHCP_OPT;

#pragma pack(pop)	// �ָ�����״̬

#endif
