#include "AlternatePort.h"

AlternatePort::AlternatePort()
{
}

AlternatePort::AlternatePort(PinPort pin):OutputPort(pin)
{
}

//��ȡ�����ƽ
byte AlternatePort::ReadInput()
{
	return this->Read();
}
