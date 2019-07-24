#include "TTime.h"

#include "Core\Environment.h"

#if defined(__CC_ARM)
#include <time.h>
#else
#include <ctime>
#endif

#define TIME_DEBUG 0

#define UTC		true								// utc ��1970/1/1 ��ʼ��ʱ
#define UTC_CALIBRATE				946684800UL		// 2000/1/1 - 1970/1/1 ��ֵ
#ifdef UTC
#define BASE_YEAR_US				62135596800UL		// (63082281600UL-UTC_CALIBRATE)	// ��0 �� 2000-01-01����������
#else
#define BASE_YEAR_US				63082281600UL	// ��0 �� 2000-01-01����������
#endif
/************************************************ TTime ************************************************/
TTime::TTime()
{
	Seconds = 0;
	//Ticks	= 0;
#if defined(STM32F0) || defined(GD32F150)
	Index = 13;
#else
	Div = 0;
	if (Sys.FlashSize > 0x80)
		Index = 5;
	else
		Index = 1;	// �����ص� TIM3 ����
#endif
	BaseSeconds = 0;

	OnInit = nullptr;
	OnLoad = nullptr;
	OnSave = nullptr;
	OnSleep = nullptr;
}
void TTime::SetTime(UInt64 sec)
{
	if (sec >= BASE_YEAR_US) sec -= BASE_YEAR_US;

	BaseSeconds = (int)(sec - Seconds);

#if DEBUG
	/*DateTime dt(sec);
	debug_printf("TTime::SetTime ����ʱ�� ");
	dt.Show(true);*/
#endif

	// ���浽RTC
	if (OnSave) OnSave();
}


// �ؼ��Դ��룬�ŵ���ͷ
INROOT void TTime::Sleep(int ms, bool* running) const
{
	// ˯��ʱ��̫��
	if (ms <= 0) return;

	// ����ʱ��
	UInt64 end = Current() + ms;

	// �ϴ��˯��ʱ��ֱ����CPUͣ����
	if (OnSleep && ms >= 10)
	{
		while (ms >= 10)
		{
			OnSleep(ms);

			// �ж��Ƿ���Ҫ����
			if (running != nullptr && !*running) break;

			// ���¼���ʣ�µ�ʱ��
			ms = (int)(end - Current());
		}
	}
	// ˯��ʱ��̫��
	if (!ms || (running && !*running)) return;

	// ��ת
	while (true)
	{
		if (Current() >= end) break;
		if (running != nullptr && !*running) break;
	}
}

INROOT void TTime::Delay(int us) const
{
	// ˯��ʱ��̫��
	if (us <= 0) return;

	// ��ǰ������ʱ1~3us
	if (us > 100) us -= 1;

	UInt64 end = Current();
	if (us >= 1000)
	{
		// ��ֺ����΢��
		int ms = us / 1000;
		end += ms;

		us %= 1000;
	}

	// ΢��תΪ�δ�
	uint ticks = CurrentTicks() + UsToTicks(us);
	// ����������п��ܳ���1000
	uint max = UsToTicks(1000 - 1);
	if (ticks >= max)
	{
		end++;
		ticks -= max;
	}

	// �ȴ�Ŀ��ʱ��
	while (true)
	{
		// ���ȱȽϺ�����
		int n = (int)(Current() - end);
		if (n > 0) break;

		if (n == 0 && CurrentTicks() >= ticks) break;
	}
}

extern "C"
{
#ifndef _MSC_VER
	// ��ȡϵͳ�����󾭹��ĺ�����
	clock_t clock(void)
	{
		return Time.Current();
	}

	// ʵ��C�������time����
	time_t time(time_t* sec)
	{
		uint s = Time.BaseSeconds + Time.Seconds;
		if (sec)* sec = s;

		return s;
	}
#endif
}

/************************************************ TimeWheel ************************************************/

TimeWheel::TimeWheel(uint ms)
{
	Sleep = 0;
	Reset(ms);
}

void TimeWheel::Reset(uint ms)
{
	Expire = Time.Current() + ms;
}

// �Ƿ��ѹ���
bool TimeWheel::Expired()
{
	UInt64 now = Time.Current();
	if (now > Expire) return true;

	// ˯�ߣ��ͷ�CPU
	if (Sleep) Sys.Sleep(Sleep);

	return false;
}

/************************************************ TimeCost ************************************************/

TimeCost::TimeCost()
{
	Reset();
}

void TimeCost::Reset()
{
	Start = Time.Current();
	StartTicks = Time.CurrentTicks();
}

// ��ȥ��ʱ�䣬΢��
int TimeCost::Elapsed() const
{
	int ts = (int)(Time.CurrentTicks() - StartTicks);
	int ms = (int)(Time.Current() - Start);

	// �п��ܵδ𲿷ֲ���������һȦ
	if (ts > 0) return ms * 1000 + Time.TicksToUs(ts);

	// ������벿��Ҳû�У���ô������΢С����ƫ��
	if (ms <= 0) return 0;

	// ����δ��Ǹ�������ɴ��ȥ
	return ms * 1000 - Time.TicksToUs(-ts);
}

void TimeCost::Show(cstring format) const
{
	if (!format) format = "ִ�� %dus\r\n";
	debug_printf(format, Elapsed());
}

























#include "OnChip\Configuration.h"
/////////////////////////////////////////////////////////////////////////////////////

void TimeUpdate()
{
	Time.Milliseconds += 1000;
	Time.Seconds++;
}

#include "Platform\stm32.h"
#include "BspPlatform\BspPlatform.h"


//us��ʱ��100us���¾�ȷ
void delayMicroseconds(uint us)
{
#if defined STM32F0
	/*
	F072 48MHz	750us->774us
	*/
	for (int i = 0; i < us; i++)
	{
		for (int j = 0; j < 7; j++);
	}
#elif defined STM32F1
	/*
	F103 72MHz  750us->753us
	*/
	for (int i = 0; i < us; i++)
	{
		for (int j = 0; j < 10; j++);
	}
#elif defined STM32F4
	/*
	F407 168MHz 750us->759us
	*/
	for (int i = 0; i < us; i++)
	{
		for (int j = 0; j < 40; j++);
	}
#endif
}
