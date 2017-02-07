#pragma once

#include "Type.h"
#include "DateTime.h"

class STM32Rtc
{
    public:  
		void Init();
		void SetTime(uint seconds);//设置时间
		void SetTime(DateTime & dt);//设置时间-北京时间格式
		DateTime& GetTime(DateTime & dt);//读取时间		
	private:
		void RTC_NVIC_Config(void);
        void RTC_Configuration(void);
};
