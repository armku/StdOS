#include "App/ModbusLink.h"
#include "Sys.h"


#define _MODBUSLINKEST_CPP
#ifdef _MODBUSLINKEST_CPP

void ModbusSlaveLinkRoutin(void* param)
{
	static int i = 0;
	debug_printf("hello:%d\n", i++);
}

void ModbusSlaveLinkTestInit()
{
	Sys.AddTask(ModbusSlaveLinkRoutin,0,0,1000,"ModbusSlaveLinkRoutin");
}

#endif
