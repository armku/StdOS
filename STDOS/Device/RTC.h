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
	uint8_t Reversed;

	HardRTC();

	void Init();
	void LoadTime();
	void SaveTime();
	int Sleep(int ms);

	uint32_t ReadBackup(uint8_t addr);
	void WriteBackup(uint8_t addr, uint32_t value);
	
	static HardRTC* Instance();
	static void Start(bool lowpower = true, bool external = false);
public:
	void SetTime(uint32_t seconds);//设置时间
	void SetTime(DateTime & dt);//设置时间-北京时间格式
	DateTime& GetTime(DateTime & dt);//读取时间	
};

#endif
