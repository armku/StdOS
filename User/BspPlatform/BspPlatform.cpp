#include "BspPlatform.h"
#include "Sys.h"
#include "Port.h"
#include "Platform\stm32.h"
#include "OnChip\Configuration.h"
#include "USART.h"
#include "BspPlatform/Interrupt.h"
#include "../HAL/STM32F1/ARCH_UART.h"

void TimeUpdate();


void BspPlatformInit()
{	
	DeviceConfigCenter::TimeTickInit();//系统用定时器初始化
	DeviceConfigCenter::PTim2Update = TimeUpdate;
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
extern "C"
{
static char sprint_buf[64];
WEAK int SmartOS_printf(const char *format, ...)
{	
	va_list args;

	int n;
	va_start(args, format);
	n = vsprintf(sprint_buf, format, args);
	va_end(args);
	
	UART1_send_data((uint8_t*)sprint_buf,n);
	return n;
}
}
