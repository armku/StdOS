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
