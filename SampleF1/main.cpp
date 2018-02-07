#include "SerialPort.h"
#include "Drivers\AT24CXX.h"
#include "Drivers\W25QXXX.h"
#include "SString.h"
#include "string.h"
#include "List.h"
#include "Spi.h"
#include "Flash.h"
#include "App\lcd_dr.h"
#include "TInterrupt.h"
#include "Timer.h"
#include "TTime.h"
#include "Drivers\RX8025T.h"

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
    *leds = ! * leds;
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

	SerialPort::GetMessagePort()->Register(OnUsart1Read);
			
	Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
	Sys.Start();
}
