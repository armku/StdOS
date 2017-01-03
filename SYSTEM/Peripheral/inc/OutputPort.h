#pragma once

#include "Port.h"

class OutputPort:protected Port
{
	public:
		OutputPort(Pin pin,byte invert=2);
		void Init(Pin pin,byte invert=2);
		void Write(bool onoff);//输出开关
		void Open();
		void Toggle(); //翻转引脚
	private:
		bool binvert;//端口反转
};
