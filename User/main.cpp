#include <stdio.h>
#include "SerialPort.h"
#include "Drivers\AT24CXX.h"
#include "Drivers\Button.h"
#include "SString.h"
#include "string.h"
#include "List.h"
#include "Spi.h"
#include "Flash.h"
#include "Drivers\lcd_dr.h"
#include "TInterrupt.h"

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

#define namee "StdOS"
CLcd_DR lcddr1(PD3, PD6, PD7, PB3, PB4);
int main(void)
{
    TSys &sys = (TSys &)(Sys);

    sys.Name = (char*)namee;
    byte aa = vers[0];
    aa = aa;

    sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 	
	lcddr1.Init();
	lcddr1.Test(0x55,0xaa);
	Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");

    Sys.Start();
}
