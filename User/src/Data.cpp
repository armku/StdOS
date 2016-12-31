#include "Type.h"
#include "stm32f10x.h"
#include "usart.hpp"
#include "W24xxx.h"
#include "usmart.h"
#include "usart.h"
#include "W25qxx.h"
#include "Data.h"
#include "multi_button.h"
#include "Exti.h"
#include "TSys.h"
#include "OutputPort.h"

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
//按键 PC13 PA0

CExti exti(PC13);//PA1 PB3
void EXTI_PC13_Config(void);

uint flagbtn;//按键
uint exticnt;//中断次数
/*
KEY PA0 
*/
//系统初始化
void STDInit() 
{    
	Sys.Init();
    uart_init(115200); //串口初始化为115200	
    usmart_dev.init(SystemCoreClock / 1000000); //初始化USMART

	exti.Init();
	exti.On();	
	EXTI_PC13_Config();	
	
	led1.Write(false);
	led2.Write(false);
	led3.Write(false);
	Sys.ShowInfo();
	
	Sys.AddTask(softTimers,0,1,1,"1毫秒软件定时器");//1毫秒周期循环
	Sys.AddTask(ledflash,0,5,50,"状态指示灯");
	Sys.AddTask(eepread,0,10,1000,"测试任务");
	Sys.Start();
}

void ledflash()
{
//	led1.Toggle();
//	led2.Toggle();
//	led3.Toggle();
	if(flagbtn)
	{
		led2.Write(true);
	}
	else
	{
		led2.Write(false);
	}
}

void eepread()
{
	static byte cnt=0;
	printf("%3d中断次数: %d",cnt++,exticnt);
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

 /**
  * @brief  配置 PC13 为线中断口，并设置中断优先级
  * @param  无
  * @retval 无
  */
void EXTI_PC13_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	/* EXTI line(PC13) mode config */  
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}
