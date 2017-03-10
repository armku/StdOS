#pragma once

#include "Type.h"
#include "DateTime.h"

class HardRtc
{
    public:  
		void Init();
		void SetTime(uint seconds);//设置时间
		void SetTime(DateTime & dt);//设置时间-北京时间格式
		DateTime& GetTime(DateTime & dt);//读取时间	
		static HardRtc * Instance();//生成实例
		bool LowPower;
		bool External;
		void Start(bool ,bool);
	private:
		void RTC_NVIC_Config(void);
        void RTC_Configuration(void);
};
