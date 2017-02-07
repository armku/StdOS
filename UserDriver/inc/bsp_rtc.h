#pragma once

#include "stm32f10x.h"
#include "Type.h"

class StmRtc
{
    public:
        void RTC_NVIC_Config(void);
        void RTC_Configuration(void); 
        void RTC_CheckAndConfig();
		void SetTime(uint seconds);//…Ë÷√ ±º‰
};
