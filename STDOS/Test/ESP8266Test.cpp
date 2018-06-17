#include "Drivers\ESP8266.h"
#include "Device\DeviceConfigHelper.h"

#define _ESP8266_TEST_CPP

#ifdef _ESP8266_TEST_CPP

void com3rcv();
void Esp8266TestInit()
{
	DeviceConfigCenter::PRcvCOM3 = com3rcv;
	DeviceConfigCenter::ConfigCom(COM3, 115200);
}
#endif
