#pragma once

#include "Type.h"
#include "Buffer.h"

class ITransport;
class SerialPortOld;

//�ص���������,�������ݵ���ʱ����
typedef uint (*IOnUsartRead)(ITransport* transport,Buffer& bs,void* param);


typedef uint (*TransportHandler)(ITransport* transport,Buffer& bs,void* param); //��ʱ������

class ITransport
{
	public:
	void SendData(byte data);		
	void SendBuffer(byte *buff,int length=-1);//��������
	void SendBuffer(char *buff,int length=-1);//��������
	void Register(IOnUsartRead handler,SerialPortOld *sp); // ע�����ݵ����¼�
};
