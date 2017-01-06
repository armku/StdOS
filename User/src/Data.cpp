#include <stdio.h>
#include "Type.h"
#include "stm32f10x.h"
#include "SerialPortOld.h"
#include "W24xxx.h"
#include "W25qxx.h"
#include "Data.h"
#include "multi_button.h"
#include "InputPortOld.h"
#include "Sys.h"
#include "WatchDog.h"
#include "Port.h"

#define debug_printf printf


//sequence ���к� cur_seq max_seq step

void ledflash();
void eepread();
//1ms�����ʱ��
void softTimers();
OutputPort led1(PB0,true);
OutputPort led2(PF7,true);
OutputPort led3(PF8,true);
CW24xxx  w24c02(PB6,PB7,AT24C02);
//���� PC13 PA0

InputPortOld exti(PC13);//PA1 PB3
 
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
	led2=!led2;
	printf("�ж����ţ�%d ֵ%d \n",pin,onoff);
}
static uint OnUsartRead(ITransport* transport,Buffer& bs,void* para)
{
	SerialPortOld* sp =(SerialPortOld*)para;
	debug_printf("%s �յ���",sp->Name);
	bs.Show(true);		
	sp->SendBuffer(bs.GetBuffer());
	
	return 0;
}
SerialPortOld sp1(COM1);
SerialPortOld sp2(COM2);
SerialPortOld sp3(COM3);
/*
KEY PA0 
*/
//ϵͳ��ʼ��
void ComTimers();
void STDInit() 
{    		
	Sys.MessagePort=COM1;
	Sys.Init();
	Sys.ShowInfo();
	sp1.Register(OnUsartRead,&sp1);
	sp2.Register(OnUsartRead,&sp2);
	sp3.Register(OnUsartRead,&sp3);
	sp1.Open();
	sp2.Open();
	sp3.Open();
	
	
    
	exti.Init();
	exti.On();	
	exti.Register(OnKeyPress);
		
	//w24c02.Test();
		
	Sys.AddTask(softTimers,0,1,1,"1���������ʱ��");//1��������ѭ��
	Sys.AddTask(ComTimers,0,1,1,"�������ݽ��ն�ʱ��");//1��������ѭ��
	Sys.AddTask(feeddog, 0, 0, 10, "���Ź�"); //���Ź�-ι��
	Sys.AddTask(ledflash,0,5,50,"״ָ̬ʾ��");
	Sys.AddTask(eepread,0,10,1000,"��������");
	Sys.Start();
}

void ledflash()
{
	//led1=!led1;
	//led2=!led2;
	led3=!led3;	
}

void eepread()
{
	//sp1.SendBuffer("COM1\n");
	sp2.SendBuffer("COM2\n");
	sp3.SendBuffer("COM3\n");
}

//1ms�����ʱ��
void softTimers()
{
    
}
byte USART_RX_BUF[100]; //���ջ���,���USART_REC_LEN���ֽ�.
extern uint com1timeidle; //����1����ʱ��
extern CFIFORing com1buf; //����1���ջ�����

extern uint com2timeidle; //����2����ʱ��
extern CFIFORing com2buf; //����2���ջ�����

extern uint com3timeidle; //����3����ʱ��
extern CFIFORing com3buf; //����3���ջ�����
//���ڽ���ͨ�Ŷ�ʱ��
void ComTimers()
{
	com1timeidle++;
	com2timeidle++;
	com3timeidle++;

    if (com1timeidle > 3)
    {
        com1timeidle = 0;
        ushort len = com1buf.GetLength();
        if (len >= 1)
        {
            com1buf.Pop(USART_RX_BUF, 0, len);
			sp1.OnUsartReceive(USART_RX_BUF,len);
        }
        com1buf.Reset();		
    }
	
	if (com2timeidle > 1)
    {
        com2timeidle = 0;
        ushort len = com2buf.GetLength();
        if (len >= 3)
        {
            com2buf.Pop(USART_RX_BUF, 0, len);
			sp2.OnUsartReceive(USART_RX_BUF,len);
        }
        com2buf.Reset();		
    }
	
	if (com3timeidle > 1)
    {
        com3timeidle = 0;
        ushort len = com3buf.GetLength();
        if (len >= 1)
        {
            com3buf.Pop(USART_RX_BUF, 0, len);
			sp3.OnUsartReceive(USART_RX_BUF,len);
        }
        com3buf.Reset();		
    }
}
