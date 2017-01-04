#pragma once

#include "Port.h"

class OutputPort:public Port
{
	public:
		OutputPort(Pin pin,byte invert=2);
		void Init(Pin pin,byte invert=2);
		void Write(bool onoff);//输出开关
		void Open();		
	private:
		bool binvert;//端口反转
};
