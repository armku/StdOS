#include "BspPlatform.h"
#include "Sys.h"
#include "Core\Queue.h"
#include "Port.h"
#include "Platform\stm32.h"
#include "Device\DeviceConfigHelper.h"

void TimeUpdate();

void OS_ComSendChk(void *param);
//串口接收判断
void OS_ComRcvChk(void *param);
void BspPlatformInit()
{	
	DeviceConfigCenter::ConfigCom(COM1, 256000);
	DeviceConfigCenter::ConfigCom(COM3, 256000);
	DeviceConfigCenter::TimeTickInit();//系统用定时器初始化
	DeviceConfigCenter::PTim2Update = TimeUpdate;

	Sys.AddTask(OS_ComSendChk, 0, 5, 1, "ComSendChk");
	Sys.AddTask(OS_ComRcvChk, 0, 100, 1, "ComRcvChk");
}

int DeviceConfigCenter::CurrentTick()
{
#if defined STM32F0
	return (TIM2->CNT) >> 1;
#elif defined STM32F1
	return (TIM2->CNT) >> 1;
#elif defined STM32F4
	return (TIM2->CNT) >> 1;
#endif
}
uint32_t DeviceConfigCenter::CurrentTicks1()
{
#if defined STM32F0
	return SysTick->LOAD - SysTick->VAL;
#elif defined STM32F1
	return SysTick->LOAD - SysTick->VAL;
#elif defined STM32F4
	return SysTick->LOAD - SysTick->VAL;
#endif
}
#include <stdio.h>
#include "stdarg.h"
/////////////////////////////////////////////////////////////////////////////
static char sprint_buf[1024];
int StdPrintf(const char *format, ...)
{	
	va_list args;

	int n;
	va_start(args, format);
	n = vsprintf(sprint_buf, format, args);
	va_end(args);

	DeviceConfigCenter::comSend(COM1, Buffer(sprint_buf, n));
	return n;
}
