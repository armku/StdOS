#ifndef __WatchDog_H__
#define __WatchDog_H__

#include <stdint.h>

// ���Ź�
class WatchDog
{
private:
	WatchDog();

public:
	uint32_t Timeout; // ��ǰ��ʱʱ��

	bool Config(uint32_t ms);	// ���ÿ��Ź�ι������ʱ�䣬������ʱ�佫����MCU
	void ConfigMax();		// ���Ź��޷��رգ�ֻ������һ�����ֵ
	void Feed(); // ι��

	static WatchDog& Current();
	static void FeedDogTask(void* param);
	// �򿪿��Ź����ι��ʱ��26208ms��Ĭ��20ms
	static void Start(uint32_t msTimeout = 5000, uint32_t msFeed = 20);
};

#endif
