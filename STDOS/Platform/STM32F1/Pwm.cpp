#include "Pwm.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif

void Pwm::Open()
{
	/*
	REMAP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
	*/
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	Timer::Open();
	
	/* PWMģʽ���� */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //ʹ�����
	TIM_OCInitStructure.TIM_Pulse = 0; //���ó�ʼPWM������Ϊ0	
	TIM_OCInitStructure.TIM_OCPolarity = this->Polarity?TIM_OCPolarity_Low:TIM_OCPolarity_High; //����ʱ������ֵС��CCR1_ValʱΪ�͵�ƽ

	switch (this->_index)
	{
		case Timer1:
            break;
        case Timer2:
            if(this->Enabled[0])
			{
				TIM_OC1Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��3
				TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
			}
			if(this->Enabled[1])
			{
				TIM_OC2Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��3
				TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
			}
			if(this->Enabled[2])
			{
				TIM_OC3Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��3
				TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
			}
			if(this->Enabled[3])
			{
				TIM_OC4Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��3
				TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
			}
			
			TIM_ARRPreloadConfig(TIM2, ENABLE); //ʹ��TIM3���ؼĴ���ARR
            break;
        case Timer3:
			if(this->Enabled[0])
			{
				TIM_OC1Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��3
				TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
			}
			if(this->Enabled[1])
			{
				TIM_OC2Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��3
				TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
			}
			if(this->Enabled[2])
			{
				TIM_OC3Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��3
				TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
			}
			if(this->Enabled[3])
			{
				TIM_OC4Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��3
				TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
			}
			
			TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIM3���ؼĴ���ARR
            break;
        case Timer4:
			#if defined(STM32F1) || defined(STM32F4)			
            if(this->Enabled[0])
			{
				TIM_OC1Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��3
				TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
			}
			if(this->Enabled[1])
			{
				TIM_OC2Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��3
				TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
			}
			
			if(this->Enabled[2])
			{
				TIM_OC3Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��3
				TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
			}
			if(this->Enabled[3])
			{
				TIM_OC4Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��3
				TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
			}			
			TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIM3���ؼĴ���ARR
            #elif defined STM32F0
			#endif
			break;
        case Timer5:
			#if defined(STM32F1) || defined(STM32F4)
            if(this->Enabled[0])
			{
				TIM_OC1Init(TIM5, &TIM_OCInitStructure); //ʹ��ͨ��3
				TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
			}
			if(this->Enabled[1])
			{
				TIM_OC2Init(TIM5, &TIM_OCInitStructure); //ʹ��ͨ��3
				TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
			}
			if(this->Enabled[2])
			{
				TIM_OC3Init(TIM5, &TIM_OCInitStructure); //ʹ��ͨ��3
				TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
			}
			if(this->Enabled[3])
			{
				TIM_OC4Init(TIM5, &TIM_OCInitStructure); //ʹ��ͨ��3
				TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable); //ʹ��Ԥװ��	
			}			
			TIM_ARRPreloadConfig(TIM5, ENABLE); //ʹ��TIM3���ؼĴ���ARR
            #elif defined STM32F0
			#endif
			break;
        case Timer6:
            break;
        case Timer7:
            break;
        case Timer8:
            break;
        case Timer9:
            break;
        case Timer10:
            break;
        case Timer11:
            break;
        case Timer12:
            break;
        case Timer13:
            break;
        case Timer14:
            break;
        case Timer15:
            break;
        case Timer16:
            break;
        case Timer17:
            break;
        case Timer18:
            break;
        default:
			break;
	}
}



void Pwm::Close()
{
	Timer::Close();
}
// ˢ�����
void Pwm::Flush()
{
}
void Pwm::Config()
{
	Timer::Config();
	switch (this->_index)
	{
		case Timer1:
            break;
        case Timer2:
            if(this->Enabled[0])
			{
				this->Ports[0]=new AlternatePort(PA0, false, true);
			}
			if(this->Enabled[1])
			{
				this->Ports[1]=new AlternatePort(PA1, false, true);
			}
			if(this->Enabled[2])
			{
				this->Ports[2]=new AlternatePort(PA2, false, true);
			}
			if(this->Enabled[3])
			{
				this->Ports[3]=new AlternatePort(PA3, false, true);
			}
			break;
        case Timer3:
			if(this->Enabled[0])
			{
				this->Ports[0]=new AlternatePort(PA6, false, true);
			}
			if(this->Enabled[1])
			{
				this->Ports[1]=new AlternatePort(PA7, false, true);
			}
			if(this->Enabled[2])
			{
				this->Ports[2]=new AlternatePort(PB0, false, true);
			}
			if(this->Enabled[3])
			{
				this->Ports[3]=new AlternatePort(PB1, false, true);
			}
            break;
        case Timer4:
            if(this->Enabled[0])
			{
				this->Ports[0]=new AlternatePort(PB6, false, true);
			}
			if(this->Enabled[1])
			{
				this->Ports[1]=new AlternatePort(PB7, false, true);
			}
			if(this->Enabled[2])
			{
				this->Ports[2]=new AlternatePort(PB8, false, true);
			}
			if(this->Enabled[3])
			{
				this->Ports[3]=new AlternatePort(PB9, false, true);
			}
			break;
        case Timer5:
            if(this->Enabled[0])
			{
				this->Ports[0]=new AlternatePort();
			}
			if(this->Enabled[1])
			{
				this->Ports[1]=new AlternatePort();
			}
			if(this->Enabled[2])
			{
				this->Ports[2]=new AlternatePort();
			}
			if(this->Enabled[3])
			{
				this->Ports[3]=new AlternatePort();
			}
			break;
        case Timer6:
            break;
        case Timer7:
            break;
        case Timer8:
            break;
        case Timer9:
            break;
        case Timer10:
            break;
        case Timer11:
            break;
        case Timer12:
            break;
        case Timer13:
            break;
        case Timer14:
            break;
        case Timer15:
            break;
        case Timer16:
            break;
        case Timer17:
            break;
        case Timer18:
            break;
        default:
			break;
	}
}
void PwmData::OnInterrupt()
{
	
}
void PwmData::Config()
{
}













