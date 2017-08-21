#include "DateTime.h"
#include <stdio.h>

//ida ok
DateTime::DateTime()
{
    this->Init();
}
//ida ok
DateTime::DateTime(int year, int month, int day)
{
	if(year<1970)
	{
		year=1970;
	}
	if(month<1 || month>12)
	{
		month=1;
	}
	if(day<1 || day >31)
	{
		day=1;
	}
    this->Year = year;
    this->Month = month;
    this->Day = day;
	this->Hour=0;
	this->Minute=0;
	this->Second=0;
	this->Ms=0;
}
//ida ok
DateTime::DateTime(int seconds)
{
   this->Parse(seconds);
   this->Ms = 0;

}
//ida ok
DateTime::DateTime(const DateTime &value)
{
	*this=value;
}
DateTime::DateTime(DateTime&& value)
{
  //?
}
//ida ok
DateTime &DateTime::operator = (int seconds)
{
    *this=(seconds);

    return  *this;
}
DateTime &DateTime::operator = (const DateTime &value)
{
	if ( *this != value )
  {
//     this->ToArray((DateTime *)&v4);
//    this->ToArray((DateTime *)&v5);
//    Buffer::operator=(&v5, &v4);
  }
    return  *this;
}
//ida ok
DateTime &DateTime::Parse(int seconds)
{
    this->Second = seconds % 60;
    this->Minute = seconds / 60 % 60;
    int minutes = seconds / 60 / 60;
    this->Minute = minutes % 24;
    this->ParseDays(minutes / 24);

    return  *this;
}

//ida ok
DateTime &DateTime::ParseMs(Int64 ms)
{
    this->Parse(ms / 1000);
    this->Ms = ms % 1000;
    return  *this;
}
const int CummulativeDaysForMonth[] = 
{
    0, 0x1F, 0x3B, 0x5A, 0x78, 0x97, 0xB5, 0xD4, 0xF3, 0x111, 0x130, 0x14E, 0x16D
};
//ida ok
DateTime &DateTime::ParseDays(int days)
{
    int v2; // r2@3
    signed int daysRemain; // r1@5
    int v4; // r6@9
    bool v5; // r6@17

    this->Year = days / 365;
    if (this->Year < 136)
    {
        this->Year += 1970;
    }
    v2 = (this->Year - 1) / 4 - (this->Year - 1) / 100 + (this->Year - 1) / 400 - 477 + 365 *(this->Year - 1970);
    if (v2 > days)
    {
        --this->Year;
        v2 = (this->Year - 1) / 4 - (this->Year - 1) / 100 + (this->Year - 1) / 400 - 477 + 365 *(this->Year - 1970);
    }
    daysRemain = days - v2;

    this->Month = daysRemain / 31+1;
    v4 = (!(this->Year % 4) && this->Year % 100 || !(this->Year % 400)) && this->Month + 1 > 2;
    if (daysRemain >= v4 + CummulativeDaysForMonth[this->Month])
        ++this->Month;
    v5 = (!(this->Year % 4) && this->Year % 100 || !(this->Year % 400)) && this->Month > 2;
    this->Day = daysRemain - (v5 + (CummulativeDaysForMonth[this->Month - 1])) + 1;
    return  *this;
}
//ida ok
// 1970/1/1以来天数
int DateTime::TotalDays()const
{
    int v1 = (!(this->Year % 4) && this->Year % 100 || !(this->Year % 400))
    && this->Month > 2;
  return CummulativeDaysForMonth[this->Month - 1]
       + v1
       + (this->Year - 1) / 4
       - (this->Year - 1) / 100
       + (this->Year - 1) / 400
       - 477
       + 365 * (this->Year - 1970)
       + this->Day
       - 1;
}
//ida ok
// 1970/1/1以来秒数
int DateTime::TotalSeconds()const
{
    return this->Second
       + 60 * (this->Minute + 60 * (this->Hour + 24 * this->TotalDays()));
}
//ida ok
// 1970/1/1以来毫秒数
Int64 DateTime::TotalMs()const
{
    return 1000 * this->TotalSeconds() + this->Ms;
}
//ida ok
byte DateTime::DayOfWeek()const
{
    int v1; // r6@5

  v1 = (!(this->Year % 4) && this->Year % 100 || !(this->Year % 400))
    && this->Month > 2;
  return ((CummulativeDaysForMonth[this->Month - 1]
                          + v1
                          + (this->Year - 1) / 4
                          - (this->Year - 1) / 100
                          + (this->Year - 1) / 400
                          - 477
                          + 365 * (this->Year - 1970)
                          + this->Day
                          + 3)
                         % 7);
}
DateTime DateTime::Date()const
{
     return DateTime(this->Year, this->Month, this->Day);
}
DateTime DateTime::AddYears(int value) const
{
  //?  
  return *this;
}
DateTime DateTime::AddMonths(int value) const
{
  //?  
  return *this;
}
DateTime DateTime::AddDays(int value) const
{
  //?  
  return *this;
}
DateTime DateTime::AddHours(int value) const
{
  //?  
  return *this;
}
DateTime DateTime::AddMinutes(int value) const
{
  //?  
  return *this;
}
DateTime DateTime::AddSeconds(int value) const
{
  //?  
  return *this;
}
DateTime DateTime::AddMilliseconds(Int64 value) const
{
  //?  
  return *this;
}
DateTime DateTime::Add(const TimeSpan& value) const
{
  //?  
  return *this;
}
DateTime DateTime::operator+(const TimeSpan& value)
{
	//?  
  return *this;
}
DateTime DateTime::operator-(const TimeSpan& value)
{
	//?  
  return *this;
}
//TimeSpan DateTime::operator-(const DateTime& left, const DateTime& right)
//{
//	//?  
//  return *this;
//}
int DateTime::CompareTo(const DateTime& value) const
{
	//?  
  return 0;
}
bool operator==	(const DateTime& left, const DateTime& right)
{
	return true;
}
bool operator!=	(const DateTime& left, const DateTime& right)
{
	return true;
}
bool operator>	(const DateTime& left, const DateTime& right)
{
	return true;
}
bool operator<	(const DateTime& left, const DateTime& right)
{
	return true;
}
bool operator>=	(const DateTime& left, const DateTime& right)
{
	return true;
}
bool operator<=	(const DateTime& left, const DateTime& right)
{
	return true;
}

//String DateTime::ToString() const
//{
//}
void DateTime::Show(bool newLine)const
{
    printf("%04d-%02d-%02d %02d:%02d:%02d", this->Year, this->Month, this->Day, this->Hour, this->Minute, this->Second);
}
//当前时间
DateTime DateTime::Now()
{
    DateTime dt;
    dt.Year = 2017;
    dt.Month = 01;
    dt.Day = 29;
    dt.Hour = 10;
    dt.Minute = 55;
    dt.Second = 12;
    return dt;
}
#if DEBUG
void DateTime::Test()
{
}
#endif
// ida ok
void DateTime::Init()
{
    this->Year = 1970;
    this->Month = 1;
    this->Day = 1;
    this->Hour = 0;
    this->Minute = 0;
    this->Second = 0;
    this->Ms = 0;
}
//Buffer DateTime::ToArray()
//{
//}
//const Buffer DateTime::ToArray() const
//{
//	return Buffer(this, *this, 10);
//}
