#include "../Bsp/Porting.h"
#include "Sys.h"
#include "../component/lib/Buffer.h"

char *AppVersion = "0.1.2018.1115"; //版本号

mcuGpio led1;// (GPIOB, GPIO_Pin_0);

void LedTask(void *param)
{
	led1 = !led1;
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

void MqttLinkTestInit();
void ModbusMasterLinkTestInit();
void EspDemoLinkTestInit();
void BspInit()
{
	led1.SetPin(PB0);
	led1.mode(PIN_MODE::OUTPUT_PP);
	
	Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");

	//usart111.OnReceive = Com1test;
	//MqttLinkTestInit();
	//ModbusMasterLinkTestInit();
	EspDemoLinkTestInit();
}

/*
			LED1	LED2	LED3	LED4
STM32F0		PC6		PC7		PC8		PC9
STM32F1		PB0		PF7		PF8
STM32F4正点 PF9		PF10
F4安富莱F4-V5 PI10	PF7		PC2		PC2

*/
