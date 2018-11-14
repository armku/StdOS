#ifndef __Version_H__
#define __Version_H__

#include "Type.h"

// 版本
class Version
{
public:
	int	Major;	// 主版本
	int	Minor;	// 次版本
	int Year;	// 年
	int MonthDay;// 月日

	Version(int major = 0, int minor = 0, int year = 0, int monday = 0);
	Version(const Version& ver);
	int Parse(char* str);//适配版本，格式1.0.2018.1114
	int Parse(const char* str) { return Parse((char*)str); }//适配版本，格式1.0.2018.1114
	
    Version& operator=(const Version& ver);
	void Show(bool newLine = true) const;
	
	int CompareTo(const Version& value) const;
    friend bool operator==	(const Version& left, const Version& right);
    friend bool operator!=	(const Version& left, const Version& right);
    friend bool operator>	(const Version& left, const Version& right);
    friend bool operator<	(const Version& left, const Version& right);
    friend bool operator>=	(const Version& left, const Version& right);
    friend bool operator<=	(const Version& left, const Version& right);
};

/*
版本号格式：1.0.2018.1114
*/

#endif
