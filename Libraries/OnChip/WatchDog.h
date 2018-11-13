#ifndef __WatchDog_H__
#define __WatchDog_H__

#include "Core\Type.h"

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
	// �򿪿��Ź����ι��ʱ��26208ms��Ĭ��2000ms
	static void Start(uint32_t msTimeout = 2000, uint32_t msFeed = 100);
};

#endif
