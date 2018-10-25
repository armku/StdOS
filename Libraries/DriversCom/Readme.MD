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


#include "DriversCom/DemoLink.h"
#include "Sys.h"
#include "USART.h"
#include "BspPlatform/Interrupt.h"

//#define _DEMOLINKTEST_H
#ifdef _DEMOLINKTEST_H

USART usart222(USART2, 115200);
DemoLink demolink(usart222);
/*
|header - fn- datalen- databuf- chksum|
|AA    -  00 -03 -     XX- XX- XX- XX|
|AA  00 00 AA
电脑发送：AA 00 00 AA
下位机响应 AA 00 03 41 42 43 73
*/

void DemoLinkRoutin(void * param)
{
	if (demolink.CheckFrame())
	{
		debug_printf("Rcv a frame\n");
		demolink.txFrame.data[0] = 0x41;
		demolink.txFrame.data[1] = 0x42;
		demolink.txFrame.data[2] = 0x43;
		demolink.txFrame.fnCode = 0x00;
		demolink.txFrame.isUpdated = true;

		demolink.Send();
	}
}

void DemoLinkRoutinS(void * param)
{
	demolink.txFrame.data[0] = 0x41;
	demolink.txFrame.data[1] = 0x42;
	demolink.txFrame.data[2] = 0x43;
	demolink.txFrame.fnCode = 0x00;
	//demolink.txFrame.dataLength = 0;
	demolink.txFrame.isUpdated = true;
	
	demolink.Send();
}

void DemoLinkTestInit()
{
	Sys.AddTask(DemoLinkRoutin,0,0,1,"DemoLinkRoutin");
	//Sys.AddTask(DemoLinkRoutinS, 0, 0, 2000, "DemoLinkRoutin");
}

#endif // _DEMOLINKTEST_H


