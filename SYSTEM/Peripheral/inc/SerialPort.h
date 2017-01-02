#pragma once

#include "ITransport.h"
#include "Sys.h"

//�ص��������� downָʾ���»��ǵ��𴥷�
typedef void(*BtnInCallback)(ITransport* transport,Buffer & bs);

class SerialPort:public ITransport
{
	public:
		SerialPort(MessagePort_T comport=COM1,uint baudrate=115200);
		void Open();//��
		void Register(BtnInCallback onin);
		//void Write(byte buf[]);//��������
	protected:
		MessagePort_T comPort;
		uint baudRate;
		BtnInCallback onIn;//���յ�����
		
};
