#include "../Esp8266.h"
#include "Sys.h"
#include "BspPlatform/Interrupt.h"
#include "Port.h"
#include "Task.h"

#define _ESP8266EST_CPP
#ifdef _ESP8266EST_CPP

USARTHAL usart222(COM3,115200);

void checkComRoutin(void* param)
{

}

void Esp8266Routin(void* param)
{

}

void Esp8266TestInit()
{
	Sys.AddTask(checkComRoutin, 0, 0, 10, "EspCOmCHK");
	Sys.AddTask(Esp8266Routin, 0, 0, 100, "EspRoutin");
}
#endif
