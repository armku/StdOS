#include "TTime.h"
#include "Device\RTC.h"
#include "Device\Timer.h"

extern byte fac_us; //us��ʱ������ ÿ��us��Ҫ��systickʱ���� 	
int clock()
{
  return  Time.Current();
}

TTime Time; //ϵͳʱ�䣬�������û�ֱ��ʹ��

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

// ʹ��RTC��������Initǰ����
void TTime::UseRTC()
{
    HardRTC *rtc = HardRTC::Instance();
    rtc->LowPower = false;
    rtc->External = false;

    rtc->Init();
    Sys.AddTask(RtcRefresh, rtc, 100, 100, "Rtc");
}

// ����ʱ��
void TTime::SetTime(UInt64 seconds)
{
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
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
        format = "ִ�� %d ΢��\r\n";
    int us = this->Elapsed();
    SmartOS_printf(format, us);
}
