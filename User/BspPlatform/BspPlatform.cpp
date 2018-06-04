#include "BspPlatform.h"
#if defined STM32F0
#elif defined STM32F1
#include "stm32f10x.h"
#elif defined STM32F4
#endif
#include "Sys.h"
#include "Core\Queue.h"
#include "Port.h"
#include "Platform\stm32.h"
#include "Device\DeviceConfigHelper.h"

void TimeUpdate();
void BspPlatformInit()
{	
	DeviceConfigCenter::ConfigCom(COM1, 256000);
	DeviceConfigCenter::ConfigCom(COM3, 256000);
	DeviceConfigCenter::TimeTickInit();//系统用定时器初始化
	DeviceConfigCenter::PTim2Update = TimeUpdate;
}
int DeviceConfigCenter::CurrentTick()
{
#if defined STM32F0

#elif defined STM32F1
	return (TIM2->CNT) >> 1;
#elif defined STM32F4

#endif
}
uint32_t DeviceConfigCenter::CurrentTicks1()
{
#if defined STM32F0

#elif defined STM32F1
	return SysTick->LOAD - SysTick->VAL;
#elif defined STM32F4

#endif
}
#include <stdio.h>
#include "stdarg.h"
/////////////////////////////////////////////////////////////////////////////
int StdPrintf(const char *format, ...)
{
	static char sprint_buf[1024];
	va_list args;

	int n;
	va_start(args, format);
	n = vsprintf(sprint_buf, format, args);
	va_end(args);

	DeviceConfigCenter::comSend(COM1, Buffer(sprint_buf, n));
	return n;
}

