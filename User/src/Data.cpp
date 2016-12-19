#include <stdint.h>
#include "stm32f10x.h"
#include "delay.h"
#include "usart.hpp"
#include "Led.h"
#include "W24xxx.h"
#include "Scheduling.h"
#include "usmart.h"
#include "usart.h"
#include "W25qxx.h"
#include "Data.h"
#include "multi_button.h"

CTaskScheduler Scheduling; //调度


CFIFORing com1buf; //串口1接收缓冲区
uint32_t com1timeidle; //串口1空闲时间
//sequence 序列号 cur_seq max_seq step

void ledflash();
void eepread();
//1ms软件定时器
void softTimers();
CLed led1(PB0);
CLed led2(PB13);


CPort key1(PA0);
CPort key2(PC13);

void BTN2_SINGLE_CLICK_Handler(void *btn)
{
    //do something...
    printf("按键2一次按键\n");
}
void BTN2_DOUBLE_CLICK_Handler(void *btn)
{
    //do something...
    printf("按键22次按键\n");
}
CButton btn2(PC13);
void keyapp()
{
//    static uint8_t btn1_event_val;
	static uint8_t btn2_event_val;
	
	//return;
//    if (btn1_event_val != get_button_event(&btn1))
//    {
//        btn1_event_val = get_button_event(&btn1);

//        if (btn1_event_val == PRESS_DOWN)
//        {
//            //do something
//            printf("按键1被按下\n");
//        }
//        else if (btn1_event_val == PRESS_UP)
//        {
//            //do something
//        }
//        else if (btn1_event_val == LONG_PRESS_HOLD)
//        {
//            printf("长按键\n");
//            //do something
//        }
//    }
//    if (btn1_event_val == LONG_PRESS_HOLD)
//    {
//        printf("长按键1\n");
//    }

//    if (btn2_event_val != btn2.get_button_event())
//    {
//        btn2_event_val = btn2.get_button_event();
//        if (btn2_event_val == PRESS_DOWN)
//        {
//            printf("按键2被按下\n");
//        }
//        if (btn2_event_val == PRESS_UP)
//        {
//            printf("按键2被松开\n");
//        }
//    }
	if (btn2_event_val == LONG_PRESS_HOLD)
    {
        printf("长按键2\n");
    }
}

void btn2ticks();
//系统初始化
void STDInit() 
{
    delay_init();
    uart_init(115200); //串口初始化为115200
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE );
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//关闭jtag，保留swd	
    usmart_dev.init(SystemCoreClock / 1000000); //初始化USMART

    printf("System init\n");
   
	btn2.attach(SINGLE_CLICK, BTN2_SINGLE_CLICK_Handler);
	btn2.attach(DOUBLE_CLICK, BTN2_DOUBLE_CLICK_Handler);
    
    Scheduling.ThreadAdd(softTimers, 1); //1毫秒周期循环
    Scheduling.ThreadAdd(ledflash, 50);
    Scheduling.ThreadAdd(eepread, 100);
    Scheduling.ThreadAdd(btn2ticks, 5);
    Scheduling.ThreadAdd(keyapp, 500);
}

void ledflash()
{
    led1.Toggle();
    led2.Toggle();
}

void btn2ticks()
{
    btn2.ticks();
}

void eepread()
{
    //    uint8_t buf[200];
    //at24c02.ReadBytes(buf, 0, 100);
}

//1ms软件定时器
void softTimers()
{
    com1timeidle++;

    if (com1timeidle > 3)
    {
        com1timeidle = 0;
        uint16_t len = com1buf.GetLength();
        if (len >= 3)
        {
            USART_RX_STA = len - 2;
            USART_RX_STA |= 0x8000;

            com1buf.Pop(USART_RX_BUF, 0, len - 2);
        }
        com1buf.Reset();
    }
}
