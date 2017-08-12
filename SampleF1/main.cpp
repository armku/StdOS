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


const byte vers[] = "yyyy-MM-dd HH:mm:ss";
#if 1
    OutputPort led1(PB0,false);
    OutputPort led2(PF7,false);
    OutputPort led3(PF8,false);
#else 
    OutputPort led1(PD0,false);
    OutputPort led2(PD1,false);
    OutputPort led3(PD2,false);
	OutputPort ledLCD(PD12,false);
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
	static int i=0;
	printf("\r\n%d: cnt:%d",i++,time6cnt);
}

#define namee "StdOS"
void AT24C02Test();
void TimeTest();
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
	TimeTest();
	Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
	Sys.AddTask(TimerTask, &led1, 0, 1000, "TimerTask");
	
    Sys.Start();	
}
 
    Delegate < Timer & > abc;
    void tim2refesh(void *param)
    {
        time6cnt++;
    }
    Timer *timer2;
    void TimeTest()
    {
        // 初始化为输出
        timer2 = new Timer(Timer6);
        abc.Bind(tim2refesh);
        timer2->Register(abc);
        timer2->Open();
        //	timer2->SetFrequency(1);
        //	timer2->Config();
    }
