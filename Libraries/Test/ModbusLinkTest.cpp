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
	if (modbusSlave.CheckFrame())
	{
		switch (modbusSlave.rxFrame.fnCode)
		{
		case 3:
			modbusSlave.txFrame.devid = modbusSlave.rxFrame.devid;
			modbusSlave.txFrame.fnCode = modbusSlave.rxFrame.fnCode;
			modbusSlave.txFrame.regAddr = modbusSlave.rxFrame.regAddr;
			modbusSlave.txFrame.regLength = modbusSlave.rxFrame.regLength;
			modbusSlave.txFrame.SetReg(0, 10);
			modbusSlave.txFrame.SetReg(1, 20);
			modbusSlave.txFrame.SetRegLen(10);
			modbusSlave.txFrame.isUpdated = true;
			modbusSlave.Send();
			modbusSlave.rxFrame.RemoveOneFrame();
			break;
		default:
			break;
		}
		debug_printf("frame ok\n");
	}
	else
	{
		debug_printf("frame error\n");
	}
}

void ModbusSlaveLinkTestInit()
{
	Sys.AddTask(ModbusSlaveLinkRoutin,0,0,1000,"ModbusSlaveLinkRoutin");
}

#endif
