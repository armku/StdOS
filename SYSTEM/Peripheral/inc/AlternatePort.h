#pragma once

#include "OutputPort.h"

class AlternatePort:public OutputPort
{
	public:
		AlternatePort();
		AlternatePort(Pin pin);
	byte ReadInput();//��ȡ�����ƽ
};
