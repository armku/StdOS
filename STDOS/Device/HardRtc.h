#pragma once

#include "Type.h"
#include "DateTime.h"

class HardRtc
{
    public:  
		void Init();
		void SetTime(uint seconds);//����ʱ��
		void SetTime(DateTime & dt);//����ʱ��-����ʱ���ʽ
		DateTime& GetTime(DateTime & dt);//��ȡʱ��	
		static HardRtc * Instance();//����ʵ��
		bool LowPower;
		bool External;
		void Start(bool ,bool);
	private:
		void RTC_NVIC_Config(void);
        void RTC_Configuration(void);
};
