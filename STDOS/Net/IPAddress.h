#ifndef _IPAddress_H_
#define _IPAddress_H_

#include "Core\ByteArray.h"
#include "Core\SString.h"

// IPЭ������
enum class ProtocolType
{
	Ip		= 0,
	Icmp	= 1,
	Igmp	= 2,
	Tcp		= 6,
	Udp		= 17,
};

// IP��ַ
class IPAddress
{
public:
	uint	Value;	// ��ַ

	IPAddress(int value)		{ Value = (uint)value; }
	IPAddress(uint value = 0)	{ Value = value; }
	IPAddress(const byte* ips);
	IPAddress(byte ip1, byte ip2, byte ip3, byte ip4);
	IPAddress(const Buffer& arr);

    IPAddress& operator=(int v)			{ Value = (uint)v; return *this; }
    IPAddress& operator=(uint v)		{ Value = v; return *this; }
    IPAddress& operator=(const byte* v);
    IPAddress& operator=(const Buffer& arr);
    IPAddress& operator=(const IPAddress& addr);

    // �������������[]����������������һ��ʹ���±�������
    byte& operator[](int i);

	// �ֽ�����
    ByteArray ToArray() const;
	void CopyTo(byte* ips) const;

	bool IsAny() const;
	bool IsBroadcast() const;
	// ��ȡ����
	uint GetSubNet(const IPAddress& mask) const;

	// ���������ַ�����ʾ��ʽ
	String ToString() const;
	void Show(bool newLine = false) const { ToString().Show(newLine); }

    friend bool operator==(const IPAddress& addr1, const IPAddress& addr2) { return addr1.Value == addr2.Value; }
    friend bool operator!=(const IPAddress& addr1, const IPAddress& addr2) { return addr1.Value != addr2.Value; }

	static const IPAddress& Any();
	static const IPAddress& Broadcast();

	// ���ַ���IP��ַ����ΪIPAddress
	static IPAddress Parse(const String& ipstr);
};

#endif
