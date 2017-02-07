#pragma once

#include "Type.h"
#include "DateTime.h"

class STM32Rtc
{
    public:  
		void Init();
		void SetTime(uint seconds);//����ʱ��
		void SetTime(DateTime & dt);//����ʱ��-����ʱ���ʽ
		DateTime& GetTime(DateTime & dt);//��ȡʱ��		
	private:
		void RTC_NVIC_Config(void);
        void RTC_Configuration(void);
};
