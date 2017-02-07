#pragma once

#include "stm32f10x.h"
#include "bsp_date.h"
#include "Type.h"

/* ������������������Ļ�,PC13�ͻ����Ƶ��ΪRTC Clock/64��ʱ�� */
//#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */

class StmRtc
{
    public:
        void RTC_NVIC_Config(void);
        void RTC_Configuration(void); 
        void RTC_CheckAndConfig(struct rtc_time *tm);
		void SetTime(uint seconds);//����ʱ��
};
