#include "Version.h"
#include <stdio.h>

Version::Version(int major, int minor, int year, int monday)
{
	this->Major = major;
	this->Minor = minor;
	this->Year = year;
	this->MonthDay = monday;
}

Version::Version(const Version &ver)
{
	this->Major = ver.Major;
	this->Minor = ver.Minor;
	this->Year = ver.Year;
	this->MonthDay = ver.MonthDay;
}
//ÊÊÅä°æ±¾£¬¸ñÊ½1.0.2018.1114
int Version::Parse(char* str)
{

	return -1;
}

void Version::Show(bool newLine) const
{
	StdPrintf("%d.%d.%d.%d", this->Major, this->Minor, this->Year, this->MonthDay);
	newLine ? StdPrintf("\n") : StdPrintf("");
}
Version &Version::operator = (const Version &ver)
{
	auto v = new Version(ver.Major, ver.Minor, ver.Year, ver.MonthDay);
	v->Major = ver.Major;
	v->Minor = ver.Minor;
	v->Year = ver.Year;
	v->MonthDay = ver.MonthDay;
	return  *v;
}

int Version::CompareTo(const Version &value)const
{
	if (this->Major > value.Major)
	{
		return 1;
	}
	else if (this->Major < value.Major)
	{
		return  -1;
	}
	else
	{
	}
	if (this->Minor > value.Minor)
	{
		return 1;
	}
	else if (this->Minor < value.Minor)
	{
		return  -1;
	}
	else
	{
	}
	if (this->Year > value.Year)
	{
		return 1;
	}
	else if (this->Year < value.Year)
	{
		return  -1;
	}
	else
	{
	}
	if (this->MonthDay > value.MonthDay)
	{
		return 1;
	}
	else if (this->MonthDay < value.MonthDay)
	{
		return  -1;
	}
	else
	{
	}
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
