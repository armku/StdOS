#include "Task.h"


Task::Task(){}

void Task::TimeTick() //���10ms����һ��
{
    for (ushort i = 0; i < this->NextID; i++)
    {
        this->thread[i].TickCur++;
    }
    this->ticks++;
}

void Task::Routin() //����
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
