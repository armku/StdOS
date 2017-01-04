#pragma once

#include "ITransport.h"
#include "Sys.h"
#include "AlternatePort.h"


//回调函数定义 down指示按下还是弹起触发
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
		void Open();//打开
		void Register(BtnInCallback onin);
		//void Write(byte buf[]);//发送数据
	protected:
		MessagePort_T comPort;
		uint baudRate;
		BtnInCallback onIn;//接收到数据
		
};
