#include "SerialPort.h"


SerialPort::SerialPort(MessagePort_T comport,uint baudrate)
{
	this->comPort=comport;
	this->baudRate=baudrate;
}
//��
void SerialPort::Open()
{
	
}
void SerialPort::Register(BtnInCallback onin)
{
	this->onIn=onin;
}
//д����
//void SerialPort::Write(Buffer& bs)
//{
//	
//}
