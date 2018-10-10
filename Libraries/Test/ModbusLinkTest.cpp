#include "App/ModbusLink.h"
#include "Sys.h"
#include "BspPlatform/Interrupt.h"
#include "Port.h"


#define _MODBUSLINKEST_CPP
#ifdef _MODBUSLINKEST_CPP

USART usart222(USART2, 115200);
ModbusSlaveLink modbusSlave(usart222);
OutputPort p485dr(PC2);
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
			p485dr = 1;//接收模式
			modbusSlave.rxFrame.RemoveOneFrame();
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
	p485dr.Open();
	p485dr = 1;//接收模式
	Sys.AddTask(ModbusSlaveLinkRoutin,0,0,1,"ModbusSlaveLinkRoutin");
}

#endif
