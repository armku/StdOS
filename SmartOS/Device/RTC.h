﻿#ifndef __RTC_H__
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
