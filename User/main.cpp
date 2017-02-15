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
	
	byte buf[4];
	buf[0]=0x04;
	buf[1]=0x01;
	buf[2]=0x00;
	buf[3]=0x05;
    sp->SendBuffer(buf,4);	
    return 0;
}
SerialPort sp2(COM2);

int main(void)
{
    Sys.MessagePort = COM1;
    Sys.Init();
    Sys.ShowInfo();
   
    sp2.RS485 = &rs485;
    rs485 = 0;
	sp2.Register(OnUsartRead);
	sp2.Open();
    
    exti.InitOld();
    exti.On();
    exti.RegisterOld(OnKeyPress);
   
    PWM pwm1(PC9);
    pwm1.Init();
    pwm1.SetOutPercent(50);
	
	SerialPort::GetMessagePort()->Register(OnUsartRead);
	
    Sys.AddTask(feeddog, 0, 0, 1000, "WatchDog"); 
    Sys.AddTask(ledflash, 0, 5, 500, "LedTask"); 

    Sys.Start();
}
