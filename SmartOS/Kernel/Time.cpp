#include "TTime.h"
#include "OnChip\Configuration.h"


TTime::TTime()
{
	this->BaseSeconds = 0;
	this->Seconds = 0;

	this->Index = 0;
	this->Milliseconds = 0;

#if ! (defined(STM32F0) || defined(GD32F150))
	this->Div = 0;			// ��Ƶϵ��������Ƶ64k���޷��ô���64M��Ƶ��оƬ����õ�1kʱ��
#endif
}
/////////////////////////////////////////////////////////////////////////////////////
TimeCost::TimeCost()
{
	this->Reset();
}

void TimeCost::Reset()
{
	this->Start = millis();
	this->StartTicks = Time.CurrentTicks();
}
// ��ȥ��ʱ�䣬΢��
int TimeCost::Elapsed()const
{
	int ticks = Time.CurrentTicks() - this->StartTicks;
	int times = millis() - this->Start;
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


//��ʱ����
void delay(UInt64 ms)
{
	Sys.Sleep112233(ms);
}
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
//ϵͳ������ĺ�����
UInt64 millis()
{
	return Time.Milliseconds + DeviceConfigCenter::CurrentTick();
}
