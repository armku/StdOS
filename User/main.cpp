#include <stdio.h>
#include "Type.h"
#include "stm32f10x.h"
#include "SerialPortOld.h"
#include "W24xxx.h"
#include "W25qxx.h"
#include "multi_button.h"
#include "InputPortOld.h"
#include "Sys.h"
#include "WatchDog.h"
#include "Port.h"
#include "Buffer.h"
#include "CString.h"

void ledflash();
OutputPort led1(PB0, true);
OutputPort led2(PF7, true);
OutputPort led3(PF8, true);
//按键 PC13 PA0

InputPortOld exti(PC13); //PA1 PB3

WatchDog dog(3000);
void feeddog()
{
    dog.Feed();
}

void OnKeyPress(Pin pin, bool onoff)
{
    //led1.Write(onoff);
    led2 = !led2;
    printf("中断引脚：%d 值%d \n", pin, onoff);
}

static uint OnUsartRead(ITransport *transport, Buffer &bs, void *para)
{
    SerialPortOld *sp = (SerialPortOld*)para;
    debug_printf("%s 收到：[%d]", sp->Name, bs.Length());
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
//系统初始化
void ComTimers();
void assert_failed(uint8_t* file, uint32_t line);
void STDInit()
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

    exti.InitOld();
    exti.On();
    exti.RegisterOld(OnKeyPress);

    Sys.AddTask(ComTimers, 0, 1, 1, "串口数据接收定时器"); //1毫秒周期循环
    Sys.AddTask(feeddog, 0, 0, 10, "看门狗"); //看门狗-喂狗
    Sys.AddTask(ledflash, 0, 5, 50, "状态指示灯");
    
	
	
	
	//	char cs[]="this is a buffer test";
    //	Buffer bs(cs,sizeof(cs));
    //	debug_printf("Buffer bs(cs,strlen(cs))=>%s \r\n",cs);

    //	byte buf[]={'1','2','3','4'};
    //	byte bts[]={'5','6','7','8','9','0'};
    //	Buffer bs(buf,sizeof(buf));
    //	bs.Show(false);
    //	bs=bts;
    //	bs.Show(false);

    byte buf[] = 
    {
        1, 2, 3, 4
    };
    byte bts[] = 
    {
        5, 6, 7
    };

    Buffer bs(buf, sizeof(buf));
    Buffer bs2(bts, sizeof(bts));

    bs.Show(true);
    bs = bs2;
    bs.Show(true);
    bs2 = bs;
    bs2.SetLength(bs.Length());
    bs2 = bs;
    bs2.Show(true);
	Buffer bs3=bs2.Sub(0,2);
	bs3.Show(true);
	debug_printf("测试String\n");
	#if 1
	char * errstr="发现错误";
	String str;		
	assert(str.Length()==1,errstr);
	assert(str.Capacity() == 0X40 - 1, errstr);
	
	String str1("456");
	debug_printf("str1 length = %d",str1.Length());
	str1.Show(true);
	assert(str1=="456",errstr);
	assert(str1.GetBuffer()=="456",errstr);
	
	String str2(str1);
	assert(str1==str2,errstr);
	#endif
	
	
	
	
	
	
	Sys.Start();	
}

void ledflash()
{
    //	led1=!led1;
    //	led2=!led2;
    led3 = !led3;
}

byte USART_RX_BUF[100]; //接收缓冲,最大USART_REC_LEN个字节.
extern uint com1timeidle; //串口1空闲时间
extern CFIFORing com1buf; //串口1接收缓冲区

extern uint com2timeidle; //串口2空闲时间
extern CFIFORing com2buf; //串口2接收缓冲区

extern uint com3timeidle; //串口3空闲时间
extern CFIFORing com3buf; //串口3接收缓冲区
//串口接收通信定时器
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


int main(void)
{
    STDInit();

    while (1){}
}
