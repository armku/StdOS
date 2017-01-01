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

void InputPort::Open()
{
	
}
//启用事件处理
void InputPort::UsePress()
{
	
}
