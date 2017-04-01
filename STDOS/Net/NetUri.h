#ifndef _NetUri_H_
#define _NetUri_H_

#include "IPAddress.h"
#include "IPEndPoint.h"

// Э������
enum class NetType
{
	Unknown	= 0,
	Tcp		= 6,
	Udp		= 17,
	Http	= 80,
};

// ������Դ
class NetUri
{
public:
	IPAddress	Address;	// ��ַ
	ushort		Port;		// �˿�
	NetType		Type;		// Э������
	String		Host;		// Զ�̵�ַ���ַ�����ʽ��������IP�ַ���

	NetUri();
	NetUri(const String& uri);
	NetUri(NetType type, const IPAddress& addr, ushort port);
	NetUri(NetType type, const String& host, ushort port);

	bool IsTcp() const { return Type == NetType::Tcp; }
	bool IsUdp() const { return Type == NetType::Udp; }
	
	String ToString() const;
};

#endif
