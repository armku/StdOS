#ifndef __RTC_H__
#define __RTC_H__

#include "Kernel\Sys.h"

// 实时时钟
class HardRTC
{
public:
	bool LowPower;	// 是否使用低功耗休眠。默认true
	bool External;	// 是否使用外部时钟。默认false
	bool Opened;
	byte Reversed;

	HardRTC();

	void Init();
	void LoadTime();
	void SaveTime();
	int Sleep(int ms);

	uint ReadBackup(byte addr);
	void WriteBackup(byte addr, uint value);
	
	static HardRTC* Instance();
	static void Start(bool lowpower = true, bool external = false);
};

#endif
#pragma once

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
