#include <stdio.h>
#include "Type.h"
#include "stm32f10x.h"
#include "SerialPort.h"
#include "W24xxx.h"
#include "W25qxx.h"
#include "multi_button.h"
#include "InputPort.h"
#include "Sys.h"
#include "WatchDog.h"
#include "Port.h"
#include "Buffer.h"
#include "CString.h"
#include "string.h"
#include "DateTime.h"
#include "TimeCost.h"
#include "List.h"
#include "FIFORing.h"

WatchDog dog(3000);
void feeddog(void * param)
{
    dog.Feed();
}


/*
KEY PA0 
 */
OutputPort led1(PB0, true);
OutputPort led2(PF7, true);
OutputPort led3(PF8, true);
void ledflash(void * param)
{
    //	led1=!led1;
    //	led2=!led2;
    led3 = !led3;
}


//���� PC13 PA0
InputPortOld exti(PC13); //PA1 PB3     PA0 PC13
InputPort exti1(PA0);
void OnKeyPress(Pin pin, bool onoff)
{
    //led1.Write(onoff);
    led2 = !led2;
    printf("�ж����ţ�%d ֵ%d \n", pin, onoff);
}

SerialPortOld sp1(COM1);
SerialPortOld sp2(COM2);
SerialPortOld sp3(COM3);
byte USART_RX_BUF[100]; //���ջ���,���USART_REC_LEN���ֽ�.
extern uint com1timeidle; //����1����ʱ��
extern CFIFORing com1buf; //����1���ջ�����
extern uint com2timeidle; //����2����ʱ��
extern CFIFORing com2buf; //����2���ջ�����
extern uint com3timeidle; //����3����ʱ��
extern CFIFORing com3buf; //����3���ջ�����
//���ڽ���ͨ�Ŷ�ʱ��
void ComTimers(void * param)
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
            sp1.OnUsartReceive(USART_RX_BUF, len);
        }
        com1buf.Reset();
    }

    if (com2timeidle > 3)
    {
        com2timeidle = 0;
        ushort len = com2buf.GetLength();
        if (len >= 1)
        {
            com2buf.Pop(USART_RX_BUF, 0, len);
            sp2.OnUsartReceive(USART_RX_BUF, len);
        }
        com2buf.Reset();
    }

    if (com3timeidle > 3)
    {
        com3timeidle = 0;
        ushort len = com3buf.GetLength();
        if (len >= 1)
        {
            com3buf.Pop(USART_RX_BUF, 0, len);
            sp3.OnUsartReceive(USART_RX_BUF, len);
        }
        com3buf.Reset();
    }
}
static uint OnUsartRead(ITransport *transport, Buffer &bs, void *para)
{
    SerialPortOld *sp = (SerialPortOld*)para;
    debug_printf("%s �յ���[%d]", sp->Name, bs.Length());
    bs.Show(true);
    sp->SendBuffer(bs.GetBuffer());

    return 0;
}

int main(void)
{
    Sys.MessagePort = COM1;
    Sys.Init();
    Sys.ShowInfo();
    sp1.Register(OnUsartRead, &sp1);
    sp2.Register(OnUsartRead, &sp2);
    sp3.Register(OnUsartRead, &sp3);
    sp1.Open();
    sp2.Open();
    sp3.Open();

    TimeCost tc;
    exti.InitOld();
    exti.On();
    exti.RegisterOld(OnKeyPress);
    tc.Show();
	    	
    Sys.AddTask(ComTimers, 0, 1, 1, "�������ݽ��ն�ʱ��"); //1��������ѭ��
    Sys.AddTask(feeddog, 0, 0, 1000, "���Ź�"); //���Ź�-ι��
    Sys.AddTask(ledflash, 0, 5, 500, "״ָ̬ʾ��");    
			
    Sys.Start();
}
