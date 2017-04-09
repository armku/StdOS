#include "DateTime.h"
#include "SString.h"
#include "Version.h"
#include <stdio.h>

Version::Version()
{
    this->Major = 0;
    this->Minor = 0;
    this->Build = 2017;
}

Version::Version(int value)
{
    this->Major = (value >> 24) &0xff;
    this->Minor = (value >> 16) &0xff;
    this->Build = (value) &0xffff;
}

Version::Version(int major, int minor, int build)
{
    this->Major = major;
    this->Minor = minor;
    this->Build = build;
}

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
    dt->Year = this->Build;
	
	return *dt;
}

// 设置编译日期
Version &Version::SetCompile(int year, int month, int day)
{
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
	char *buf=new char[100];
	snprintf(buf,100,"%d.%d.%d",this->Major,this->Minor,this->Build);
    String *ret = new String(buf);

    return  *ret;
}
