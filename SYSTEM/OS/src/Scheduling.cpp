#include "Scheduling.h"


CTaskScheduler::CTaskScheduler(){}
CTaskScheduler::~CTaskScheduler(){

}

void CTaskScheduler::TimeTick() //���10ms����һ��
{
    for (uint16_t i = 0; i < this->NextID; i++)
    {
        this->thread[i].TickCur++;
    }
    this->ticks++;
}

void CTaskScheduler::Routin() //����
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
