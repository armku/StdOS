#include "OutputPort.h"

OutputPort::OutputPort(Pin pin,byte invert):Port()
{
	this->Set(pin);
	this->Init(pin,invert);
}
void OutputPort::Init(Pin pin,byte invert)
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
//Êä³ö¿ª¹Ø
void OutputPort::Write(bool onoff)
{
	if(binvert)
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
	else
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
}
void OutputPort::Open()
{
	
}
void OutputPort::Toggle()
{
    this->pinbit = !this->pinbit;
    if (this->pinbit)
    {
        this->Set();
    }
    else
    {
        this->Reset();
    }
}
