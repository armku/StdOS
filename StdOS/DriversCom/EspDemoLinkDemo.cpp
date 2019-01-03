#include "EspDemoLink.h"
#include <stdio.h>
#include "Sys.h"
#include "BspPlatform/Interrupt.h"
#include "Port.h"
#include "Task.h"


#define _ESPDEMOLINKTEST_CPP
#ifdef _ESPDEMOLINKTEST_CPP

USART usart333(USART3, 115200);
EspDemoLink esp(usart333);

void EspDemoLinkRoutin(void* param)
{
	static int step = 0;
	switch (step)
	{
	case 0:
		esp.Kick11();
		break;
	default:
		break;
	}
}

void EspDemoLinkTestInit()
{
	esp.SetPin(PG13, PG14);
	esp.Init();

	Sys.AddTask(EspDemoLinkRoutin, 0, 0, 1000, "EspLinkRoutin");
}
#endif
