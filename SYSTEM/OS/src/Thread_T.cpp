#include "Thread_T.h"


CThread::CThread()
{
	this->canRun=false;
}
CThread::~CThread()
{
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


