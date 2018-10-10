#include "App/ModbusLink.h"
#include "Sys.h"
#include "BspPlatform/Interrupt.h"


#define _MODBUSLINKEST_CPP
#ifdef _MODBUSLINKEST_CPP

USART usart222(USART2, 115200);
uint8_t buf485[20];

void ModbusSlaveLinkRoutin(void* param)
{
	static int i = 0;
	debug_printf("hello:%d\n", i++);
	if (pCOM2->RxSize() > 0)
	{
		int len = pCOM2->RxSize();
		pCOM2->GetBytes(buf485, len);
		//pCOM2->ClearRxBuf();
		pCOM2->SendBytes(buf485, len);
		debug_printf("rcb one frame\n");
	}
}

void ModbusSlaveLinkTestInit()
{
	Sys.AddTask(ModbusSlaveLinkRoutin,0,0,1000,"ModbusSlaveLinkRoutin");
}

#endif
