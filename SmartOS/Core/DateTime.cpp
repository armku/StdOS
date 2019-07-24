#include "_Core.h"

#include "SString.h"
#include "DateTime.h"

#if defined(__CC_ARM)
#include <time.h>
#else
#include <ctime>
#endif

/************************************************ DateTime ************************************************/

// ���ǵ�ʱ������� 1/1/1970 00:00:00.000 �ڹ�������1/1/1970��������
#define BASE_YEAR                   1970
// ��ֹ�����꣬�ѹ�ȥ����������1969 / 4 - 1969 / 100 + 1969 / 400 = 492 - 19 + 4 = 477
#define BASE_YEAR_LEAPYEAR_ADJUST   (((BASE_YEAR - 1) / 4) - ((BASE_YEAR - 1) / 100) + ((BASE_YEAR - 1) / 400))
#define BASE_YEAR_DAYOFWEEK_SHIFT   4		// ����ƫ��

// �����������
#define BASE_YEAR_SECONDS			62135596800UL

const int CummulativeDaysForMonth[13] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

#define IS_LEAP_YEAR(y)             (((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0))
// ���ڻ������������������������
#define NUMBER_OF_LEAP_YEARS(y)     ((((y - 1) / 4) - ((y - 1) / 100) + ((y - 1) / 400)) - BASE_YEAR_LEAPYEAR_ADJUST)
#define NUMBER_OF_YEARS(y)          (y - BASE_YEAR)

#define YEARS_TO_DAYS(y)            ((NUMBER_OF_YEARS(y) * 365) + NUMBER_OF_LEAP_YEARS(y))
#define MONTH_TO_DAYS(y, m)         (CummulativeDaysForMonth[m - 1] + ((IS_LEAP_YEAR(y) && (m > 2)) ? 1 : 0))

DateTime::DateTime()
{
	Init();
}

DateTime::DateTime(int year, int month, int day)
{
	if (year < BASE_YEAR) year = BASE_YEAR;
	if (month < 1 || month > 12) month = 1;
	if (day < 1 || day > 31) day = 1;

	Year = year;
	Month = month;
	Day = day;
	Hour = 0;
	Minute = 0;
	Second = 0;
	Ms = 0;
}

DateTime::DateTime(int seconds)
{
	Parse(seconds);

	Ms = 0;
}

DateTime::DateTime(const DateTime& value)
{
	*this = value;
}

DateTime::DateTime(DateTime&& value)
{
	// �����Է����ݣ��ѶԷ���ʼ��ΪĬ��ֵ
	ToArray() = value.ToArray();
	// �Է����������ˣ��Ͳ�Ҫ�˷�ʱ������ʼ����������û����Ҫ�ͷŵ���Դ
	//value.Init();
}

void DateTime::Init()
{
	//Buffer(&Year, &Ms - &Year + sizeof(Ms)).Clear();
	Year = BASE_YEAR;
	Month = 1;
	Day = 1;
	Hour = 0;
	Minute = 0;
	Second = 0;
	Ms = 0;
}

DateTime& DateTime::Parse(int seconds)
{
	auto& st = *this;

	//if(seconds >= BASE_YEAR_SECONDS) seconds -= BASE_YEAR_SECONDS;

	// �ֱ������롢�롢�֡�ʱ��ʣ������
	int time = seconds;
	st.Second = time % 60;
	time /= 60;
	st.Minute = time % 60;
	time /= 60;
	st.Hour = time % 24;
	time /= 24;

	ParseDays(time);

	return st;
}

DateTime& DateTime::ParseMs(Int64 ms)
{
	Parse((int)(ms / 1000LL));
	Ms = ms % 1000LL;

	return *this;
}

DateTime& DateTime::ParseDays(int days)
{
	// �������һ�첻һ���������죬��Ҫƫ��BASE_YEAR_DAYOFWEEK_SHIFT
	//DayOfWeek = (days + BASE_YEAR_DAYOFWEEK_SHIFT) % 7;
	Year = (short)(days / 365);
	if (Year < 136) Year += BASE_YEAR;

	// ����Сÿ��365����㣬���������������������ݼ�һ
	int ytd = YEARS_TO_DAYS(Year);
	if (ytd > days)
	{
		Year--;
		ytd = YEARS_TO_DAYS(Year);
	}

	// ��ȥ��ݵ�����
	days -= ytd;

	// �����ÿ��31����㣬��������������������·ݼ�һ
	Month = (byte)(days / 31 + 1);
	int mtd = MONTH_TO_DAYS(Year, Month + 1);
	if (days >= mtd) Month++;

	// �����·ݱ�ʾ������
	mtd = MONTH_TO_DAYS(Year, Month);

	// ������������ȥ�·��������õ����µڼ���
	Day = (byte)(days - mtd + 1);

	return *this;
}

Buffer DateTime::ToArray()
{
	//return Buffer(&Year, (int)&Ms - (int)&Year + sizeof(Ms));
	return Buffer(this, sizeof(this[0]));
}

const Buffer DateTime::ToArray() const
{
	//return Buffer((void*)&Year, (int)&Ms - (int)&Year + sizeof(Ms));
	return Buffer((void*)this, sizeof(this[0]));
}

// ���صȺ������
DateTime& DateTime::operator=(int seconds)
{
	Parse(seconds);

	return *this;
}

DateTime& DateTime::operator=(const DateTime& value)
{
	// �����Լ������Լ�
	if (this != &value)
	{
		// �����Է�����
		ToArray() = value.ToArray();
	}

	return *this;
}

int DateTime::TotalDays() const
{
	return YEARS_TO_DAYS(Year) + MONTH_TO_DAYS(Year, Month) + Day - 1;
}

