#include "Drivers\ESP8266.h"
#include "Device\DeviceConfigHelper.h"

#define _ESP8266_TEST_CPP

#ifdef _ESP8266_TEST_CPP
Esp8266 esp;

void Esp8266TestInit()
{
	

	esp.Init();
}

void Delay_ms(int ms)
{
	Sys.Sleep(500);
}
#endif
