#include "App/ModbusLink.h"
#include "Sys.h"
#include "BspPlatform/Interrupt.h"
#include "Port.h"
#include "Task.h"

#define _MODBUSLINKEST_CPP
#ifdef _MODBUSLINKEST_CPP

USART usart222(USART2, 115200);
ModbusSlaveLink modbusSlave(usart222);
OutputPort p485dr(PC2);
uint32_t taskrcvid = 1000;
void ModbusSlaveLinkRoutin(void* param)
{
	static int i = 0;

	/*if (modbusSlave.com.RxSize() > 0)
	{
	uint8_t ch = 0;
	modbusSlave.com.GetByte(ch);
	debug_printf("rcv:%02x\n",ch );
	}
	return;*/
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
			p485dr = 0;//发送模式
			modbusSlave.Send();
			debug_printf("task rcvid:%d\n", taskrcvid);
			if (Task::Get(taskrcvid) != NULL)
			{
				Task::Get(taskrcvid)->Set(true, 10);
			}
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


void ModbusSlaveLinkRoutinRcv(void * param)
{
	p485dr = 1;//接收模式
	debug_printf("into rcv mode\n");
	Task::Current().Set(false);
	taskrcvid = Task::Current().ID;
	debug_printf("cur task id:%d\n", taskrcvid);
}
void ModbusSlaveLinkTestInit()
{
	p485dr.Open();
	p485dr = 1;//接收模式
	Sys.AddTask(ModbusSlaveLinkRoutin, 0, 0, 1, "ModbusSlaveLinkRoutin");
	Sys.AddTask(ModbusSlaveLinkRoutinRcv, 0, 0, 1, "ModbusSlaveLinkRoutinRcv");
}

#endif
