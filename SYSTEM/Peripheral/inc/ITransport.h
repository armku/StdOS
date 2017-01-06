#pragma once

#include "Type.h"
#include "Buffer.h"

class ITransport;
class SerialPortOld;

//�ص���������,�������ݵ���ʱ����
typedef void(*IOnUsartRead)(ITransport* transport, Buffer& bs,void* param);

class ITransport
{
	public:
	void SendData(byte data);		
	void SendBuffer(byte *buff,int length=-1);//��������
	void SendBuffer(char *buff,int length=-1);//��������
	void Register(IOnUsartRead handler,SerialPortOld spo); // ע�����ݵ����¼�	
};
