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
#include "Exti.h"

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
//���� PC13 PA0

CExti exti(PA0);//PA1 PB3

//ϵͳ��ʼ��
void STDInit() 
{
    delay_init();
    uart_init(115200); //���ڳ�ʼ��Ϊ115200
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE );
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//�ر�jtag������swd	
    usmart_dev.init(SystemCoreClock / 1000000); //��ʼ��USMART

	exti.Init();
	exti.On();
    printf("System init\n");
   	    
    Scheduling.ThreadAdd(softTimers, 1); //1��������ѭ��
    Scheduling.ThreadAdd(ledflash, 50);
    Scheduling.ThreadAdd(eepread, 1000);
}

void ledflash()
{
    led1.Toggle();
    led2.Toggle();
}

extern uint16_t exti0;
void eepread()
{
	static uint8_t cnt=0;
	printf("%3d�жϴ���:%d",cnt++,exti0);
	printf("\n");
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
