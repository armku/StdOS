#include "SerialPort.h"


SerialPort::SerialPort(MessagePort_T comport,uint baudrate)
{
	this->comPort=comport;
	this->baudRate=baudrate;
}
//打开
void SerialPort::Open()
{
	
}
void SerialPort::Register(BtnInCallback onin)
{
	this->onIn=onin;
}
//写数据
//void SerialPort::Write(Buffer& bs)
//{
//	
//}
