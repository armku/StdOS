#include "BspPlatform\BspPlatform.h"
#include "Bsp.h"
#include "Device\Port.h"

#ifdef STM32F0
OutputPort led1(PC6, true);
OutputPort led2(PC7, true);
OutputPort led3(PC8, true);
OutputPort led4(PC9, true);

OutputPort ledss[] =
{
	led1, led2, led3, led4
};

void LedTask(void *param)
{
#ifdef STM32F0
	static uint8_t flag = 1;
	OutputPort *leds = (OutputPort*)param;

	flag <<= 1;
	flag &= 0x0f;
	if (flag == 0)
	{
		flag = 1;
	}

	leds[0] = flag & 0x01;
	leds[1] = flag & 0x02;
	leds[2] = flag & 0x04;
	leds[3] = flag & 0x08;

	//    led2 = key0;
#endif // STM32F0
}
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

void LedTask(void *param)
{
	auto leds = (OutputPort*)param;
	*leds = !* leds;
	//    led2 = key0;
}
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
void LedTask(void *param)
{
	OutputPort *leds = (OutputPort*)param;
	*leds = !* leds;
	led2 = !led2;
	//    led3 = !led3;
	//    led4 = !led4;
	//led2 = keyup;
}
#endif // STM32F0
//uint32_t OnUsart1Read(ITransport *transport, Buffer &bs, void *para, void *para2)
//{
//	bs.Show(true);
//	//transport->Write(bs);
//	return 0;
//}
void Esp8266Test();
void ssd1309Test();
void BspInit()
{	
//	Esp8266Test();
//	ssd1309Test();
//	SerialPort::GetMessagePort()->Register(OnUsart1Read);

	Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
}
