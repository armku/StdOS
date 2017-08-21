#include "DateTime.h"
#include "SString.h"
#include "Version.h"
#include <stdio.h>

// IDA OK
Version::Version()
{
    this->Major = 0;
    this->Minor = 0;
    this->Build = 0;
}

Version::Version(int value)
{
    this->Major = (value >> 24) &0xff;
    this->Minor = (value >> 16) &0xff;
    this->Build = (value) &0xffff;
}
//IDA OK
Version::Version(int major, int minor, int build)
{
    this->Major = major;
    this->Minor = minor;
    this->Build = build;
	if(build>120000)
	{
		this->SetCompile(build);
	}
}
//IDA OK
Version::Version(const Version &ver)
{
    this->Major = ver.Major;
    this->Minor = ver.Minor;
    this->Build = ver.Build;
}

Version::Version(Version && ver)
{
    this->Major = ver.Major;
    this->Minor = ver.Minor;
    this->Build = ver.Build;
}

Version &Version::operator = (const Version &ver)
{
    Version *v = new Version();
    v->Major = ver.Major;
    v->Minor = ver.Minor;
    v->Build = ver.Build;
    return  *v;
}

int Version::ToValue()const
{
    int ret = 0;
    ret = this->Major;
    ret <<= 8;
    ret |= this->Minor;
    ret <<= 16;
    ret |= this->Build;
    return ret;
}

int Version::CompareTo(const Version &value)const
{
    if (this->Major > value.Major)
    {
        return 1;
    }
    else if (this->Major < value.Major)
    {
        return  - 1;
    }
    else
    {}
    if (this->Minor > value.Minor)
    {
        return 1;
    }
    else if (this->Minor < value.Minor)
    {
        return  - 1;
    }
    else
    {}
    if (this->Build > value.Build)
    {
        return 1;
    }
    else if (this->Build < value.Build)
    {
        return  - 1;
    }
    else
    {}
        return 0;
}

bool operator == (const Version &left, const Version &right)
{
    return (left.CompareTo(right)) == 0;
}

bool operator != (const Version &left, const Version &right)
{
    return (left.CompareTo(right)) != 0;
}

bool operator > (const Version &left, const Version &right)
{
    return (left.CompareTo(right)) > 0;
}

bool operator < (const Version &left, const Version &right)
{
    return (left.CompareTo(right)) < 0;
}

bool operator >= (const Version &left, const Version &right)
{
    return (left.CompareTo(right)) >= 0;
}

bool operator <= (const Version &left, const Version &right)
{
    return (left.CompareTo(right)) <= 0;
}

// 根据版本号反推编译时间。
DateTime Version::Compile()const
{
    DateTime *dt = new DateTime();
	dt->ParseDays(this->Build+10957);//加上1970-2000年的天数

    return  *dt;
}
const byte mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31}; 
#define leapyear(year)		((year) % 4 == 0)
#define days_in_year(a) 	(leapyear(a) ? 366 : 365)
#define days_in_month(a) 	(mon_table[(a) - 1])
// 设置编译日期
Version &Version::SetCompile(int year, int month, int day)
{
    if (year < 2000)
    {
        this->Build = 0;
    }
	else if((month<1)||(month>12))
	{
		this->Build = 0;
	}
	else if((day<1)||(day>31))
	{
		this->Build=0;
	}
    else
    {
        int days = 0;
		month--;
		while(month>0)
		{
			days+=days_in_month(month);
			month--;
		}
		days+=day;
        while (year > 2000)
        {
            days += days_in_year(year);
            year--;
        }		
		this->Build=days;
    }	
    return  *this;
}

// 设置编译日期 170325
Version &Version::SetCompile(int buildday)
{
    this->Build = buildday;
    return  *this;
}

String Version::ToString()const
{
    char *buf = new char[100];
    snprintf(buf, 100, "%d.%d.%d", this->Major, this->Minor, this->Build);
    String *ret = new String(buf);

    return  *ret;
}
