#ifndef _SmartOS_DNS_H_
#define _SmartOS_DNS_H_

#include "Net\ITransport.h"

// DNSЭ��
class DNS
{
public:
	NetworkInterface&	Host;	// ����

	DNS(NetworkInterface& host, const IPAddress& dns);
	~DNS();

	IPAddress Query(const String& domain, int msTimeout = 1000);	// ����

	// ��ݲ�ѯ����������ֱ�Ӳ�ѯ���
	static IPAddress Query(NetworkInterface& host, const String& domain, int times = 5, int msTimeout = 1000);

private:
	static uint OnReceive(ITransport* port, Buffer& bs, void* param, void* param2);
	void Process(Buffer& bs, const IPEndPoint& server);

	Socket*		_Socket;
	Array*		_Buffer;
};

#endif
