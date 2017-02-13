#include <stdio.h>
#include "SerialPort.h"
#include "W24xxx.h"
#include "multi_button.h"
#include "InputPort.h"
#include "WatchDog.h"
#include "CString.h"
#include "string.h"
#include "DateTime.h"
#include "List.h"
#include "FIFORing.h"
#include "Pwm.h"

// N = 2^32/365/24/60/60 = 136 年

WatchDog dog(3000);
void feeddog(void *param)
{
    dog.Feed();
} 

OutputPort led1(PB0, true);
OutputPort led2(PF7, true);
OutputPort led3(PF8, true);
void ledflash(void *param)
{
    //	led1=!led1;
    //	led2=!led2;
    led3 = !led3;
}

//按键 PC13 PA0
InputPortOld exti(PC13); //PA1 PB3     PA0 PC13
InputPort exti1(PA0);
void OnKeyPress(Pin pin, bool onoff)
{
    //led1.Write(onoff);
    led2 = !led2;
    printf("中断引脚：%d 值%d \n", pin, onoff);
}

/*
ISO-V2:PB5控制485方向
ISO-V3:PC2控制485方向
*/
OutputPort rs485(PC2);
static uint OnUsartRead(ITransport *transport, Buffer &bs, void *para)
{	
    SerialPort *sp = (SerialPort*)para;
    debug_printf("%s 收到：[%d]", sp->Name, bs.Length());
    bs.Show(true);
    bs.Show(false);
    String str = "Hello master";
    sp->SendBuffer(str.GetBuffer());
		
    return 0;
}
//SerialPort spp3(COM3);

int main(void)
{
    Sys.MessagePort = COM3;
    Sys.Init();
    Sys.ShowInfo();
   
    //sp2.RS485 = &rs485;
    rs485 = 1;
    
    exti.InitOld();
    exti.On();
    exti.RegisterOld(OnKeyPress);
   
    PWM pwm1(PC9);
    pwm1.Init();
    pwm1.SetOutPercent(50);
	
//	spp3.Open();
//	spp3.Register(OnUsartRead);	
//	String str="Hello work";
//	spp3.SendBuffer(str.GetBuffer());
	
	SerialPort::GetMessagePort()->Register(OnUsartRead);
	
    Sys.AddTask(feeddog, 0, 0, 1000, "看门狗"); //看门狗-喂狗
    Sys.AddTask(ledflash, 0, 5, 500, "状态指示灯"); 

    Sys.Start();
}
