#include "App\DemoLink.h"
#include "Sys.h"
#include "USART.h"

#define _DEMOLINKTEST_H
#ifdef _DEMOLINKTEST_H

USART usart222(USART2, 115200);
DemoLink demolink(usart222);

void DemoLinkRoutin(void * param)
{
	if (demolink.CheckFrame())
	{
		debug_printf("Rcv a frame\n");
	}
}

void DemoLinkTestInit()
{
	Sys.AddTask(DemoLinkRoutin,0,0,1,"DemoLinkRoutin");
}

#endif // _DEMOLINKTEST_H
