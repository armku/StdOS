#pragma once

#include "BasePort.h"

//回调函数定义 down指示按下还是弹起触发
typedef void(*BtnInCallback)(PinPort pin,bool down); 

class InputPort:protected BasePort
{
	public:
		InputPort(PinPort pin,byte invert=2);
		void Init(PinPort pin,byte invert=2);
		byte Read();//读取端口电平
		void Open();	
		void UsePress();//启用事件处理
	public:
		BtnInCallback callBack;//回掉函数
		uint PressTime;//弹起世间
	private:
		bool binvert;//端口反转
};
