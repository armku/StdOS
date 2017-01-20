#include "DateTime.h"

DateTime::DateTime()
{
}
DateTime::DateTime(ushort year, byte month, byte day)
{
	this->Year=year;
	this->Month=month;
	this->Day=day;
}
DateTime::DateTime(uint seconds)
{
	
}
DateTime::DateTime(const DateTime &value)
{
	
}
#if 0
DateTime::DateTime(DateTime && value)
{
}
#endif
DateTime &DateTime::operator = (uint seconds)
{
	return *this;
}
DateTime &DateTime::operator = (const DateTime &value)
{
	return *this;
}
DateTime &DateTime::Parse(uint seconds)
{
	return *this;
}
DateTime &DateTime::ParseMs(UInt64 ms)
{
	return *this;
}
uint DateTime::TotalDays()const
{
	
	return 0;
}
uint DateTime::TotalSeconds()const
{	
	return 0;
}
UInt64 DateTime::TotalMs()const
{	
	return 0;
}
byte DateTime::DayOfWeek()const
{
	return 0;
}
DateTime DateTime::Date()const
{
	return *this;
}
//当前时间
void DateTime::Now()
{
	
}
