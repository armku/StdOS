#include <stdio.h>
#include "SerialPort.h"
#include "Drivers\AT24CXX.h"
#include "Drivers\Button.h"
#include "Port.h"
#include "WatchDog.h"
#include "SString.h"
#include "string.h"
#include "DateTime.h"
#include "List.h"
#include "Pwm.h"
//#include "Modbus\Modbus1.h"
#include "Spi.h"
#include "RTC.h"
#include "Flash.h"
#include "Drivers\lcd_dr.h"
#include "TInterrupt.h"

const byte vers[]="yyyy-MM-dd HH:mm:ss";
#if 1
OutputPort led1(PB0);
OutputPort led2(PF7);
OutputPort led3(PF8);
#else
OutputPort led1(PD0);
OutputPort led2(PD1);
OutputPort led3(PD2);
#endif

void LedTask(void *param)
{
    OutputPort *leds = (OutputPort*)param;
    *leds = ! * leds;
//    led2 = key0;
}
//#define namee "StdOS"
//PWM ledLCD(PD12);

int main(void)
{
    TSys &sys = (TSys &)(Sys);
    //    #if 0
    //        sys.Codec = codec;
    //	#endif

//    sys.Name = (char*)namee;
	byte aa=vers[0];
	aa=aa;

    sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
        WatchDog::Start(20000, 10000);
    #else 
        WatchDog::Start();
    #endif 
    //    #if 0
    //        //flash 最后一块作为配置区
    //        Config::Current = &Config::CreateFlash();
    //    #endif  
	
    Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
	
    Sys.Start();
}
