#include "DS1302.h"

//#define _DS1302_TEST_CPP
#ifdef _DS1302_TEST_CPP
DateTime now; //��ǰʱ��
DS1302 ds1302;
//����ʱ��
void rtcUpdate(void *param)
{
	auto *tm = (DateTime*)param;

	ds1302.GetTime(*tm);
	now.Show();
}

void DS1302Test()
{
	ds1302.SetPin(PB8, PB9, PE0);
	ds1302.Init();

	ds1302.GetTime(now);
	if (now.Year < 2010)
	{
		now.Year = 2018;
		now.Month = 1;
		now.Day = 13;
		now.Hour = 14;
		now.Minute = 45;
		now.Second = 0;
		ds1302.SetTime(now);
	}
	Sys.AddTask(rtcUpdate, &now, 100, 100, "ʱ�����");
}
#endif // _DS1302_TEST_
