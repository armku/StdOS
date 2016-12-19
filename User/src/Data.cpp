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

CTaskScheduler Scheduling; //����


CFIFORing com1buf; //����1���ջ�����
uint32_t com1timeidle; //����1����ʱ��
//sequence ���к� cur_seq max_seq step

void ledflash();
void eepread();
//1ms�����ʱ��
void softTimers();
CLed led1(PB0);
CLed led2(PB13);


CPort key1(PA0);
CPort key2(PC13);

void BTN2_SINGLE_CLICK_Handler(void *btn)
{
    //do something...
    printf("����2һ�ΰ���\n");
}
void BTN2_DOUBLE_CLICK_Handler(void *btn)
{
    //do something...
    printf("����22�ΰ���\n");
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
//            printf("����1������\n");
//        }
//        else if (btn1_event_val == PRESS_UP)
//        {
//            //do something
//        }
//        else if (btn1_event_val == LONG_PRESS_HOLD)
//        {
//            printf("������\n");
//            //do something
//        }
//    }
//    if (btn1_event_val == LONG_PRESS_HOLD)
//    {
//        printf("������1\n");
//    }

//    if (btn2_event_val != btn2.get_button_event())
//    {
//        btn2_event_val = btn2.get_button_event();
//        if (btn2_event_val == PRESS_DOWN)
//        {
//            printf("����2������\n");
//        }
//        if (btn2_event_val == PRESS_UP)
//        {
//            printf("����2���ɿ�\n");
//        }
//    }
	if (btn2_event_val == LONG_PRESS_HOLD)
    {
        printf("������2\n");
    }
}

void btn2ticks();
//ϵͳ��ʼ��
void STDInit() 
{
    delay_init();
    uart_init(115200); //���ڳ�ʼ��Ϊ115200
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE );
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//�ر�jtag������swd	
    usmart_dev.init(SystemCoreClock / 1000000); //��ʼ��USMART

    printf("System init\n");
   
	btn2.attach(SINGLE_CLICK, BTN2_SINGLE_CLICK_Handler);
	btn2.attach(DOUBLE_CLICK, BTN2_DOUBLE_CLICK_Handler);
    
    Scheduling.ThreadAdd(softTimers, 1); //1��������ѭ��
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

//1ms�����ʱ��
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
