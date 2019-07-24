#ifndef __TimeSpan_H__
#define __TimeSpan_H__

// ʱ����
class TimeSpan
{
public:
	TimeSpan(Int64 ms = 0);
	TimeSpan(int hours, int minutes, int seconds);
	TimeSpan(int days, int hours, int minutes, int seconds);

	int Days() const;
	int Hours() const;
	int Minutes() const;
	int Seconds() const;
	int Ms() const;
	int TotalDays() const;
	int TotalHours() const;
	int TotalMinutes() const;
	int TotalSeconds() const;
	Int64 TotalMs() const;

	int CompareTo(const TimeSpan& value) const;
	friend bool operator==	(const TimeSpan& left, const TimeSpan& right);
	friend bool operator!=	(const TimeSpan& left, const TimeSpan& right);
	friend bool operator>	(const TimeSpan& left, const TimeSpan& right);
	friend bool operator<	(const TimeSpan& left, const TimeSpan& right);
	friend bool operator>=	(const TimeSpan& left, const TimeSpan& right);
	friend bool operator<=	(const TimeSpan& left, const TimeSpan& right);

	String ToString() const;
	void Show(bool newLine = false) const;

private:
	int	_Seconds;
	int	_Ms;
};

/*
�ֿ��洢�����ͺ��������������ʱ��ֻ��Ҫ�����������㣬��������64λ�������㣬����Ч�ʡ�
*/

#endif
