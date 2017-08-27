#include "RTC.h"
#include "TTime.h"

HardRTC::HardRTC()
{
    this->LowPower = true;
    this->External = false;
}

HardRTC *HardRTC::Instance()
{
    return new HardRTC();
}

void HardRTC::Start(bool lowpower, bool external)
{
    lowpower = lowpower;
    external = external;
}

//测试时钟
#if 0
    DateTime now; //当前时间

    void TimeRefresh(void *param)
    {
        HardRTC *rtc = (HardRTC*)param;
        rtc->LoadTime();
        now = Time.BaseSeconds + Time.Seconds;
        now.Show();
        debug_printf("  BaseSeconds:%d Seconds:%d\r\n", Time.BaseSeconds, Time.Seconds);
    }
    void RTCtest()
    {
        HardRTC *rtc = HardRTC::Instance();
        rtc->LowPower = false;
        rtc->External = false;

        rtc->LoadTime();
        Time.BaseSeconds = Time.Seconds + Time.BaseSeconds;
        now = Time.BaseSeconds + Time.Seconds;
        if (now.TotalSeconds() < 100)
        {
            now.Year = 2017;
            now.Month = 6;
            now.Day = 17;
            now.Hour = 14;
            now.Minute = 17;

            rtc->Init(); //设置时间时初始化，否则错乱
            Time.BaseSeconds = now.TotalSeconds() - Time.Seconds;
            rtc->LoadTime();
            rtc->SaveTime();
        }
        Sys.AddTask(TimeRefresh, rtc, 100, 1000, "TimeUp");
    }
#endif 
