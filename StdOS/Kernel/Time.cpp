#include "TTime.h"
#include "OnChip\Configuration.h"

TTime Time; //ϵͳʱ�䣬�������û�ֱ��ʹ��
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
	if (!format)
		format = "ִ�� %d ΢��\n";
	int us = this->Elapsed();
	debug_printf(format, us);
}

void TimeUpdate()
{
	Time.Milliseconds += 1000;
	Time.Seconds++;	
}
int gTicks = 0; //ÿ��us��Ҫ��systickʱ���� 	

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
	// ��ʼ��Ϊ���
	this->Index = 2;

	gTicks = (Sys.Clock >> 3) / 0xF4240u;
	//��ʼ���ӳٺ���
	//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
	//SYSCLK:ϵͳʱ��
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //ѡ���ⲿʱ��  HCLK/8
	SysTick_Config(9000); //����SysTick tick is 9ms	9000
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

	SysTick_Config(9000); //����SysTick tick is 9ms	9000
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //�ر�jtag������swd	
#elif defined STM32F4	
		// ��ʼ��Ϊ���
		this->Index = 2;
		gTicks = (Sys.Clock >> 3) / 0xF4240u;		
		//��ʼ���ӳٺ���
		//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
		//SYSCLK:ϵͳʱ��
	//        uint32_t SYSCLK = 168;
		SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
		SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //ѡ���ⲿʱ��  HCLK/8
		SysTick_Config(9000); //����SysTick tick is 9ms	9000
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

	// ��ǰ�δ�ʱ��
	uint32_t TTime::CurrentTicks()const
	{
		return DeviceConfigCenter::CurrentTicks1();
	}

	//��ʱ����
	void delay(uint64_t ms)
	{
		Sys.Sleep112233(ms);
	}
	//us��ʱ��100us���¾�ȷ
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
	//ϵͳ������ĺ�����
	uint64_t millis()
	{
		return Time.Milliseconds + DeviceConfigCenter::CurrentTick();
	}
	//���������ڵ�΢����
	uint64_t micros()
	{
		return millis() *1000;
	}

