#include "AlternatePort.h"

AlternatePort::AlternatePort()
{
}

AlternatePort::AlternatePort(Pin pin):OutputPort(pin)
{
}

//读取输入电平
byte AlternatePort::ReadInput()
{
	return this->Read();
}
