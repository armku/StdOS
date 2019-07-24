#include "Type.h"
#include "DateTime.h"
#include "SString.h"

#include "Version.h"

/************************************************ Version ************************************************/

Version::Version()
{
	Major = 0;
	Minor = 0;
	Build = 0;
}

Version::Version(int value)
{
	Major = value >> 24;
	Minor = value >> 16;

	Build = value & 0xFFFF;
}

Version::Version(int major, int minor, int build)
{
	Major = major;
	Minor = minor;
	Build = build;

	// 有可能是 170325 这样的写法
	if (build > 120000) SetCompile(build);
}

Version::Version(const Version& ver)
{
	*this = ver;
}

Version::Version(Version&& ver)
{
	*this = ver;
}

Version& Version::operator=(const Version& ver)
{
	Major = ver.Major;
	Minor = ver.Minor;
	Build = ver.Build;

	return *this;
}

int Version::ToValue() const
{
	return (Major << 24) | (Minor << 16) | Build;
}

int Version::CompareTo(const Version& value) const
{
	int n = (int)Major - value.Major;
	if (n) return n;

	n = (int)Minor - value.Minor;
	if (n) return n;

	n = (int)Build - value.Build;
	if (n) return n;

	return 0;
}

bool operator==	(const Version& left, const Version& right) { return left.CompareTo(right) == 0; }
bool operator!=	(const Version& left, const Version& right) { return left.CompareTo(right) != 0; }
bool operator>	(const Version& left, const Version& right) { return left.CompareTo(right) > 0; }
bool operator<	(const Version& left, const Version& right) { return left.CompareTo(right) < 0; }
bool operator>=	(const Version& left, const Version& right) { return left.CompareTo(right) >= 0; }
bool operator<=	(const Version& left, const Version& right) { return left.CompareTo(right) <= 0; }

// 根据版本号反推编译时间
DateTime Version::Compile() const
{
	DateTime dt(2000, 1, 1);

	// Build 是2000-01-01以来的天数
	dt = dt.AddDays(Build);

	return dt;
}

Version& Version::SetCompile(int year, int month, int day)
{
	DateTime dt(2000, 1, 1);
	DateTime dt2(year, month, day);
	Build = (dt2 - dt).TotalDays();

	return *this;
}

// 设置编译日期 170325
Version& Version::SetCompile(int buildday)
{
	int day = buildday % 100;
	buildday /= 100;
	int month = buildday % 100;
	int year = buildday / 100 + 2000;

	DateTime dt(2000, 1, 1);
	DateTime dt2(year, month, day);
	Build = (dt2 - dt).TotalDays();

	return *this;
}

String Version::ToString() const
{
	String str;
	str = str + Major + '.' + Minor + '.' + Build;

	return str;
}
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Version.h"
#include "Sys.h"

template <typename T1>
void memset1(T1 *buf, int len, char val)
{
	for (int i = 0; i < len; i++)
		buf[i] = val;
}

static void parse(char* str, int* major, int* minor, int* year, int* monday)
{
	int len = strlen(str);
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
	for (int i = dotpos[0] + 1; i < dotpos[1]; i++)
	{
		buf[i - (dotpos[0] + 1)] = str[i];
		buf[i - (dotpos[0] + 1) + 1] = 0;
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

//适配版本，格式1.0.2018.1114
int Version::Parse(char* str)
{
	parse(str, &this->Major, &this->Minor, &this->Year, &this->MonthDay);
	return 0;
}

void Version::Show(bool newLine) const
{
	debug_printf("%d.%d.%d.%d", this->Major, this->Minor, this->Year, this->MonthDay);
	newLine ? debug_printf("\n") : debug_printf("");
}
#include <stdio.h>
#include "stdarg.h"
int Version::Show(char* buf, int pos)
{
	int ret = 0;
	ret = sprintf(buf+pos, "%d.%d.%d.%d", this->Major, this->Minor, this->Year, this->MonthDay);
	return ret;
}
