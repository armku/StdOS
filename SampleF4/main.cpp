#include "SerialPort.h"
#include "Timer.h"
#include "Spi.h"

OutputPort led1(PF9,false);
OutputPort led2(PF10,false);
//InputPort key0(PE4,true);
InputPort key1(PE3,true);
InputPort key2(PE2,true);
InputPort keyup(PA0,true,InputPort::DOWN);
SpiSoft spi128;

void LedTask(void *param)
{
    OutputPort *leds = (OutputPort*)param;
    *leds = ! * leds;	
    led2 = keyup;
}

uint OnUsart1Read(ITransport *transport, Buffer &bs, void *para, void *para2)
{
    bs.Show(true);
    return 0;
}
uint time6cnt;
void TimerTask(void *param)
{
//    static int i = 0;
//    printf("\r\n%d: cnt:%d", i++, time6cnt);
}
void InterruptTest();
int main(void)
{
	Sys.Init();
	#if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 		
	SerialPort::GetMessagePort()->Register(OnUsart1Read);
	
	Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
	Sys.AddTask(TimerTask, &led1, 0, 1000, "TimerTask");
	InterruptTest();
    Sys.Start();
}
