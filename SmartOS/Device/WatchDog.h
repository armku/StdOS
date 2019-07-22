#ifndef __WatchDog_H__
#define __WatchDog_H__

#include "Core\Type.h"

// ���Ź�
class WatchDog
{
private:
	WatchDog();

public:
	uint Timeout; // ��ǰ��ʱʱ��

	bool Config(uint ms);	// ���ÿ��Ź�ι������ʱ�䣬������ʱ�佫����MCU
	void ConfigMax();		// ���Ź��޷��رգ�ֻ������һ�����ֵ
	void Feed(); // ι��

	static WatchDog& Current();
	static void FeedDogTask(void* param);
	// �򿪿��Ź����ι��ʱ��26208ms��Ĭ��20ms
	static void Start(uint msTimeout = 5000, uint msFeed = 20);
};

#endif
