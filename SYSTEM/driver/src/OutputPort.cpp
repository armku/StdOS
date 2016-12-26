#include "OutputPort.h"

OutputPort::OutputPort(PinPort pin,bool invert):CPort(pin)
{
	this->SetModeOut_OD();
	this->invertPort=invert;
}
//true���ߵ�ƽ��false���͵�ƽ
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
//�˿ںţ��Ƿ�ת��ƽ
void OutputPort::Init(PinPort pin,bool  invert)
{
	this->invertPort=invert;
}
		//��ֵ���������
void OutputPort::operator = (const bool value)
{
	this->Write(value);
}
	
