#pragma once

#include "OutputPort.h"

class AlternatePort:public OutputPort
{
	public:
		AlternatePort();
		AlternatePort(PinPort pin);
	byte ReadInput();//读取输入电平
};
