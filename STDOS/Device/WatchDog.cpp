#include "WatchDog.h"

WatchDog::WatchDog()
{ 
	this->Timeout=3000;
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
// 打开看门狗。最长喂狗时间26208ms，默认2000ms
void WatchDog::Start(uint msTimeOut,uint msFeed)
{
	cur=new WatchDog();
	cur->Config(msTimeOut);
	Sys.AddTask(FeedDogTask,&WatchDog::Current(),10,msFeed,"FeedDog");
}
