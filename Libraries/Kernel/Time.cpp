#include "TTime.h"
#include "OnChip\Configuration.h"

TTime Time; //系统时间，不建议用户直接使用
TTime::TTime()
{
	this->BaseSeconds = 0;
	this->Seconds = 0;
}
//延时nms
void TTime::Sleep(int nms, bool *running)const
{
	if (nms > 0)
	{
		int64_t endcurrent = Time.Current() + nms;

		if (nms)
		{
			if (!running || *running != 0)
			{
				while (1)
				{
					if (((endcurrent + Time.Current()) >= endcurrent))
						break;
					if (running)
					{
						if (!*running)
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
	static uint32_t ticksPerMs = 0;
	sumus = nus;
	if (nus < 800)
	{
		this->DelayUs(nus);
	}
	else if (nus > 0)
	{
		if (nus > 100)
			sumus = nus - 1;
		summs = this->Current();
		if (sumus >= 1000)
		{
			summs += sumus / 1000;
			sumus %= 1000;
		}
		startTicks = this->CurrentTicks();
		stopTicks = startTicks + this->UsToTicks(sumus);
		if (ticksPerMs < 10)
			ticksPerMs = this->UsToTicks(1000);
		if (stopTicks >= ticksPerMs)
		{
			++summs;
			stopTicks -= ticksPerMs;
		}
		while (true)
		{
			if (this->Current() > summs)
				break;
			if (this->Current() == summs)
			{
				if (this->CurrentTicks() >= stopTicks)
					break;
			}
		}
	}
	else {}
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
TimeCost::TimeCost()
{
	this->Reset();
}

void TimeCost::Reset()
{
	this->Start = Time.Current();
	this->StartTicks = Time.CurrentTicks();
}
// 逝去的时间，微秒
int TimeCost::Elapsed()const
{
	int ticks = Time.CurrentTicks() - this->StartTicks;
	int times = Time.Current() - this->Start;
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
	if (!format)
		format = "执行 %d 微妙\n";
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
#include "Platform\stm32.h"
#include "BspPlatform\BspPlatform.h"

extern int gTicks; //
void TTime::Init()
{
#if defined STM32F0	
	// 初始化为输出
	this->Index = 2;

	gTicks = (Sys.Clock >> 3) / 0xF4240u;
	//初始化延迟函数
	//SYSTICK的时钟固定为HCLK时钟的1/8
	//SYSCLK:系统时钟
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //选择外部时钟  HCLK/8
	SysTick_Config(9000); //配置SysTick tick is 9ms	9000
	NVIC_SetPriority(SysTick_IRQn, 0);
	switch (this->Index)
	{
	case 2:
		NVIC_SetPriority(TIM2_IRQn, 0);
		break;
	case 5:
		break;
	case 6:
		//NVIC_SetPriority(TIM7_IRQn, 0);
		break;
	default:
		break;
	}
#elif defined STM32F1
	this->Index = 5;
	gTicks = (Sys.Clock >> 3) / 0xF4240u;

	SysTick_Config(9000); //配置SysTick tick is 9ms	9000
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //关闭jtag，保留swd	
#elif defined STM32F4	
		// 初始化为输出
		this->Index = 2;
		gTicks = (Sys.Clock >> 3) / 0xF4240u;		
		//初始化延迟函数
		//SYSTICK的时钟固定为HCLK时钟的1/8
		//SYSCLK:系统时钟
	//        uint32_t SYSCLK = 168;
		SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
		SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //选择外部时钟  HCLK/8
		SysTick_Config(9000); //配置SysTick tick is 9ms	9000
		NVIC_SetPriority(SysTick_IRQn, 0);
		switch (this->Index)
		{
		case 2:
			NVIC_SetPriority(TIM3_IRQn, 0);
			break;
		case 5:
			break;
		case 6:
			NVIC_SetPriority(TIM7_IRQn, 0);
			break;
		default:
			break;
		}
#endif
}

	// 当前滴答时钟
	uint32_t TTime::CurrentTicks()const
	{
		return DeviceConfigCenter::CurrentTicks1();
	}

	// 当前毫秒数 计数值2000
	uint64_t TTime::Current()const
	{
		return this->Milliseconds + DeviceConfigCenter::CurrentTick();
	}

	//us延时，100us以下精确
	void TTime::DelayUs(int nus) const
	{
#if defined STM32F0
		/*
		F072 48MHz	750us->774us
		*/
		for (int i = 0; i < nus; i++)
		{
			for (int j = 0; j < 7; j++);
		}
#elif defined STM32F1
		/*
		F103 72MHz  750us->753us
		*/
		for (int i = 0; i < nus; i++)
		{
			for (int j = 0; j < 10; j++);
		}
#elif defined STM32F4
		/*
		F407 168MHz 750us->759us
		*/
		for (int i = 0; i < nus; i++)
		{
			for (int j = 0; j < 40; j++);
		}
#endif
	}   
