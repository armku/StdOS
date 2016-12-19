#include "Scheduling.h"


CTaskScheduler::CTaskScheduler(){}
CTaskScheduler::~CTaskScheduler(){

}

void CTaskScheduler::TimeTick() //间隔10ms调用一次
{
    for (uint16_t i = 0; i < this->NextID; i++)
    {
        this->thread[i].TickCur++;
    }
    this->ticks++;
}

void CTaskScheduler::Routin() //运行
{
    for (uint16_t i = 0; i < this->NextID; i++)
    {
        this->thread[i].checkRun();
    }
}

void CTaskScheduler::ThreadAdd(void(*callback)(void), uint32_t intervalms, uint32_t delaycntms)
{

    if (this->NextID >= PROCESSCNT)
    {
        return ;
    }
    this->thread[this->NextID].SetPara(callback, intervalms, delaycntms);
    this->NextID++;
}
