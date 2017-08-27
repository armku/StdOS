#include "WatchDog.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif

WatchDog::WatchDog()
{ 
	this->Timeout=3000;
}

WatchDog::~WatchDog()
{
    ConfigMax();
}

WatchDog* cur;
WatchDog& WatchDog::Current()
{
	return *cur;
}
void WatchDog::FeedDogTask(void* param)
{
	WatchDog *dog = (WatchDog*)param;
    dog->Feed();
}
// �򿪿��Ź����ι��ʱ��26208ms��Ĭ��2000ms
void WatchDog::Start(uint msTimeOut,uint msFeed)
{
	cur=new WatchDog();
	cur->Config(msTimeOut);
	Sys.AddTask(FeedDogTask,&WatchDog::Current(),10,msFeed,"FeedDog");
}
//���Դ���
#if 0
#if DEBUG
        WatchDog::Start(20000, 10000);
    #else 
        WatchDog::Start();
    #endif 
#endif
