#include "InputPort.h"

InputPort::InputPort(PinPort pin,byte invert):BasePort(pin)
{
	this->Init(pin,invert);
}
void InputPort::Init(PinPort pin,byte invert)
{
	this->SetModeOut_PP();
	switch(invert)
	{
		case 0:
			this->binvert=invert;
			break;
		case 1:
			this->binvert=invert;
			break;
		case 2:
			if(this->Read())
			{
				this->binvert=true;
			}
			else
			{
				this->binvert=false;
			}
			break;
		default:
			break;
	}
}
//��ȡ�˿ڵ�ƽ
byte InputPort::Read()
{
	return this->Read1();
}
void InputPort::Open()
{
	
}
//�����¼�����
void InputPort::UsePress()
{
	
}
