#include <stdio.h>
#include "SerialPort.h"
#include "Drivers\AT24CXX.h"
#include "Drivers\W25QXXX.h"
#include "Drivers\Button.h"
#include "SString.h"
#include "string.h"
#include "List.h"
#include "Spi.h"
#include "Flash.h"
#include "Drivers\lcd_dr.h"
#include "TInterrupt.h"
#include "Timer.h"
#include "TTime.h"

class TTTTime
{
	public:
		uint	Seconds;		// 全局秒数，系统启动后总秒数。累加
		UInt64	Milliseconds;	// 全局毫秒数，系统启动后总毫秒（1000ms整部分）。累加
		 uint CurrentTicks;	// 当前滴答时钟
		UInt64 Current; 		// 当前毫秒数
};
TTTTime now123;
void Test12(void* param)
{
	now123.Seconds=Time.Seconds;
	now123.Milliseconds=Time.Milliseconds;
	now123.Current=Time.Current();
	now123.CurrentTicks=Time.CurrentTicks();
}
const byte vers[] = "yyyy-MM-dd HH:mm:ss";
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
    OutputPort *leds = (OutputPort*)param;
    *leds = ! * leds;
    //    led2 = key0;
}

uint OnUsart1Read(ITransport *transport, Buffer &bs, void *para, void *para2)
{
    bs.Show(true);
    return 0;
}

uint time6cnt;
void TimerTask(void *param)
{
    static int i = 0;
    //printf("\r\n%d: cnt:%d", i++, time6cnt);
}

#define namee "StdOS"
void AT24C02Test();


int main(void)
{
    Sys.Name = (char*)namee;
    byte aa = vers[0];
    aa = aa;

    Sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 
    SerialPort::GetMessagePort()->Register(OnUsart1Read);
    //	AT24C02Test();    
    Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
    Sys.AddTask(TimerTask, &led1, 0, 1000, "TimerTask");
	Sys.AddTask(Test12,0,600,1000,"Test");

    Sys.Start();
}
