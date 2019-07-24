#ifndef __Version_H__
#define __Version_H__

#include "Type.h"

// 版本
class Version
{
public:
//	byte	Major;	// 主版本
//	byte	Minor;	// 次版本
	short	Build;	// 编译时间。2000年以来天数

	Version();
	Version(int value);
	Version(int major, int minor, int build);
	Version(const Version& ver);
	Version(Version&& ver);

	Version& operator=(const Version& ver);

	int ToValue() const;
	operator int() const { return ToValue(); }

	int CompareTo(const Version& value) const;
	friend bool operator==	(const Version& left, const Version& right);
	friend bool operator!=	(const Version& left, const Version& right);
	friend bool operator>	(const Version& left, const Version& right);
	friend bool operator<	(const Version& left, const Version& right);
	friend bool operator>=	(const Version& left, const Version& right);
	friend bool operator<=	(const Version& left, const Version& right);

	// 根据版本号反推编译时间。
	DateTime Compile() const;
	// 设置编译日期
	Version& SetCompile(int year, int month, int day);
	// 设置编译日期 170325
	Version& SetCompile(int buildday);

	String ToString() const;
public:
	int	Major;	// 主版本
	int	Minor;	// 次版本
	int Year;	// 年
	int MonthDay;// 月日

	Version(int major , int minor , int year , int monday );
	int Parse(char* str);//适配版本，格式1.0.2018.1114
	int Parse(const char* str) { return Parse((char*)str); }//适配版本，格式1.0.2018.1114
	
	void Show(bool newLine = true) const;
	int Show(char* buf, int pos);
	int Show(byte* buf, int pos) { return Show((char*)buf, pos); }
};

/*
版本号格式：1.0.2018.1114
*/

#endif
