#ifndef _TinyIP_DHCP_H_
#define _TinyIP_DHCP_H_

// DHCPЭ��
class Dhcp
{
private:
	uint dhcpid;	// ����ID
	uint taskID;	// ����ID
	UInt64 _expired;	// Ŀ�����ʱ�䣬����
	Socket*	_Socket;

	void Discover();
	void Request();
	void PareOption(Stream& ms);

	void SendDhcp(byte* buf, uint len);

	static void Loop(void* param);
public:
	NetworkInterface&	Host;	// ����
	IPAddress	IP;			// ��ȡ��IP��ַ
	IPAddress	Server;

	uint ExpiredTime;	// ����ʱ�䣬Ĭ��5000����
	bool Running;	// ��������
	bool Result;	// �Ƿ��ȡIP�ɹ�
	byte Times;		// ���д���
	byte MaxTimes;	// ������Դ�����Ĭ��6�Σ������ô�����Ȼʧ����ָ���һ������

	Dhcp(NetworkInterface& host);
	~Dhcp();

	void Start();	// ��ʼ
	void Stop();	// ֹͣ

	Delegate<Dhcp&>	OnStop;	// ��this����

private:
	static uint OnReceive(ITransport* port, Buffer& bs, void* param, void* param2);
	void Process(Buffer& bs, const IPEndPoint& ep);
};

#endif
