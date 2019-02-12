#include "BspPlatform.h"
#include "Sys.h"
#include "Port.h"
#include "../../stm32.h"
#include "Configuration.h"
#include "Interrupt.h"

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
	for (int i = 0; i <n; i++)
	{
		/* 发送一个字节数据到USART */
		USART_SendData(USART1, sprint_buf[i]);

		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
#endif
	return n;
}
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 0
#pragma import(__use_no_semihosting)             
#ifdef __cplusplus
#else
//标准库需要的支持函数                 
	struct __FILE
	{
		int handle;

	};

	FILE __stdout;

	//定义_sys_exit()以避免使用半主机模式    
	_sys_exit(int x)
	{
		x = x;
	}
#endif
	//重定义fputc函数 
	int fputc(int ch, FILE *f)
	{
		while ((USART1->SR & 0X40) == 0);//循环发送,直到发送完毕   
		USART1->DR = (u8)ch;
		return ch;
	}
#else 

	/*使用microLib的方法*/
	 
	int fputc(int ch, FILE *f)
	{
		USART_SendData(USART1, (uint8_t) ch);

		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}

		return ch;
	}
	int GetKey (void)  {

		while (!(USART1->SR & USART_FLAG_RXNE));

		return ((int)(USART1->DR & 0x1FF));
	}
	
#endif
#ifdef __cplusplus
}
#endif