int DateTime::TotalSeconds() const
{
	int s = TotalDays() * 24 + Hour;
	s = s * 60 + Minute;
	s = s * 60 + Second;

	return s;
}

Int64 DateTime::TotalMs() const
{
	Int64 sec = (Int64)TotalSeconds();

	return sec * 1000LL + (Int64)Ms;
}

// ��ȡ����
byte DateTime::DayOfWeek() const
{
	int days = YEARS_TO_DAYS(Year) + MONTH_TO_DAYS(Year, Month) + Day - 1;
	return (days + BASE_YEAR_DAYOFWEEK_SHIFT) % 7;
}

// ȡʱ�����ڵ����ڲ���
DateTime DateTime::Date() const
{
	return DateTime(Year, Month, Day);
}

DateTime DateTime::AddYears(int value) const
{
	auto dt = *this;
	dt.Year += value;

	return dt;
}

DateTime DateTime::AddMonths(int value) const
{
	value += Month;
	int ys = value / 12;
	int ms = value % 12;

	// ���������������Ҫ����
	if (ms <= 0)
	{
		ys--;
		ms += 12;
	}

	auto dt = *this;
	dt.Year += ys;
	dt.Month = ms;

	return dt;
}

DateTime DateTime::AddDays(int value) const
{
	auto dt = *this;
	dt.ParseDays(TotalDays() + value);

	return dt;
}

DateTime DateTime::AddHours(int value) const
{
	return AddSeconds(value * 3600);
}

DateTime DateTime::AddMinutes(int value) const
{
	return AddSeconds(value * 60);
}

DateTime DateTime::AddSeconds(int value) const
{
	auto dt = *this;
	dt.Parse(TotalSeconds() + value);

	return dt;
}

DateTime DateTime::AddMilliseconds(Int64 value) const
{
	DateTime dt;
	dt.ParseMs(TotalMs() + value);

	return dt;
}

DateTime DateTime::Add(const TimeSpan& value) const { return AddMilliseconds(value.TotalMs()); }
DateTime DateTime::operator+(const TimeSpan& value) { return AddMilliseconds(value.TotalMs()); }
DateTime DateTime::operator-(const TimeSpan& value) { return AddMilliseconds(-value.TotalMs()); }

TimeSpan operator-(const DateTime& left, const DateTime& right)
{
	Int64 ms = left.TotalMs() - right.TotalMs();

	return TimeSpan(ms);
}

int DateTime::CompareTo(const DateTime& value) const
{
	int n = (int)Year - value.Year;
	if (n) return n;

	n = (int)Month - value.Month;
	if (n) return n;

	n = (int)Day - value.Day;
	if (n) return n;

	n = (int)Hour - value.Hour;
	if (n) return n;

	n = (int)Minute - value.Minute;
	if (n) return n;

	n = (int)Second - value.Second;
	if (n) return n;

	n = (int)Ms - value.Ms;
	if (n) return n;

	return 0;
}

bool operator==	(const DateTime& left, const DateTime& right) { return left.CompareTo(right) == 0; }
bool operator!=	(const DateTime& left, const DateTime& right) { return left.CompareTo(right) != 0; }
bool operator>	(const DateTime& left, const DateTime& right) { return left.CompareTo(right) > 0; }
bool operator<	(const DateTime& left, const DateTime& right) { return left.CompareTo(right) < 0; }
bool operator>=	(const DateTime& left, const DateTime& right) { return left.CompareTo(right) >= 0; }
bool operator<=	(const DateTime& left, const DateTime& right) { return left.CompareTo(right) <= 0; }

String DateTime::ToString() const
{
	// F��ȫ�� yyyy-MM-dd HH:mm:ss
	String str;
	if (Year < 10) str += '0';
	if (Year < 100) str += '0';
	if (Year < 1000) str += '0';
	str = str + Year + '-';

	if (Month < 10) str += '0';
	str = str + Month + '-';

	if (Day < 10) str += '0';
	str = str + Day + ' ';

	if (Hour < 10) str += '0';
	str = str + Hour + ':';

	if (Minute < 10) str += '0';
	str = str + Minute + ':';

	if (Second < 10) str += '0';
	str = str + Second;

	return str;
}

void DateTime::Show(bool newLine) const
{
	ToString().Show(newLine);
}

// Ĭ�ϸ�ʽ��ʱ��Ϊyyyy-MM-dd HH:mm:ss
/*
	d������ M/d/yy
	D������ yyyy-MM-dd
	t��ʱ�� mm:ss
	T��ʱ�� HH:mm:ss
	f��ȫ�� M/d/yy HH:mm
	F��ȫ�� yyyy-MM-dd HH:mm:ss
*/
/*cstring DateTime::GetString(byte kind, char* str)
{
	auto& st = *this;
	switch(kind)
	{
		case 'd':
			sprintf(str, "%d/%d/%02d", st.Month, st.Day, st.Year % 100);
			break;
		case 'D':
			sprintf(str, "%04d-%02d-%02d", st.Year, st.Month, st.Day);
			break;
		case 't':
			sprintf(str, "%02d:%02d", st.Hour, st.Minute);
			break;
		case 'T':
			sprintf(str, "%02d:%02d:%02d", st.Hour, st.Minute, st.Second);
			break;
		case 'f':
			sprintf(str, "%d/%d/%02d %02d:%02d", st.Month, st.Day, st.Year % 100, st.Hour, st.Minute);
			break;
		case 'F':
			sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d", st.Year, st.Month, st.Day, st.Hour, st.Minute, st.Second);
			break;
		default:
			//assert_param(false);
			break;
	}

	return str;
}*/

// ��ǰʱ��
//DateTime DateTime::Now()
//{
//	DateTime dt((int)time(NULL));
//
//	return dt;
//}
