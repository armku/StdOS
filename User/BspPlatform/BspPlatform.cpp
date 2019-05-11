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
static char sprint_buf[64];
int StdPrintf(const char *format, ...)
{	
	va_list args;

	int n;
	va_start(args, format);
	n = vsprintf(sprint_buf, format, args);
	va_end(args);
	
#if 0
	pCOM1->SendBytes((uint8_t*)sprint_buf, n);
#else
#if 1
	UART1_send_data((uint8_t*)sprint_buf,n);
#else

	for (int i = 0; i <n; i++)
	{
		/* 发送一个字节数据到USART */
		USART_SendData(USART1, sprint_buf[i]);

		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
#endif
#endif
	return n;
}
