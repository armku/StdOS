#pragma once

#include "stm32f10x.h"
#include "bsp_date.h"

/* 如果定义了下面这个宏的话,PC13就会输出频率为RTC Clock/64的时钟 */
//#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */

void RTC_NVIC_Config(void);
void RTC_Configuration(void);
void Time_Adjust(struct rtc_time *tm);
static uint8_t USART_Scanf(uint32_t value);
void RTC_CheckAndConfig(struct rtc_time *tm);
