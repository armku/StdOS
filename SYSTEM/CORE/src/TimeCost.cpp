#include "stdio.h"
#include "TimeCost.h"

TimeCost::TimeCost()
{
	this->startTickCounts=0;
}
//��ʾִ��ʱ��
void TimeCost::Show()
{
	printf("ִ�� %d ΢��",12);
}
//��ʼ��ʱ
void TimeCost::Start()
{
	this->startTickCounts=0;
}
//������ʱ�� us
uint TimeCost::Elapsed()
{
	uint ret=12;
	ret-=this->startTickCounts;
	return ret;
}

