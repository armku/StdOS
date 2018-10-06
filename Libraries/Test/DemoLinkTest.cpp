#include "App\DemoLink.h"
#include "Sys.h"
#include "USART.h"
#include "BspPlatform\Interrupt.h"

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
