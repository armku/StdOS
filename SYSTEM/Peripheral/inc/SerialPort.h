#pragma once

#include "Type.h"
#include "TSys.h"

class SerialPort
{
	public:
		SerialPort(MessagePort_T comport=COM1,uint baudrate=115200);
		void Open();//打开
		void Write(byte buf[]);//发送数据
	protected:
		MessagePort_T comPort;
		uint baudRate;
		
};
