#ifndef __DateTime_H__
#define __DateTime_H__

#include "Buffer.h"
//#include "TimeSpan.h"

// ʱ������
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
	DateTime(DateTime&& value);

	// ���صȺ������
    DateTime& operator=(int seconds);
    DateTime& operator=(const DateTime& value);

	DateTime& Parse(int seconds);
	DateTime& ParseMs(int64_t ms);
	DateTime& ParseDays(int days);

	// 1970/1/1��������
	int TotalDays() const;
	// 1970/1/1��������
	int TotalSeconds() const;
	// 1970/1/1����������
	int64_t TotalMs() const;

	// ��ȡ���ڣ�0~6��ʾ�����쵽������
	uint8_t DayOfWeek() const;
	// ȡʱ�����ڵ����ڲ���
	DateTime Date() const;

	// Add���������µ�ʵ��
	DateTime AddYears(int value) const;
	DateTime AddMonths(int value) const;
	DateTime AddDays(int value) const;
	DateTime AddHours(int value) const;
	DateTime AddMinutes(int value) const;
	DateTime AddSeconds(int value) const;
	DateTime AddMilliseconds(int64_t value) const;
	//DateTime Add(const TimeSpan& value) const;

	//DateTime operator+(const TimeSpan& value);
	//DateTime operator-(const TimeSpan& value);
    //friend TimeSpan operator-(const DateTime& left, const DateTime& right);

	// ʱ��Ƚ�
	int CompareTo(const DateTime& value) const;
    friend bool operator==	(const DateTime& left, const DateTime& right);
    friend bool operator!=	(const DateTime& left, const DateTime& right);
    friend bool operator>	(const DateTime& left, const DateTime& right);
    friend bool operator<	(const DateTime& left, const DateTime& right);
    friend bool operator>=	(const DateTime& left, const DateTime& right);
    friend bool operator<=	(const DateTime& left, const DateTime& right);

	String ToString() const;
	void Show(bool newLine = false) const;

	// Ĭ�ϸ�ʽ��ʱ��Ϊyyyy-MM-dd HH:mm:ss
	/*
	d������ M/d/yy
	D������ yyyy-MM-dd
	t��ʱ�� mm:ss
	T��ʱ�� HH:mm:ss
	f��ȫ�� M/d/yy HH:mm
	F��ȫ�� yyyy-MM-dd HH:mm:ss
	*/
	//cstring GetString(uint8_t kind = 'F', char* str = nullptr);

	// ��ǰʱ��
	static DateTime Now();

#if DEBUG
	static void Test();
#endif

private:
	void Init();
	Buffer ToArray();
	const Buffer ToArray() const;
};

#endif
