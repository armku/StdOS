#ifndef __WatchDog_H__
#define __WatchDog_H__

#include "Kernel\Sys.h"

// ���Ź�
class WatchDog
{
private:
	WatchDog();

public:
	//WatchDog(uint ms = 3000);
	~WatchDog();

	uint Timeout; // ��ǰ��ʱʱ��

	bool Config(uint ms);	// ���ÿ��Ź�ι������ʱ�䣬������ʱ�佫����MCU
	void ConfigMax();		// ���Ź��޷��رգ�ֻ������һ�����ֵ
	void Feed(); // ι��

	static WatchDog& Current();
	static void FeedDogTask(void* param);
	// �򿪿��Ź����ι��ʱ��26208ms��Ĭ��2000ms
	static void Start(uint msTimeout = 2000, uint msFeed = 500);
};

#endif
