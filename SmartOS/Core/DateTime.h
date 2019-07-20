#ifndef __DateTime_H__
#define __DateTime_H__

#include "Type.h"

// 时间日期
class DateTime
{
public:
	short	Year;
	uint8_t	Month;
	uint8_t	Day;
	uint8_t	Hour;
	uint8_t	Minute;
	uint8_t	Second;
	short	Ms;

	DateTime();
	DateTime(int year, int month, int day);
	DateTime(int seconds);
	DateTime(const DateTime& value);
//	DateTime(DateTime&& value);

	DateTime& operator=(int seconds);
    DateTime& operator=(const DateTime& value);

	DateTime& Parse(int seconds);
	DateTime& ParseMs(int64_t ms);
	DateTime& ParseDays(int days);

	// 1970/1/1以来天数
	int TotalDays() const;
	// 1970/1/1以来秒数
	int TotalSeconds() const;
	// 1970/1/1以来毫秒数
	int64_t TotalMs() const;

	// 获取星期，0~6表示星期天到星期六
	uint8_t DayOfWeek() const;
	// 取时间日期的日期部分
	DateTime Date() const;

	// Add函数返回新的实例
	DateTime AddYears(int value) const;
	DateTime AddMonths(int value) const;
	DateTime AddDays(int value) const;
	DateTime AddHours(int value) const;
	DateTime AddMinutes(int value) const;
	DateTime AddSeconds(int value) const;
	DateTime AddMilliseconds(int64_t value) const;
	
	// 时间比较
	int CompareTo(const DateTime& value) const;
    friend bool operator==	(const DateTime& left, const DateTime& right);
    friend bool operator!=	(const DateTime& left, const DateTime& right);
    friend bool operator>	(const DateTime& left, const DateTime& right);
    friend bool operator<	(const DateTime& left, const DateTime& right);
    friend bool operator>=	(const DateTime& left, const DateTime& right);
    friend bool operator<=	(const DateTime& left, const DateTime& right);

	void Show(bool newLine = false) const;

	// 默认格式化时间为yyyy-MM-dd HH:mm:ss
	/*
	d短日期 M/d/yy
	D长日期 yyyy-MM-dd
	t短时间 mm:ss
	T长时间 HH:mm:ss
	f短全部 M/d/yy HH:mm
	F长全部 yyyy-MM-dd HH:mm:ss
	*/
	//cstring GetString(uint8_t kind = 'F', char* str = NULL);
private:
	void Init();
};

#endif
