#include "Sys.h"
#include "SerialPort.h"

#define _ESP8266_TEST_H
#ifdef _ESP8266_TEST_H

uint32_t OnUsart3Read(ITransport *transport, Buffer &bs, void *para, void *para2)
{
	bs.Show(true);
	//transport->Write(bs);
	return 0;
}
void sp3Routin(void * param)
{
	auto sp=(SerialPort*)param;
}

SerialPort sp3(COM3);
void Esp8266Test()
{
	sp3.Register(OnUsart3Read);
	sp3.SetBaudRate(115200);
	sp3.Open();
	
	Sys.AddTask(sp3Routin,&sp3,0,1000,"Esp8266Test");
}
#endif
