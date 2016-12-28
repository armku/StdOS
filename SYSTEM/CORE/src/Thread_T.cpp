#include "Thread_T.h"


CThread::CThread()
{
	this->canRun=false;
}
CThread::~CThread()
{
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


