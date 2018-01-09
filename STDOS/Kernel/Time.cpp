#include "TTime.h"
#include "Device\RTC.h"
#include "Device\Timer.h"


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
// 微秒级延迟
void TTime::Delay(int nus)const
{
	int sumus;
	UInt64 summs;
	uint startTicks;
	uint stopTicks;
	static uint ticksPerMs=0;
	sumus=nus;
	if(nus < 800)
	{
		this->DelayUs(nus);
	}
	else if(nus > 0)
	{
		if(nus > 100)
			sumus = nus-1;
		summs = this->Current();
		if(sumus >= 1000)
		{
			summs+=sumus/1000;
			sumus%=1000;
		}
		startTicks = this->CurrentTicks();
		stopTicks = startTicks+this->UsToTicks(sumus);
		if(ticksPerMs <10)
		ticksPerMs = this->UsToTicks(1000);
		if(stopTicks >= ticksPerMs)
		{
			++summs;
			stopTicks -= ticksPerMs;
		}
		while(true)
		{
			if(this->Current() > summs)
				break;
			if(this->Current() == summs)				
			{
				if(this->CurrentTicks() >= stopTicks)
					break;
			}
		}		
	}
	else{}
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
			ret = 1000 * times - Time.TicksToUs(-ticks);
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
    if(ret<0)
		ret=0;
	return ret;
}

void TimeCost::Show(cstring format)const
{
    if (!format)
        format = "执行 %d 微妙\r\n";
    int us = this->Elapsed();
    debug_printf(format, us);
}
