#include "AlternatePort.h"

AlternatePort::AlternatePort()
{
}

AlternatePort::AlternatePort(Pin pin):OutputPort(pin)
{
}

//��ȡ�����ƽ
byte AlternatePort::ReadInput()
{
	return this->Read();
}
