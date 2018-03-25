#include "Kernel\Sys.h"

//#define _DATETIME_TEST_H
#ifdef _DATETIME_TEST_H
static void TestCtor()
{
	TS("TestCtor");

    DateTime dt;
	assert(dt.Year == 1970 && dt.Month == 1 && dt.Day == 1, "DateTime()");
	assert(dt.DayOfWeek() == 4, "DateTime()");
	assert(dt.Hour == 0 && dt.Minute == 0 && dt.Second == 0 && dt.Ms == 0, "DateTime()");

    DateTime dt2(2016, 5, 18);
	assert(dt2.Year == 2016 && dt2.Month == 5 && dt2.Day == 18, "DateTime(uint16_t year, uint8_t month, uint8_t day)");
	assert(dt2.DayOfWeek() == 3, "DateTime(uint16_t year, uint8_t month, uint8_t day)");
	assert(dt2.Hour == 0 && dt2.Minute == 0 && dt2.Second == 0 && dt2.Ms == 0, "DateTime(uint16_t year, uint8_t month, uint8_t day)");

    DateTime dt3(1463443233);
	assert(dt3.Year == 2016 && dt3.Month == 5 && dt3.Day == 17, "DateTime(uint32_t seconds)");
	assert(dt3.DayOfWeek() == 2, "DateTime(uint32_t seconds)");
	assert(dt3.Hour == 0 && dt3.Minute == 0 && dt3.Second == 33 && dt3.Ms == 00, "DateTime(uint32_t seconds)");
}

static void TestParseMs()
{
	TS("TestCtor");

    DateTime dt3;
	dt3	= 1463443233;
	assert(dt3.Year == 2016 && dt3.Month == 5 && dt3.Day == 17, "DateTime(uint32_t seconds)");
	assert(dt3.DayOfWeek() == 2, "DateTime(uint32_t seconds)");
	assert(dt3.Hour == 0 && dt3.Minute == 0 && dt3.Second == 33 && dt3.Ms == 00, "DateTime(uint32_t seconds)");
}

static void TestTotal()
{
	TS("TestTotal");

    DateTime dt	= 1463443233;
	assert(dt.TotalSeconds() == 1463443233, "TotalSeconds");
	assert(dt.TotalMs() == 1463443233000ULL, "TotalMs");
}

void DateTime::Test()
{
    debug_printf("\r\n");
    debug_printf("TestDateTime Start......\r\n");

	TestCtor();
	TestParseMs();
	TestTotal();

    debug_printf("\r\n TestDateTime Finish!\r\n");
}
#endif
