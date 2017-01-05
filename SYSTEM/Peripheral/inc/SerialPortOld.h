#pragma once

#include "Sys.h"
#include "FIFORing.h"

class SerialPortOld
{
	public:
		SerialPortOld(COM_Def comPort,uint bound=115200);
		COM_Def ComPort;//端口号
		
		
};
extern uint com1timeidle; //串口1空闲时间

extern CFIFORing com1buf;//串口1接收缓冲区
