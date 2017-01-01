#pragma once

#include "BasePort.h"



class InputPort:protected BasePort
{
	public:
		InputPort(PinPort pin,byte invert=2);
		void Init(PinPort pin,byte invert=2);		
		void Open();	
		void UsePress();//启用事件处理
	public:
		//BtnInCallback callBack;//回掉函数
		uint PressTime;//弹起世间
	private:
		bool binvert;//端口反转
};
