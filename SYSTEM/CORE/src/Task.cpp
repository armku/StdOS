#include <stdio.h>
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

void Task::AddTask(void(*callback)(void), uint delaycntms, uint intervalms,const char* name)
{
	static int i=1;

    if (this->NextID >= PROCESSCNT)
    {
        return ;
    }
    this->thread[this->NextID].SetPara(callback, intervalms, delaycntms);
    this->NextID++;
	printf("增加任务%02d: ",i++);
	printf(name);
	printf("\n");
}

CThread::CThread()
{
	this->canRun=false;
}
void CThread::checkRun()//判断是否运行
{
	//间隔运行
	if(this->canRun)
	{
		//时间间隔为0，每次都运行
		if(this->intervalms==0)
		{
			this->TickCur=0;
			this->callback();
		}
		//到达间隔时间，运行
		else if(this->TickCur>=this->intervalms)
		{
			this->TickCur=0;
			this->callback();
		}		
		else{}
	}
	//第一次运行
	else
		{
			if(this->TickCur>=this->delaycnt)
			{
				this->canRun=true;
				this->TickCur=0;
				this->callback();
			}
		}
}
void CThread::SetPara(void (*callback)(void),uint intervalms,uint delaycntms)//设置参数
{
	this->callback=callback;
	this->intervalms=intervalms;
	this->delaycnt=delaycntms;
	if(delaycntms==0)
	{
		this->canRun=true;
	}
	else
	{
		this->canRun=false;
	}
}
