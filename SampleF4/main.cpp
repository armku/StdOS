#include "SerialPort.h"
#include "Timer.h"
#include "Spi.h"

OutputPort led1(PF9,false);
OutputPort led2(PF10,false);

void LedTask(void *param)
{
    OutputPort *leds = (OutputPort*)param;
    *leds = ! * leds;	
    //led2 = keyup;
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
void w25q128test();
int mainarmfly(void);
int main(void)
{
	mainarmfly();
	Sys.Init();
	#if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 		
	SerialPort::GetMessagePort()->Register(OnUsart1Read);
	
	Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
	Sys.AddTask(TimerTask, &led1, 0, 1000, "TimerTask");
	InterruptTest();
	w25q128test();
    Sys.Start();
}
