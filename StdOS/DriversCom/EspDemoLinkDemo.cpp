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

}

void EspDemoLinkTestInit()
{
	esp.SetPin(PG13, PG14);


	Sys.AddTask(EspDemoLinkRoutin, 0, 0, 1000, "EspLinkRoutin");
}
#endif
