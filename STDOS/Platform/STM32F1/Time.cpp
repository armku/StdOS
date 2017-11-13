/*
���ļ���Ҫ�Ż���
*/
#include "TTime.h"
#include "Device\RTC.h"
#include "Device\Timer.h"

#ifdef STM32F0
    #include "stm32f0xx.h"
#elif defined STM32F1
    #include "stm32f10x.h"
#elif defined STM32F4
    #include "stm32f4xx.h"
#endif 

extern Timer *timerTick;
extern Delegate < Timer & > abc;
extern int gTicks; //
extern byte fac_us; //us��ʱ������ ÿ��us��Ҫ��systickʱ���� 	
void timTickrefesh(void *param);
void TTime::Init()
{

    // ��ʼ��Ϊ���
    #if defined STM32F0
        this->Index = 6;
    #elif defined STM32F1
        this->Index = 5;
    #elif defined STM32F4
        this->Index = 2;
    #endif 
	
	
	gTicks = (Sys.Clock >> 3) / 0xF4240u;
	
    timerTick = new Timer((TIMER)this->Index);
    abc.Bind(timTickrefesh);
    timerTick->Register(abc);
    timerTick->Open();
    //timerTick->SetFrequency(1000);
    //        timer2->Config();

    //��ʼ���ӳٺ���
    //SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
    //SYSCLK:ϵͳʱ��
    #if defined STM32F0
    #elif defined STM32F1
        SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //ѡ���ⲿʱ��  HCLK/8
        fac_us = SystemCoreClock / 8000000; //Ϊϵͳʱ�ӵ�1/8  
//        fac_ms = (ushort)fac_us *1000; //����ÿ��ms��Ҫ��systickʱ����   
    #elif defined STM32F4
        uint SYSCLK = 168;
        SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
        fac_us = SYSCLK / 8; //�����Ƿ�ʹ��OS,fac_us����Ҫʹ��

//        fac_ms = (u16)fac_us *1000; //��OS��,����ÿ��ms��Ҫ��systickʱ����   
    #endif 

    #ifdef STM32F0
        fac_us = SystemCoreClock / 8000000 * 8; //Ϊϵͳʱ�ӵ�1/8 //��OS��,����ÿ��us��Ҫ��systickʱ���� 
    #elif defined STM32F1
        fac_us = SystemCoreClock / 8000000 * 8; //Ϊϵͳʱ�ӵ�1/8 //��OS��,����ÿ��us��Ҫ��systickʱ���� 
    #elif defined STM32F4
        fac_us = SystemCoreClock / 8000000 * 8; //Ϊϵͳʱ�ӵ�1/8 //��OS��,����ÿ��us��Ҫ��systickʱ���� 
    #endif 
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //ѡ���ⲿʱ��  HCLK/8
    SysTick_Config(9000); //����SysTick tick is 9ms	9000
    #ifdef STM32F0
    #elif defined STM32F1
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //�ر�jtag������swd	
    #elif defined STM32F4
    #endif 
    NVIC_SetPriority(SysTick_IRQn, 0);
	switch(this->Index)
	{
		case 2:
			NVIC_SetPriority(TIM3_IRQn, 0);
			break;
		case 5:
			#if defined STM32F0
			#elif defined STM32F1
				NVIC_SetPriority(TIM6_IRQn, 0);
			#elif defined STM32F4
			#endif
			break;
		case 6:
			NVIC_SetPriority(TIM7_IRQn, 0);
			break;
		default:
			break;
	}
}
extern uint systickcnt;
// ��ǰ�δ�ʱ��
uint TTime::CurrentTicks()const
{
	SmartIRQ irq;

    uint value = (SysTick->LOAD - SysTick->VAL);
	#if defined STM32F0
	#elif defined STM32F1
    if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG)
    {
        systickcnt += SysTick->LOAD;
    }
	#elif defined STM32F4
	#endif
	
    return systickcnt + value;
}

// ��ǰ������
UInt64 TTime::Current()const
{
    __IO ushort ms = 0;
    switch (this->Index)
    {
		case 2:
			ms=(TIM3->CNT) >> 1;
			break;
        case 5:
            ms = (TIM6->CNT) >> 1;
            break;
        case 6:
            ms = (TIM7->CNT) >> 1;
            break;
        default:
            break;
    }
    return this->Milliseconds + ms;
}
