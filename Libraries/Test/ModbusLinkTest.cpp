#include "DriversCom/ModbusLink.h"
#include "Sys.h"
#include "BspPlatform/Interrupt.h"
#include "Port.h"
#include "Task.h"

#define _MODBUSLINKEST_CPP
#ifdef _MODBUSLINKEST_CPP
//测试 01 03 00 00 00 0A C5 CD
USART usart222(USART2, 115200);
ModbusSlaveLink modbusSlave(usart222);
OutputPort p485dr;
void ModbusSlaveLinkRoutin(void* param)
{
	static int i = 0;
		
	if (modbusSlave.CheckFrame())
	{
		switch (modbusSlave.rxFrame.fnCode)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			modbusSlave.txFrame.devid = modbusSlave.rxFrame.devid;
			modbusSlave.txFrame.fnCode = modbusSlave.rxFrame.fnCode;
			modbusSlave.txFrame.regAddr = modbusSlave.rxFrame.regAddr;
			modbusSlave.txFrame.regLength = modbusSlave.rxFrame.regLength;
			for (int i = 0; i < 10; i++)
				modbusSlave.txFrame.SetReg(i, i);
			modbusSlave.txFrame.SetRegLen(10);
			modbusSlave.txFrame.isUpdated = true;
			modbusSlave.Send();
			break;
		default:
			break;
		}
		debug_printf("hello:%d\n", i++);
		debug_printf("frame ok\n");
	}
	else
	{
		//debug_printf("frame error\n");
	}
}

void ModbusSlaveLinkTestInit()
{
	p485dr.Set(PC2);
	p485dr.Invert=false;
	p485dr.OpenDrain=false;
	p485dr.Open();
	p485dr = 1;//接收模式
	modbusSlave.com.RS485 = &p485dr;
	Sys.AddTask(ModbusSlaveLinkRoutin, 0, 0, 1, "ModbusSlaveLinkRoutin");
}

#endif
