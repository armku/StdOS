#pragma once

#include "OutputPort.h"

class AlternatePort:public OutputPort
{
	public:
		AlternatePort();
		AlternatePort(PinPort pin);
	byte ReadInput();//��ȡ�����ƽ
};
