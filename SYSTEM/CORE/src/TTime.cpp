#include "TTime.h"

TTime::TTime()
{
}
//返回当前毫秒数
uint TTime::Ms()
{
	return this->Current;
}
//设置系统当前时间,Unix格式秒数
void TTime::SetTime(UInt64 seconds)
{
	
}
