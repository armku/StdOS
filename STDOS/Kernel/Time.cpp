#include "TTime.h"
#include "Device\Configuration.h"

TTime Time; //ϵͳʱ�䣬�������û�ֱ��ʹ��

TTime::TTime()
{
	this->BaseSeconds = 0;
	this->Seconds = 0;
}
//��ʱnms
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
// ΢�뼶�ӳ�
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
TimeWheel::TimeWheel(uint32_t ms)
{
	this->Sleep = 0;
	this->Reset(ms);
}

void TimeWheel::Reset(uint32_t ms)
{
	this->Expire = Time.Current() + ms;
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
	this->Start = Time.Current();
	this->StartTicks = Time.CurrentTicks();
}
// ��ȥ��ʱ�䣬΢��
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

	// ��ǰ������ ����ֵ2000
	uint64_t TTime::Current()const
	{
		return this->Milliseconds + DeviceConfigCenter::CurrentTick();
	}

	//us��ʱ��100us���¾�ȷ
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


#ifdef __cplusplus
	extern "C"
	{
#endif 

		//����Ϊ��ຯ��
		//THUMBָ�֧�ֻ������
		//�������·���ʵ��ִ�л��ָ��WFI  
		void WFI_SET(void)
		{
			__ASM volatile("wfi");
		}

		//����ջ����ַ
		//addr:ջ����ַ
		__asm void MSR_MSP(uint32_t addr)
		{
			MSR MSP, r0  //set Main Stack value
				BX r14
		}
#ifdef __cplusplus
	}
#endif
