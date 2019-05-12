#include "OnChip\Port.h"
#include "OnChip/USART.h"
#include "Sys.h"
#include "Buffer.h"

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

USART usart111(COM1,115200);

void Com1test(void *param)
{
	uint8_t buf[20];
	int len = 0;
	len = usart111.RxSize();
	if (usart111.GetBytes(buf, len))
	{
		Buffer(buf, len).Show(true);
	}
	else
	{
		debug_printf("com1 idle error\n");
	}
}

//串口测试开始
#ifdef __cplusplus
extern "C" {
#endif

#include "../HAL/STM32F1/ARCH_UART.h"
static uint8_t   loop_buf[64] = { 0 };                             //定义环形缓冲区
static volatile uint32_t loop_wp = 0;                                    //定义环形缓冲区写指针
static volatile uint32_t loop_rp = 0;
//向环形缓冲区【写】一字节数据
static void write_loop_buf(uint8_t dat)
{
	uint32_t next_wp;

	next_wp = (loop_wp + 1) & 63;

	if (next_wp == loop_rp)
	{
		return;
	}
	loop_buf[loop_wp] = dat;
	loop_wp = next_wp;

	UART_1_send_byte(dat);
}

#ifdef __cplusplus
}
#endif

void Com1ReadTest(void* param)
{
	
}

void com1test()
{
	Sys.AddTask(Com1ReadTest, 0, 0, 10, "Com1ReadTest");
	UART_1_rcv_IRQ = write_loop_buf;
	//usart111.OnReceive = Com1test;
}



//串口测试结束

void EspDemoLinkTestInit();
void ModbusSlaveLinkTestInit();
void w25qxxxtest();
void BspInit()
{
	led1.SetPin(PB0);
	led1.pinMode(GPIO_Out_PP);

	led1 = 1;
	
	Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
	
	com1test();

	/*EspDemoLinkTestInit();*/
	ModbusSlaveLinkTestInit();
	
	//w25qxxxtest();
	//ad71248Test();
}

/*
			LED1	LED2	LED3	LED4
STM32F0		PC6		PC7		PC8		PC9
STM32F1		PB0		PF7		PF8
STM32F4正点 PF9		PF10
F4安富莱F4-V5 PI10	PF7		PC2		PC2

*/
