#pragma once

#include "OutputPort.h"

class AlternatePort:public OutputPort
{
	public:
	byte ReadInput();//读取输入电平
};
