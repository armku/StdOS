#pragma once

#include "ITransport.h"
#include "Sys.h"
#include "AlternatePort.h"


//�ص��������� downָʾ���»��ǵ��𴥷�
typedef void(*BtnInCallback)(ITransport* transport,Buffer & bs);

class SerialPort:public ITransport
{
	private:
    byte _index;
    byte _parity;
    byte _dataBits;
    byte _stopBits;
    int _baudRate;
	
	//AlternatePort _tx;
	
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
