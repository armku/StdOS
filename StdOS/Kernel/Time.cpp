#include "TTime.h"
#include "OnChip\Configuration.h"

TTime Time; //系统时间，不建议用户直接使用
TTime::TTime()
{
	this->BaseSeconds = 0;
	this->Seconds = 0;
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
// 逝去的时间，微秒
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

	//延时毫秒
	void delay(uint64_t ms)
	{
		Sys.Sleep112233(ms);
	}
	//us延时，100us以下精确
	void delayMicroseconds(uint32_t us)
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
	//系统启动后的毫秒数
	uint64_t millis()
	{
		return Time.Milliseconds + DeviceConfigCenter::CurrentTick();
	}
	//开机到现在的微妙数
	uint64_t micros()
	{
		return millis() *1000;
	}

