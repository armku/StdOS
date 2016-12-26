#include "OutputPort.h"

OutputPort::OutputPort(PinPort pin,bool invert):CPort(pin)
{
	this->SetModeOut_OD();
	this->invertPort=invert;
}
//true：高电平，false：低电平
void OutputPort::Write(const bool onoff)
{
	if(this->invertPort)
	{
		if(onoff)
		{
			this->Set();
		}
		else
		{
			this->Reset();
		}
	}
	else
	{
		if(onoff)
		{
			this->Reset();
		}
		else
		{
			this->Set();
		}
	}
}
//端口号，是否反转电平
void OutputPort::Init(PinPort pin,bool  invert)
{
	this->invertPort=invert;
}
		//赋值操作运算符
void OutputPort::operator = (const bool value)
{
	this->Write(value);
}
	
