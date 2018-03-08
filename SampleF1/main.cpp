#include "SerialPort.h"
#include "Bsp\Bsp.h"

#if 1
OutputPort led1(PB0, false);
OutputPort led2(PF7, false);
OutputPort led3(PF8, false);
#else 
OutputPort led1(PD0, false);
OutputPort led2(PD1, false);
OutputPort led3(PD2, false);
OutputPort ledLCD(PD12, false);
#endif 

void LedTask(void *param)
{
	auto leds = (OutputPort*)param;
	*leds = !* leds;
	//    led2 = key0;
}

uint OnUsart1Read(ITransport *transport, Buffer &bs, void *para, void *para2)
{
	//transport->Write(bs);
	bs.Show(true);
	return 0;
}
int main(void)
{
	Sys.Init();
#if DEBUG
	Sys.MessagePort = COM1;
	Sys.ShowInfo();
#endif 	
	BspInit();

	SerialPort::GetMessagePort()->Register(OnUsart1Read);

	Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
	Sys.Start();
}
