#include "DateTime.h"
#include <stdio.h>

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
#define FEBRUARY		2
#define STARTOFTIME		1970
#define SECDAY			86400L           /*  一天有多少s */
#define SECYR			(SECDAY * 365)
#define leapyear(year)		((year) % 4 == 0)
#define days_in_year(a) 	(leapyear(a) ? 366 : 365)
#define days_in_month(a) 	(month_days[(a) - 1])
static int month_days[12] = 
{
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};
DateTime &DateTime::operator = (uint seconds)
{
	register uint i;
    register long hms, day;
		
    day = seconds / SECDAY; /* 有多少天 */
    hms = seconds % SECDAY; /* 今天的时间，单位s */

    /* Hours, minutes, seconds are easy */
  this->Hour = hms / 3600;
  this->Minute = (hms % 3600) / 60;
  this->Second = (hms % 3600) % 60;

     /* Number of years in days */ /*算出当前年份，起始的计数年份为1970年*/
    for (i = STARTOFTIME; day >= days_in_year(i); i++)
    {
        day -= days_in_year(i);
    } 
	this->Year = i;

     /* Number of months in days left */ /*计算当前的月份*/
    if (leapyear(this->Year))
    {
        days_in_month(FEBRUARY) = 29;
    }
    for (i = 1; day >= days_in_month(i); i++)
    {
        day -= days_in_month(i);
    }
    days_in_month(FEBRUARY) = 28;
    this->Month = i;

     /* Days are what is left over (+1) from all that. */ /*计算当前日期*/
    this->Day = day + 1;
  
	
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
	int leapsToDate;
    int lastYear;
    int day;
    int MonthOffset[] = 
    {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
    };

    lastYear =this->Year - 1;

    /*计算从公元元年到计数的前一年之中一共经历了多少个闰年*/
    leapsToDate = lastYear / 4-lastYear / 100+lastYear / 400;

    /*如若计数的这一年为闰年，且计数的月份在2月之后，则日数加1，否则不加1*/
    if ((this->Year % 4 == 0) && ((this->Year % 100 != 0) || (this->Year % 400 == 0)) && (this->Month > 2))
    {
        /*
         * We are past Feb. 29 in a leap year
         */
        day = 1;
    }
    else
    {
        day = 0;
    }

    day += lastYear * 365+leapsToDate + MonthOffset[this->Month - 1] + this->Day; /*计算从公元元年元旦到计数日期一共有多少天*/

	return day % 7;
}
DateTime DateTime::Date()const
{
	return *this;
}
//当前时间
DateTime DateTime::Now()
{
	DateTime dt;
	dt.Year=2017;
	dt.Month=01;
	dt.Day=29;
	dt.Hour=10;
	dt.Minute=55;
	dt.Second=12;
	return dt;
}
void DateTime::Show(bool newLine)const
{
	printf("%04d-%02d-%02d %02d:%02d:%02d",this->Year,this->Month,this->Day,this->Hour,this->Minute,this->Second);
	Object::Show(true);
}
