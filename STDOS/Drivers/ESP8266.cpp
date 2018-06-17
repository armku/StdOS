#include "ESP8266.h"
#include "Sys.h"
#include "Device\DeviceConfigHelper.h"

ESP8266::ESP8266()
{
}

uint8_t chbuf3[1000];
void com3rcv()
{
	Buffer bs1(chbuf3, ArrayLength(chbuf3));

	Rxx3.Read(bs1);

	debug_printf("COM1RCV:\n");
	bs1.ShowHex(true);
}
void ESP8266::Init()
{
	DeviceConfigCenter::PRcvCOM3 = com3rcv;
	DeviceConfigCenter::ConfigCom(COM3, 115200);
}
