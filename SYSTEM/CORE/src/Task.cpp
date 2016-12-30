#include <stdio.h>
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

void Task::AddTask(void(*callback)(void), uint delaycntms, uint intervalms,const char* name)
{
	static int i=1;

    if (this->NextID >= PROCESSCNT)
    {
        return ;
    }
    this->thread[this->NextID].SetPara(callback, intervalms, delaycntms);
    this->NextID++;
	printf("��������%02d: ",i++);
	printf(name);
	printf("\n");
}

CThread::CThread()
{
	this->canRun=false;
}
void CThread::checkRun()//�ж��Ƿ�����
{
	//�������
	if(this->canRun)
	{
		//ʱ����Ϊ0��ÿ�ζ�����
		if(this->intervalms==0)
		{
			this->TickCur=0;
			this->callback();
		}
		//������ʱ�䣬����
		else if(this->TickCur>=this->intervalms)
		{
			this->TickCur=0;
			this->callback();
		}		
		else{}
	}
	//��һ������
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
void CThread::SetPara(void (*callback)(void),uint intervalms,uint delaycntms)//���ò���
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
