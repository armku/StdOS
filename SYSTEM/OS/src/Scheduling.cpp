#include "Scheduling.h"


CTaskScheduler::CTaskScheduler(){}
CTaskScheduler::~CTaskScheduler(){

}

void CTaskScheduler::TimeTick() //间隔10ms调用一次
{
    for (ushort i = 0; i < this->NextID; i++)
    {
        this->thread[i].TickCur++;
    }
    this->ticks++;
}

void CTaskScheduler::Routin() //运行
{
    for (ushort i = 0; i < this->NextID; i++)
    {
        this->thread[i].checkRun();
    }
}

void CTaskScheduler::ThreadAdd(void(*callback)(void), uint intervalms, uint delaycntms)
{

    if (this->NextID >= PROCESSCNT)
    {
        return ;
    }
    this->thread[this->NextID].SetPara(callback, intervalms, delaycntms);
    this->NextID++;
}
