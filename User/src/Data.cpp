#include "Type.h"
#include "stm32f10x.h"
#include "usart.hpp"
#include "Led.h"
#include "W24xxx.h"
#include "usmart.h"
#include "usart.h"
#include "W25qxx.h"
#include "Data.h"
#include "multi_button.h"
#include "Exti.h"
#include "TSys.h"

CFIFORing com1buf; //串口1接收缓冲区
uint com1timeidle; //串口1空闲时间
//sequence 序列号 cur_seq max_seq step

void ledflash();
void eepread();
//1ms软件定时器
void softTimers();
CLed led1(PB0);
CLed led2(PB13);
//按键 PC13 PA0

CExti exti(PA0);//PA1 PB3

//系统初始化
void STDInit() 
{    
	Sys.Init();
    uart_init(115200); //串口初始化为115200
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE );
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//关闭jtag，保留swd	
    usmart_dev.init(SystemCoreClock / 1000000); //初始化USMART

	exti.Init();
	exti.On();    
	
	Sys.ShowInfo();
	
	Sys.AddTask(softTimers,0,1,1);//1毫秒周期循环
	Sys.AddTask(ledflash,0,5,50);
	Sys.AddTask(eepread,0,10,10000);
	Sys.Start();
}

void ledflash()
{
    led1.Toggle();
    led2.Toggle();
}

extern ushort exti0;
void eepread()
{
	static byte cnt=0;
	printf("%3d中断次数:%d",cnt++,exti0);
	printf("\n");
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
