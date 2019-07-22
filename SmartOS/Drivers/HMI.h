#ifndef __HMI_H__
#define __HMI_H__

#include "Device\SerialPort.h"

class HMI
{
public:
	ITransport*	Port;	// �����

	HMI();
	~HMI();

	// ��ʼ���������Ĵ���
	void Init(COM idx, int baudrate = 115200); 
	// ��ʼ�����
	void Init(ITransport* port);	

	// �������ݵ����¼�
	uint OnReceive(Buffer& bs, void* param);
	static uint OnPortReceive(ITransport* sender, Buffer& bs, void* param, void* param2);

	// ����ָ�������
	void Send(const String& cmd);
	// ���ͽ�����־
	void SenDFlag();
};

#endif
