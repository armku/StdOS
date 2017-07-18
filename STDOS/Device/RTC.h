#ifndef __RTC_H__
#define __RTC_H__

#include "Kernel\Sys.h"

// ʵʱʱ��
class HardRTC
{
public:
	bool LowPower;	// �Ƿ�ʹ�õ͹������ߡ�Ĭ��true
	bool External;	// �Ƿ�ʹ���ⲿʱ�ӡ�Ĭ��false
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
