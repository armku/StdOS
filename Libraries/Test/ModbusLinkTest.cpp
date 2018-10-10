#include "App/ModbusLink.h"
#include "Sys.h"
#include "BspPlatform/Interrupt.h"


#define _MODBUSLINKEST_CPP
#ifdef _MODBUSLINKEST_CPP

USART usart222(USART2, 115200);
uint8_t buf485[20];
ModbusSlaveLink modbusSlave(usart222);

void ModbusSlaveLinkRoutin(void* param)
{
	static int i = 0;
	debug_printf("hello:%d\n", i++);
	modbusSlave.CheckFrame();
}

void ModbusSlaveLinkTestInit()
{
	Sys.AddTask(ModbusSlaveLinkRoutin,0,0,1000,"ModbusSlaveLinkRoutin");
}

#endif
