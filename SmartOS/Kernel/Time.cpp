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
// ��ȥ��ʱ�䣬΢��
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
	if (!format) format = "ִ�� %dus\r\n";
	debug_printf(format, Elapsed());
}

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
