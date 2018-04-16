#include "TTime.h"

TTime Time; //系统时间，不建议用户直接使用

TTime::TTime()
{
    this->BaseSeconds = 0;
    this->Seconds = 0;
}
//延时nms
void TTime::Sleep(int nms, bool *running)const
{
	if( nms > 0 )
	{
		int64_t endcurrent = Time.Current() + nms;
		
		if(nms)
		{
			if(!running || *running !=0)
			{
				while(1)
				{					
					if(((endcurrent + Time.Current()) >= endcurrent))
						break;
					if(running)
					{
						if(!*running)
							break;
					}
				}
			}
		}		
	}
}
// 微秒级延迟
void TTime::Delay(int nus)const
{
	int sumus;
	uint64_t summs;
	uint32_t startTicks;
	uint32_t stopTicks;
	static uint32_t ticksPerMs=0;
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
TimeWheel::TimeWheel(uint32_t ms)
{
	this->Sleep=0;
	this->Reset(ms);
}

void TimeWheel::Reset(uint32_t ms)
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

void TimeUpdate()
{
	Time.Milliseconds += 1000;
	Time.Seconds++;	
}
int gTicks = 0; //每个us需要的systick时钟数 	

uint32_t TTime::TicksToUs(uint32_t ticks)const
{
	uint32_t result;

	if (ticks)
		result = ticks / gTicks;
	else
		result = 0;
	return result;

}

uint32_t TTime::UsToTicks(uint32_t us)const
{
	uint32_t result;

	if (us)
		result = us * gTicks;
	else
		result = 0;
	return result;
}
