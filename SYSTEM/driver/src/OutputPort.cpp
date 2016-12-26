#include "OutputPort.h"

COutputPort::COutputPort(PinPort pin):CPort(pin)
{
	this->SetModeOut_OD();
}
