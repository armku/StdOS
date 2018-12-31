#include "ModbusLink.h"
#include "Sys.h"
#include "BspPlatform/Interrupt.h"
#include "Port.h"
#include "Task.h"

#ifndef _MODBUSMASTERLINKEST_CPP
#define _MODBUSMASTERLINKEST_CPP

//USART usart222(USART2, 9600);
USART usart222(USART3, 115200);
OutputPort u485dr;
ModbusMasterLink modMaster(usart222);

void ModbusMasterDemoRoutin(void* param)
{
	modMaster.GetValue(0,0,0);
}


void ModbusMasterLinkTestInit()
{
	u485dr.Set(PC2);
	u485dr.OpenDrain = false;
	u485dr.Invert = false;
	u485dr.Open();
	u485dr = 0;//进入接收模式
	usart222.RS485 = &u485dr;

	Sys.AddTask(ModbusMasterDemoRoutin, 0, 0, 1000, "ModbusMaster");
}

#endif // !_MODBUSMASTERLINKEST_CPP
