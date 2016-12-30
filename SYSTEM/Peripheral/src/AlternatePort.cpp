#include "AlternatePort.h"

//读取输入电平
byte AlternatePort::ReadInput()
{
	return this->Read();
}