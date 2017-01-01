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

CFIFORing com1buf; //����1���ջ�����
uint com1timeidle; //����1����ʱ��
//sequence ���к� cur_seq max_seq step

void ledflash();
void eepread();
//1ms�����ʱ��
void softTimers();
OutputPort led1(PB0,true);
OutputPort led2(PF7,true);
OutputPort led3(PF8,true);
//���� PC13 PA0

CExti exti(PC13);//PA1 PB3

void OnKeyPress(PinPort pin, bool onoff)
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
	printf("�ж����ţ�%d ֵ%d \n",pin,onoff);
}
/*
KEY PA0 
*/
//ϵͳ��ʼ��
void STDInit() 
{    	
	Sys.Init();
    uart_init(115200);
    usmart_dev.init(SystemCoreClock / 1000000); //��ʼ��USMART

	exti.Init();
	exti.On();	
	exti.Register(OnKeyPress);
	
	led1.Write(false);
	led2.Write(false);
	led3.Write(false);
	Sys.ShowInfo();
		
	Sys.AddTask(softTimers,0,1,1,"1���������ʱ��");//1��������ѭ��
	Sys.AddTask(ledflash,0,5,50,"״ָ̬ʾ��");
	Sys.AddTask(eepread,0,10,1000,"��������");
	Sys.Start();
}

void ledflash()
{
//	led1.Toggle();
//	led2.Toggle();
	led3.Toggle();	
}

void eepread()
{
	
}

//1ms�����ʱ��
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
