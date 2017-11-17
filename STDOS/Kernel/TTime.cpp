#include "TTime.h"
#include "Device\RTC.h"
#include "Device\Timer.h"

extern byte fac_us; //us延时倍乘数 每个us需要的systick时钟数 	
int clock()
{
  return  Time.Current();
}

TTime Time; //系统时间，不建议用户直接使用

TTime::TTime()
{
    this->BaseSeconds = 0;
    this->Seconds = 0;
}

void RtcRefresh(void *param)
{
    HardRTC *rtc = (HardRTC*)param;
    rtc->LoadTime();
}

// 使用RTC，必须在Init前调用
void TTime::UseRTC()
{
    HardRTC *rtc = HardRTC::Instance();
    rtc->LowPower = false;
    rtc->External = false;

    rtc->Init();
    Sys.AddTask(RtcRefresh, rtc, 100, 100, "Rtc");
}

// 设置时间
void TTime::SetTime(UInt64 seconds)
{
}
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void TTime::Sleep(int nms, bool *running)const
{
    this->Delay(nms *1000);
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
TimeWheel::TimeWheel(uint ms)
{
	this->Sleep=0;
	this->Reset(ms);
}

void TimeWheel::Reset(uint ms)
{
	this->Expire=Time.Current()+ms;
}
// 是否已过期
bool TimeWheel::Expired()
{
    if (Time.Current() < this->Expire)
    {
        if (this->Sleep)
            Sys.Sleep(this->Sleep);
        return false;
    }
    else
        return true;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
TimeCost::TimeCost()
{
    this->Reset();	
}

void TimeCost::Reset()
{
	this->Start=Time.Current();
	this->StartTicks=Time.CurrentTicks();
}
// 逝去的时间，微秒
int TimeCost::Elapsed()const
{
	int ticks=Time.CurrentTicks()-this->StartTicks;
	int times=Time.Current()-this->Start;
	int ret=0;
	
	if(ticks<=0)
	{
		if(times>0)
		{
			ret = 1000 * times - Time.TicksToUs(Time.CurrentTicks()-ticks);
		}
		else
		{
			ret=0;
		}
	}
	else
	{	
		ret=Time.TicksToUs(ticks)+1000*times;
	}	
    
	return ret;
}

void TimeCost::Show(cstring format)const
{
    if (!format)
        format = "执行 %d 微妙\r\n";
    int us = this->Elapsed();
    SmartOS_printf(format, us);
}
