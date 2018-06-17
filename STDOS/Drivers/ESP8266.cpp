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
void ESP8266::SetPin(Pin prst, Pin pch)
{
	this->pRst.Set(prst);
	this->pRst.Invert = false;
	this->pRst.OpenDrain = false;

	this->pCH.Set(pch);
	this->pCH.Invert = false;
	this->pCH.OpenDrain = false;

	this->pRst.Open();
	this->pCH.Open();

	this->pRst = 1;
	this->pCH = 0;
}
void ESP8266::Rst()
{
#if 0
	this->Cmd("AT+RST", "OK", "ready", 2500);

#else 
	this->pRst = 0;
	Sys.Sleep(500);
	this->pRst = 1;
#endif 
}
