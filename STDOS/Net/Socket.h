#ifndef _Socket_H_
#define _Socket_H_

#include "IPAddress.h"
#include "IPEndPoint.h"
#include "MacAddress.h"
#include "NetUri.h"

#include "Core\Delegate.h"

class NetworkInterface;

// Socket�ӿ�
class Socket
{
public:
	NetworkInterface*	Host;	// ����
	NetType	Protocol;	// Э������

	IPEndPoint	Local;	// ���ص�ַ���������ؾ�����IP��ַ��ʵ�ʼ����Ķ˿ڣ���1024��ʼ�ۼ�
	IPEndPoint	Remote;	// Զ�̵�ַ
	String		Server;	// Զ�̵�ַ���ַ�����ʽ��������IP�ַ���

	// ������������������ΪӦ�ò����Ҫ�ͷŸýӿ�
	virtual ~Socket() { }

	//// Ӧ�����ã��޸�Զ�̵�ַ�Ͷ˿�
	//virtual bool Change(const String& remote, ushort port) { return false; };

	// ��������
	virtual bool Send(const Buffer& bs) = 0;
	virtual bool SendTo(const Buffer& bs, const IPEndPoint& remote) { return Send(bs); }
	// ��������
	virtual uint Receive(Buffer& bs) = 0;

	// ȫ�־�̬
	static Socket* CreateClient(const NetUri& uri);
	static Socket* CreateRemote(const NetUri& uri);
};

#endif
