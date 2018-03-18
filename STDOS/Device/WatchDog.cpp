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
// �򿪿��Ź����ι��ʱ��26208ms��Ĭ��2000ms
void WatchDog::Start(uint msTimeOut,uint msFeed)
{
	cur=new WatchDog();
	cur->Config(msTimeOut);
	Sys.AddTask(FeedDogTask,&WatchDog::Current(),10,msFeed,"FeedDog");
}
