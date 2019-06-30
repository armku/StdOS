#include "OnChip\Port.h"
#include "OnChip/USART.h"
#include "Sys.h"
#include "Buffer.h"
#include "Core/RingBuffer.h"

char *AppVersion = "0.1.2018.1115"; //版本号

Port led1;
void ad71248Test();
void LedTask(void *param)
{
	//static bool flagonoff = false;
	//flagonoff = !flagonoff;
	Port* leds = (Port*)param;
	*leds = !* leds;
	//*leds = flagonoff;
}

USARTHAL usart111(COM1,115200);

//串口测试开始
#ifdef __cplusplus
extern "C" {
#endif

#include "../HAL/STM32F1/ARCH_UART.h"
uint8_t   loop_bufcom1[64] = { 0 };                             //定义环形缓冲区
RingBuffer ringRcvcom1(loop_bufcom1,ArrayLength(loop_bufcom1));
//向环形缓冲区【写】一字节数据
static void write_loop_buf(uint8_t dat)
{
	ringRcvcom1.Put(dat);
}

#ifdef __cplusplus
}
#endif

void Com1ReadTest(void* param)
{
	static int tmcnt = 0;//时间定时器
	static int lenold = 0;//历史接收缓冲区长度
	int readlen;//读取的缓冲区长度
	char buf[100];
	if (ringRcvcom1.length != 0)
	{
		if (lenold != ringRcvcom1.length)
		{
			//数据一直更新
			lenold = ringRcvcom1.length;
			tmcnt=0;
			return;
		}
		if (++tmcnt >= 5)
		{
			tmcnt = 0;
			//50ms没有新数据认为完整一帧数据到来
			readlen = ringRcvcom1.Get(buf, ArrayLength(buf));
			UART1_send_data((uint8_t*)buf, readlen);
		}		
	}
}

void com1test()
{
	Sys.AddTask(Com1ReadTest, 0, 0, 10, "Com1ReadTest");
	UART_1_rcv_IRQ = write_loop_buf;
	//usart111.OnReceive = Com1test;
}

//串口测试结束

void Esp8266TestInit();
void ModbusSlaveLinkTestInit();
void w25qxxxtest();
void BspInit()
{
	led1.SetPin(PB0);
	led1.pinMode(GPIO_Out_PP);

	led1 = 1;
	
	Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
	
	com1test();

	/*Esp8266TestInit();*/
	ModbusSlaveLinkTestInit();
}

/*
			LED1	LED2	LED3	LED4
STM32F0		PC6		PC7		PC8		PC9
STM32F1		PB0		PF7		PF8
STM32F4正点 PF9		PF10
F4安富莱F4-V5 PI10	PF7		PC2		PC2
*/
