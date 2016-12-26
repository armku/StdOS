#pragma once

#include "Port.h"

class OutputPort:public CPort
{
	public:
		OutputPort(PinPort pin,bool  invert=false);//端口号，是否反转电平
		void Write(const bool onoff);//true：高电平，false：低电平
		void Init(PinPort pin,bool  invert=false);//端口号，是否反转电平
		void operator = (const bool value);//赋值操作运算符
	private:
		bool invertPort;//端口反转
};
