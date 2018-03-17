#include "TTime.h"
#include "Device\Timer.h"

TTime Time; //ϵͳʱ�䣬�������û�ֱ��ʹ��

TTime::TTime()
{
    this->BaseSeconds = 0;
    this->Seconds = 0;
}
//��ʱnms
void TTime::Sleep(int nms, bool *running)const
{
	if( nms > 0 )
	{
		Int64 endcurrent = Time.Current() + nms;
		if(this->OnSleep && nms >= 10 )
		{
			while(nms >= 10)
			{
				this->OnSleep(nms);
				if(running)
				{
					if(!*running)
						break;
				}
				nms = endcurrent-Time.Current();
			}
		}
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
// ΢�뼶�ӳ�
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
// �Ƿ��ѹ���
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
// ��ȥ��ʱ�䣬΢��
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
        format = "ִ�� %d ΢��\r\n";
    int us = this->Elapsed();
    debug_printf(format, us);
}

Timer *timerTick;
Delegate < Timer & > abc;
void timTickrefesh(void *param)
{
	Time.Milliseconds += 1000;
	Time.Seconds++;
}
int gTicks = 0; //ÿ��us��Ҫ��systickʱ���� 	

uint TTime::TicksToUs(uint ticks)const
{
	uint result;

	if (ticks)
		result = ticks / gTicks;
	else
		result = 0;
	return result;

}

uint TTime::UsToTicks(uint us)const
{
	uint result;

	if (us)
		result = us * gTicks;
	else
		result = 0;
	return result;
}

