#include "Port.h"
#include "stm32f0xx.h"

const byte vers[] = "yyyy-MM-dd HH:mm:ss";
OutputPort led1(PC6, true);
OutputPort led2(PC7, true);
OutputPort led3(PC8, true);
OutputPort led4(PC9, true);

OutputPort ledss[]={led1,led2,led3,led4};

void LedTask(void *param)
{
	OutputPort *leds = (OutputPort*)param;
		
	leds[0]=!leds[0];
	leds[1]=!leds[1];
	leds[2]=!leds[2];
	leds[3]=!leds[3];
	
    //    led2 = key0;
}

#define namee "StdOS"

void main()
{
	Sys.Name = (char*)namee;
    Sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 
	
	Sys.AddTask(LedTask, &ledss, 0, 500, "LedTask");

    Sys.Start();
}
