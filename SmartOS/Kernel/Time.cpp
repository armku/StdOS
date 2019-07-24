#include "TTime.h"

#include "Core\Environment.h"

#if defined(__CC_ARM)
#include <time.h>
#else
#include <ctime>
#endif

#define TIME_DEBUG 0

#define UTC		true								// utc 从1970/1/1 开始计时
#define UTC_CALIBRATE				946684800UL		// 2000/1/1 - 1970/1/1 秒值
#ifdef UTC
#define BASE_YEAR_US				62135596800UL		// (63082281600UL-UTC_CALIBRATE)	// 从0 到 2000-01-01的所有秒数
#else
#define BASE_YEAR_US				63082281600UL	// 从0 到 2000-01-01的所有秒数
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
		Index = 1;	// 错开开关的 TIM3 背光
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
	debug_printf("TTime::SetTime 设置时间 ");
	dt.Show(true);*/
#endif

	// 保存到RTC
	if (OnSave) OnSave();
}


// 关键性代码，放到开头
INROOT void TTime::Sleep(int ms, bool* running) const
{
	// 睡眠时间太短
	if (ms <= 0) return;

	// 结束时间
	UInt64 end = Current() + ms;

	// 较大的睡眠时间直接让CPU停下来
	if (OnSleep && ms >= 10)
	{
		while (ms >= 10)
		{
			OnSleep(ms);

			// 判断是否需要继续
			if (running != nullptr && !*running) break;

			// 重新计算剩下的时间
			ms = (int)(end - Current());
		}
	}
	// 睡眠时间太短
	if (!ms || (running && !*running)) return;

	// 空转
	while (true)
	{
		if (Current() >= end) break;
		if (running != nullptr && !*running) break;
	}
}

INROOT void TTime::Delay(int us) const
{
	// 睡眠时间太短
	if (us <= 0) return;

	// 当前函数耗时1~3us
	if (us > 100) us -= 1;

	UInt64 end = Current();
	if (us >= 1000)
	{
		// 拆分毫秒和微秒
		int ms = us / 1000;
		end += ms;

		us %= 1000;
	}

	// 微秒转为滴答
	uint ticks = CurrentTicks() + UsToTicks(us);
	// 结束嘀嗒数有可能超过1000
	uint max = UsToTicks(1000 - 1);
	if (ticks >= max)
	{
		end++;
		ticks -= max;
	}

	// 等待目标时间
	while (true)
	{
		// 首先比较毫秒数
		int n = (int)(Current() - end);
		if (n > 0) break;

		if (n == 0 && CurrentTicks() >= ticks) break;
	}
}

























#include "OnChip\Configuration.h"
/////////////////////////////////////////////////////////////////////////////////////
TimeCost::TimeCost()
{
	this->Reset();
}

void TimeCost::Reset()
{
	this->Start = Sys.Ms();
	this->StartTicks = Time.CurrentTicks();
}
// 逝去的时间，微秒
int TimeCost::Elapsed()const
{
	int ticks = Time.CurrentTicks() - this->StartTicks;
	int times = Sys.Ms() - this->Start;
	int ret = 0;

	if (ticks <= 0)
	{
		if (times > 0)
		{
			ret = 1000 * times - Time.TicksToUs(-ticks);
		}
		else
		{
			ret = 0;
		}
	}
	else
	{
		ret = Time.TicksToUs(ticks) + 1000 * times;
	}
	if (ret < 0)
		ret = 0;
	return ret;
}

void TimeCost::Show(cstring format)const
{
	if (!format) format = "执行 %dus\r\n";
	debug_printf(format, Elapsed());
}

void TimeUpdate()
{
	Time.Milliseconds += 1000;
	Time.Seconds++;
}

#include "Platform\stm32.h"
#include "BspPlatform\BspPlatform.h"


//us延时，100us以下精确
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
