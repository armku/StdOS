#include "Drivers\ESP8266.h"
#include "Device\DeviceConfigHelper.h"

#define _ESP8266_TEST_CPP

#ifdef _ESP8266_TEST_CPP
ESP8266 esp;

void Esp8266TestInit()
{
	

	esp.Init();
}
#endif
