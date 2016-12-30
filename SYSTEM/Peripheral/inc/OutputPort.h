#pragma once

#include "BasePort.h"

class OutputPort:protected BasePort
{
	public:
		OutputPort(PinPort pin,byte invert=2);
		void Init(PinPort pin,byte invert=2);
		void Write(bool onoff);//输出开关
		void Open();
		void Toggle(); //翻转引脚
	private:
		bool binvert;//端口反转
};
