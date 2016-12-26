#pragma once

#include "Port.h"

typedef void(*BtnCallback)(void*); //回调函数定义

class CInputPort:public CPort
{
	public:
		CInputPort(PinPort pin);
		BtnCallback Press;//回掉函数
		bool Read();//读取状态
		void UsePress();//启用事件
		void Open();//打开触发器
		uint16_t PressTime;//按下的时长，ms
};
