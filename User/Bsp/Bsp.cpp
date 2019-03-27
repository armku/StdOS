#include "OnChip\Port.h"
#include "OnChip/USART.h"
#include "Sys.h"
#include "Buffer.h"

char *AppVersion = "0.1.2018.1115"; //版本号

Port led1;

void LedTask(void *param)
{
	Port* leds = (Port*)param;
	*leds = !* leds;
}

USART usart111(USART1, 115200);

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

void EspDemoLinkTestInit();
void ModbusSlaveLinkTestInit();
void BspInit()
{
	led1.Set(PB0);
	led1.SetPinMode(GPIO_Out_PP);
	
	led1.Open();

	led1 = 1;
	
	Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");

	/*EspDemoLinkTestInit();*/
	ModbusSlaveLinkTestInit();
	usart111.OnReceive = Com1test;
}

/*
			LED1	LED2	LED3	LED4
STM32F0		PC6		PC7		PC8		PC9
STM32F1		PB0		PF7		PF8
STM32F4正点 PF9		PF10
F4安富莱F4-V5 PI10	PF7		PC2		PC2

*/
