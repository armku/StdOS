#pragma once

#include "stm32f10x.h"

struct rtc_time
{
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
};

void GregorianDay(struct rtc_time *tm);
uint32_t mktimev(struct rtc_time *tm);
