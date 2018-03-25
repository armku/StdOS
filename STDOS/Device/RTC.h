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
	void SetTime(uint32_t seconds);//����ʱ��
	void SetTime(DateTime & dt);//����ʱ��-����ʱ���ʽ
	DateTime& GetTime(DateTime & dt);//��ȡʱ��	
};

#endif
