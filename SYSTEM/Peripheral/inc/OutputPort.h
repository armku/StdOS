#pragma once

#include "Type.h"
#include "BasePort.h"

class OutputPort:public BasePort
{
	public:
		OutputPort(PinPort pin,byte invert=2);
		void Init(PinPort pin,byte invert=2);
		void Write(bool onoff);//输出开关
		void Open();
	private:
		bool binvert;//端口反转
};
