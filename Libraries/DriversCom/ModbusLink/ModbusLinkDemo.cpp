#include "ModbusLink.h"
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
uint16_t RegInputu16[144]; //输入寄存器
uint16_t RegHoilding16[60];

void ModbusSlaveLinkRoutin(void* param)
{
	if (modbusSlave.CheckFrame())
	{
		modbusSlave.DealFrame();
		modbusSlave.rxFrame.RemoveOneFrame();
		debug_printf("rx:%d-%d tx:%d-%d\n", modbusSlave.rxFrame.Cnt, modbusSlave.com.RxCnt, modbusSlave.txFrame.Cnt, modbusSlave.com.TxCnt);
		debug_printf("rxid:%d-id:%d\n", modbusSlave.id, modbusSlave.rxFrame.devid);
	}
	else if ((modbusSlave.com.RxSize() > 0 || (modbusSlave.rxFrame.dataLength > 0)) && (Sys.Ms() - modbusSlave.com.LastRcvTime) > 5)
	{
		debug_printf("清除接收缓冲区坏数据 length:%d\n", modbusSlave.rxFrame.dataLength);
		modbusSlave.rxFrame.dataLength = 0;
		modbusSlave.com.ClearRxBuf();
	}
	else {}
}

void ModbusSlaveLinkTestInit()
{
	p485dr.Set(PC2);
	p485dr.Invert = false;
	p485dr.OpenDrain = false;
	p485dr.Open();
	p485dr = 0;//接收模式
	modbusSlave.com.RS485 = &p485dr;
	modbusSlave.id = 1;
	modbusSlave.SetRegInput(0, ArrayLength(RegInputu16), RegInputu16, 0);
	modbusSlave.SetRegHoid(0, ArrayLength(RegHoilding16), 0);

	RegInputu16[0] = 12;
	RegInputu16[1] = 13;

	Sys.AddTask(ModbusSlaveLinkRoutin, 0, 0, 1, "ModbusSlaveLinkRoutin");
}
#endif
