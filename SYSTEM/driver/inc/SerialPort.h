#pragma once

#include "Type.h"
#include "TSys.h"

class SerialPort
{
	public:
		SerialPort(MessagePort_T comport=COM1,uint baudrate=115200);
		void Open();//��
		void Write(byte buf[]);//��������
	protected:
		MessagePort_T comPort;
		uint baudRate;
		
};
