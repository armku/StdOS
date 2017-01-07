#include "TTime.h"

TTime::TTime()
{
}
//返回当前毫秒数
uint TTime::Ms()
{
	return this->_Current;
}
//设置系统当前时间,Unix格式秒数
void TTime::SetTime(UInt64 seconds)
{
	
}
//系统时间
uint TTime::Seconds()
{
	return 0;
}
//延时ms
void TTime::Sleep(uint ms)
{
	
}
//系统启动以来的毫秒数
uint64_t TTime::Current()
{
	return this->_Current;
}
