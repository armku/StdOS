#include "OnChip\Port.h"
#include "OnChip/USART.h"
#include "Sys.h"
#include "Buffer.h"
#include "Core/RingBuffer.h"

char *AppVersion = "0.1.2018.1115"; //�汾��

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

//���ڲ��Կ�ʼ
#ifdef __cplusplus
extern "C" {
#endif

#include "../HAL/STM32F1/ARCH_UART.h"
uint8_t   loop_bufcom1[64] = { 0 };                             //���廷�λ�����
RingBuffer ringRcvcom1(loop_bufcom1,ArrayLength(loop_bufcom1));
//���λ�������д��һ�ֽ�����
static void write_loop_buf(uint8_t dat)
{
	ringRcvcom1.Put(dat);
}

#ifdef __cplusplus
}
#endif

void Com1ReadTest(void* param)
{
	static int tmcnt = 0;//ʱ�䶨ʱ��
	static int lenold = 0;//��ʷ���ջ���������
	int readlen;//��ȡ�Ļ���������
	char buf[100];
	if (ringRcvcom1.length != 0)
	{
		if (lenold != ringRcvcom1.length)
		{
			//����һֱ����
			lenold = ringRcvcom1.length;
			tmcnt=0;
			return;
		}
		if (++tmcnt >= 5)
		{
			tmcnt = 0;
			//50msû����������Ϊ����һ֡���ݵ���
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

//���ڲ��Խ���

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
STM32F4���� PF9		PF10
F4������F4-V5 PI10	PF7		PC2		PC2
*/
