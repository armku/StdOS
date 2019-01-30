#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Version.h"
#include "../Type.h"
#include "../../StdOS/Kernel/Sys.h"

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
template <typename T1>
void memset1(T1 *buf, int len, char val)
{
	for (int i = 0; i < len; i++)
		buf[i] = val;
}

static void parse(char* str, int* major, int* minor, int* year, int* monday)
{
	auto len = strlen(str);
	char buf[10];
	int dotpos[5];
	int add = 0;
	memset1(dotpos, ArrayLength(dotpos), 0);
	for (int i = 0; i < len; i++)
	{
		if (str[i] == '.')
		{
			dotpos[add++] = i;			
		}
		if (str[i] == 0)
		{
			dotpos[add++] = i;
		}
		if (add > 3)
			break;
	}
	if (dotpos[3] == 0)
		dotpos[3] = len;
	//major
	memset1(buf, ArrayLength(buf), 0);
	for (int i = 0; i < dotpos[0]; i++)
	{
		buf[i] = str[i];
		buf[i + 1] = 0;
	}
	*major = atoi(buf);
	//minor
	memset1(buf, ArrayLength(buf), 0);
	for (int i = dotpos[0]+1; i < dotpos[1]; i++)
	{
		buf[i-(dotpos[0] + 1)] = str[i];
		buf[i -(dotpos[0] + 1)+ 1] = 0;
	}
	*minor = atoi(buf);
	//year
	memset1(buf, ArrayLength(buf), 0);
	for (int i = dotpos[1] + 1; i < dotpos[2]; i++)
	{
		buf[i - (dotpos[1] + 1)] = str[i];
		buf[i - (dotpos[1] + 1) + 1] = 0;
	}
	*year = atoi(buf);
	//monday
	memset1(buf, ArrayLength(buf), 0);
	for (int i = dotpos[2] + 1; i < dotpos[3]; i++)
	{
		buf[i - (dotpos[2] + 1)] = str[i];
		buf[i - (dotpos[2] + 1) + 1] = 0;
	}
	*monday = atoi(buf);
}
//ÊÊÅä°æ±¾£¬¸ñÊ½1.0.2018.1114
int Version::Parse(char* str)
{
	parse(str, &this->Major, &this->Minor, &this->Year, &this->MonthDay);
	return 0;
}

void Version::Show(bool newLine) const
{
	StdPrintf("%d.%d.%d.%d", this->Major, this->Minor, this->Year, this->MonthDay);
	newLine ? StdPrintf("\n") : StdPrintf("");
}

int Version::Show(char* buf, int pos)
{
	int ret = 0;
	ret = sprintf(buf+pos, "%d.%d.%d.%d", this->Major, this->Minor, this->Year, this->MonthDay);
	return ret;
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
