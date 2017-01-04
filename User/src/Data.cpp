#include "Type.h"
#include "stm32f10x.h"
#include "usart.hpp"
#include "W24xxx.h"
#include "usmart.h"
#include "usart.h"
#include "W25qxx.h"
#include "Data.h"
#include "multi_button.h"
#include "InputPort.h"
#include "Sys.h"
#include "OutputPort.h"
#include "WatchDog.h"

CFIFORing com1buf; //串口1接收缓冲区
uint com1timeidle; //串口1空闲时间
//sequence 序列号 cur_seq max_seq step

void ledflash();
void eepread();
//1ms软件定时器
void softTimers();
OutputPort led1(PB0,true);
OutputPort led2(PF7,true);
OutputPort led3(PF8,true);
Port TestPort(PB0);
//按键 PC13 PA0

InputPort exti(PC13);//PA1 PB3
WatchDog dog(3000);
void feeddog()
{
	dog.Feed();
}

void OnKeyPress(Pin pin, bool onoff)
{
	if(onoff)
	{
		led1.Write(true);
	}
	else		
	{
		led1.Write(false);
	}	
	led2.Toggle();
	printf("中断引脚：%d 值%d \n",pin,onoff);
}
/*
KEY PA0 
*/
//系统初始化
void STDInit() 
{    	
	Sys.Init();
    uart_init(115200);
    usmart_dev.init(SystemCoreClock / 1000000); //初始化USMART

	exti.Init();
	exti.On();	
	exti.Register(OnKeyPress);
	
	led1.Write(false);
	led2.Write(false);
	led3.Write(false);
	Sys.ShowInfo();	
		
	Sys.AddTask(softTimers,0,1,1,"1毫秒软件定时器");//1毫秒周期循环
	Sys.AddTask(feeddog, 0, 0, 10, "看门狗"); //看门狗-喂狗
	Sys.AddTask(ledflash,0,5,50,"状态指示灯");
	Sys.AddTask(eepread,0,10,1000,"测试任务");
	Sys.Start();
}

void ledflash()
{
//	led1.Toggle();
//	led2.Toggle();
	led3.Toggle();	
		
	TestPort=true;
	//TestPort=false;
}

void eepread()
{
	
}

//1ms软件定时器
void softTimers()
{
    com1timeidle++;

    if (com1timeidle > 3)
    {
        com1timeidle = 0;
        ushort len = com1buf.GetLength();
        if (len >= 3)
        {
            USART_RX_STA = len - 2;
            USART_RX_STA |= 0x8000;

            com1buf.Pop(USART_RX_BUF, 0, len - 2);
        }
        com1buf.Reset();
    }
}
