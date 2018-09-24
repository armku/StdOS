#include "Device\Configuration.h"
#include "Sys.h"

//#define _SERIALPORT485TEST_CPP
#ifdef _SERIALPORT485TEST_CPP

uint8_t buf485[20];

void SerialPort485Task12(void *param)
{
	DeviceConfigHelper::comSend(COM1, Buffer(buf485, ArrayLength(buf485)));
}
uint8_t bufrcv485[100];
void R485Rcv()
{
	Buffer bsrcv(bufrcv485, ArrayLength(bufrcv485));
	Rxx1.Read(bsrcv);
	DeviceConfigHelper::com1send(bsrcv);
}

OutputPort r485(PC0,false);

void SerialPort485TestInit()
{
	r485 = 0;//Ω” ’
	DeviceConfigHelper::pCOM1Rx485 = &r485;
	DeviceConfigHelper::PRcvCOM1 = R485Rcv;
	DeviceConfigHelper::ConfigCom(COM1, 115200);
	DeviceConfigHelper::ComChgBaudRate(COM1, 115200);

	Sys.AddTask(SerialPort485Task12, 0, 0, 500, "R485Task");
}
#endif
