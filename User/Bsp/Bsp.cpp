#include "BspPlatform\BspPlatform.h"
#include "Bsp.h"
#include "OnChip\Port.h"
#include "OnChip\Configuration.h"
#include "Buffer.h"

#ifdef STM32F0
OutputPort led1(PC6, true);
OutputPort led2(PC7, true);
OutputPort led3(PC8, true);
OutputPort led4(PC9, true);

OutputPort ledss[] =
{
	led1, led2, led3, led4
};
#elif defined STM32F1
#if 1
OutputPort led1(PB0, false);
OutputPort led2(PF7, false);
OutputPort led3(PF8, false);
#else 
OutputPort led1(PD0, false);
OutputPort led2(PD1, false);
OutputPort led3(PD2, false);
OutputPort ledLCD(PD12, false);
#endif 
#elif defined STM32F4
#if 1
//正点原子开发板F4
OutputPort led1(PF9, false);
OutputPort led2(PF10, false);
#else 
//安福莱开发板F4-V5
OutputPort led1(PI10, false);
OutputPort led2(PF7, false);
OutputPort led3(PC2, false);
OutputPort led4(PC2, false);
#endif 
#endif

void LedTask(void *param)
{
#ifdef STM32F0
	static uint8_t flag = 1;
	auto leds = (OutputPort*)param;

	flag <<= 1;
	flag &= 0x0f;
	if (flag == 0)
	{
		flag = 1;
	}

	led1 = !led1;
	led2 = !led2;
	led3 = !led3;
	led4 = !led4;
#elif defined STM32F1
	auto leds = (OutputPort*)param;
	*leds = !* leds;
	//    led2 = key0;
#elif defined STM32F4
	OutputPort *leds = (OutputPort*)param;
	*leds = !* leds;
	//led2 = !led2;
	//    led3 = !led3;
	//    led4 = !led4;
	//led2 = keyup;
#endif 
}

uint8_t chbuf[256];

void com1rcv()
{
	int len= Rxx1.Read(chbuf, ArrayLength(chbuf));
	Rxx1.Clear();

	debug_printf("COM1RCV:\n");
	Buffer(chbuf,len).ShowHex(true);
}
void comtestrcv()
{
	int len= Rxx3.Read(chbuf, ArrayLength(chbuf));
	
	debug_printf("COM3RCV:\n");
	Buffer(chbuf,len).ShowHex(true);
}
#include "OnChip/USART.h"

USART usart111(USART1, 256000);
const char *hello = "hello world\n";
void routsendtest(void * param)
{
	//DeviceConfigCenter::com1send((void*)hello,ArrayLength(hello));
	usart111.SendBytes((uint8_t*)hello, ArrayLength(hello));
}
void Esp8266TestInit();
void W5500Test();

void BspInit()
{
	led1 = 1;
	led2 = 1;
	Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
	DeviceConfigCenter::PRcvCOM1 = com1rcv;
	DeviceConfigCenter::PRcvCOM3 = comtestrcv;
	DeviceConfigCenter::ConfigCom(COM3, 256000);

	usart111.Initialize();

	Sys.AddTask(routsendtest, 0, 0, 1000, "routsendtest");

	//W5500Test();
}
