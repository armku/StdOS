#include "Task.h"


Task::Task(){}

void Task::TimeTick() //间隔10ms调用一次
{
    for (ushort i = 0; i < this->NextID; i++)
    {
        this->thread[i].TickCur++;
    }
    this->ticks++;
}

void Task::Routin() //运行
{
    for (ushort i = 0; i < this->NextID; i++)
    {
        this->thread[i].checkRun();
    }
}

void Task::AddTask(void(*callback)(void), uint delaycntms, uint intervalms)
{

    if (this->NextID >= PROCESSCNT)
    {
        return ;
    }
    this->thread[this->NextID].SetPara(callback, intervalms, delaycntms);
    this->NextID++;
}
