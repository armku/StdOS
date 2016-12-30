#include "stdio.h"
#include "TimeCost.h"

TimeCost::TimeCost()
{
	this->startTickCounts=0;
}
//显示执行时间
void TimeCost::Show()
{
	printf("执行 %d 微妙",12);
}
//开始计时
void TimeCost::Start()
{
	this->startTickCounts=0;
}
//经过的时间 us
uint TimeCost::Elapsed()
{
	uint ret=12;
	ret-=this->startTickCounts;
	return ret;
}

