#include "../src/DS1302.h"

//#define _DS1302_TEST_CPP
#ifdef _DS1302_TEST_CPP
DateTime now; //当前时间
DS1302 ds1302;
//更新时间
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
	Sys.AddTask(rtcUpdate, &now, 100, 100, "时间更新");
}
#endif // _DS1302_TEST_
