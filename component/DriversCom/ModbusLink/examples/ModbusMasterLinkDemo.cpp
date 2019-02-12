#include "../src/ModbusLink.h"
#include "Sys.h"
#include "../../../../Bsp/Porting.h"
#include "Task.h"

//#define _MODBUSMASTERLINKEST_CPP
#ifdef _MODBUSMASTERLINKEST_CPP

//HardwareSerial0_T usart222(USART2, 9600);
HardwareSerial0_T usart222(USART3, 9600);
OutputPort u485dr;
ModbusMasterLink modMaster(usart222);
uint16_t RegInputu16[144]; //输入寄存器
uint16_t RegHoilding16[60];

void ModbusMasterDemoRoutin(void* param)
{
	modMaster.GetValueRegInput(1,0,4);
}


void ModbusMasterLinkTestInit()
{
	u485dr.Set(PC2);
	u485dr.OpenDrain = false;
	u485dr.Invert = false;
	u485dr.Open();
	u485dr = 0;//进入接收模式
	usart222.RS485 = &u485dr;

	modMaster.SetRegInput(0, ArrayLength(RegInputu16), RegInputu16, 0);
	modMaster.SetRegHoid(0, ArrayLength(RegHoilding16), RegHoilding16, 0);


	sys.AddTask(ModbusMasterDemoRoutin, 0, 0, 1000, "ModbusMaster");
}

#endif // !_MODBUSMASTERLINKEST_CPP
