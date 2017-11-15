#include "PwmSolo.h"
#include "stm32f4xx.h"

void PwmSolo::OnOpen2()
{
    //u32 arr = 500-1;
    //u32 psc = 84-1;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    TIM_TimeBaseStructure.TIM_Prescaler = 2; //��ʱ����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_Period = this->_freq; //�Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    	 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = this->_invert?TIM_OCPolarity_Low:TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�
    
    switch (this->_Pin)
    {
        //TIM2_CH1
        case PA0:
            break;
            //TIM5_CH2
        case PA1:
            break;
            //TIM9_CH1
        case PA2:
            break;
            //TIM5_CH4 TIM9_CH2 TIM2_CH4
        case PA3:
            break;
            //TIM13_CH1 TIM3_CH1
        case PA6:
            break;
            //TIM14_CH1 TIM3_CH2
        case PA7:
            break;
            //TIM3_CH3
        case PB0:
            break;
            //TIM3_CH4
        case PB1:
            break;
            //TIM1_CH2
        case PE11:
            break;
            //TIM1_CH3
        case PE13:
            break;
            //TIM1_CH4
        case PE14:
            break;
            //TIM2_CH3
        case PB10:
            break;
            //TIM2_CH4
        case PB11:
            break;
            //TIM12_CH1
        case PH6:
            break;
            //TIM12_CH2
        case PH9:
            break;
            //TIM5_CH1
        case PH10:
            break;
            //TIM5_CH2
        case PH11:
            break;
            //TIM5_CH3
        case PH12:
            break;
            //TIM12_CH1
        case PB14:
            break;
            //TIM12_CH2
        case PB15:
            break;
            //TIM4_CH1
        case PD12:
            break;
            //TIM4_CH2
        case PD13:
            break;
            //TIM4_CH3
        case PD14:
            break;
            //TIM4_CH4
        case PD15:
            break;
            //TIM8_CH1 TIM3_CH1
        case PC6:
            break;
            //TIM8_CH2 TIM3_CH2
        case PC7:
            break;
            //TIM8_CH3 TIM3_CH3
        case PC8:
            break;
            //TIM8_CH4 TIM3_CH4
        case PC9:
            break;
            //TIM1_CH2
        case PA9:
            break;
            //TIM1_CH3
        case PA10:
            break;
            //TIM1_CH4
        case PA11:
            break;
            //TIM5_CH4
        case PI0:
            break;
            //TIM8_CH4
        case PI2:
            break;
            //TIM9_CH1
        case PE5:
            break;
            //TIM9_CH2
        case PE6:
            break;
            //TIM10_CH1
        case PF6:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE); 	
			GPIO_PinAFConfig(GPIOF, GPIO_PinSource6, GPIO_AF_TIM10);
			TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure);
			TIM_OC1Init(TIM10, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable);
			TIM_ARRPreloadConfig(TIM10, ENABLE);
			TIM_Cmd(TIM10, ENABLE);
            break;
            //TIM11-CH1
        case PF7:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE); 	
			GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_TIM11);
			TIM_TimeBaseInit(TIM11, &TIM_TimeBaseStructure);
			TIM_OC1Init(TIM11, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM11, TIM_OCPreload_Enable);
			TIM_ARRPreloadConfig(TIM11, ENABLE);
			TIM_Cmd(TIM11, ENABLE);
            break;
            //TIM13_CH1
        case PF8:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE); 	
			GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_TIM13);
			TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);
			TIM_OC1Init(TIM13, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable);
			TIM_ARRPreloadConfig(TIM13, ENABLE);
			TIM_Cmd(TIM13, ENABLE);
            break;
            //TIM14_CH1
        case PF9:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE); //TIM14ʱ��ʹ��    
			GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14); //GPIOF9����Ϊ��ʱ��14
			TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure); //��ʼ����ʱ��14
			TIM_OC1Init(TIM14, &TIM_OCInitStructure); //����Tָ���Ĳ�����ʼ������TIM1 4OC1
			TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable); //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
			TIM_ARRPreloadConfig(TIM14, ENABLE); //ARPEʹ�� 
			TIM_Cmd(TIM14, ENABLE); //ʹ��TIM14
            break;
        default:
            break;
    }
}

//�������ֵ
void PwmSolo::SetOut(ushort pwmValue)
{
    this->_duty = pwmValue;

    switch (this->_Pin)
    {
        //TIM2_CH1
        case PA0:
            TIM2->CCR1 = this->_duty;
            break;
            //TIM5_CH2
        case PA1:
            TIM5->CCR2 = this->_duty;
            break;
            //TIM9_CH1
        case PA2:
            TIM9->CCR1 = this->_duty;
            break;
            //TIM5_CH4 TIM9_CH2 TIM2_CH4
        case PA3:
            TIM5->CCR4 = this->_duty;
            break;
            //TIM13_CH1 TIM3_CH1
        case PA6:
            TIM13->CCR1 = this->_duty;
            break;
            //TIM14_CH1 TIM3_CH2
        case PA7:
            TIM14->CCR1 = this->_duty;
            break;
            //TIM3_CH3
        case PB0:
            TIM3->CCR3 = this->_duty;
            break;
            //TIM3_CH4
        case PB1:
            TIM3->CCR4 = this->_duty;
            break;
            //TIM1_CH2
        case PE11:
            TIM1->CCR2 = this->_duty;
            break;
            //TIM1_CH3
        case PE13:
            TIM1->CCR3 = this->_duty;
            break;
            //TIM1_CH4
        case PE14:
            TIM1->CCR4 = this->_duty;
            break;
            //TIM2_CH3
        case PB10:
            TIM2->CCR3 = this->_duty;
            break;
            //TIM2_CH4
        case PB11:
            TIM2->CCR4 = this->_duty;
            break;
            //TIM12_CH1
        case PH6:
            TIM12->CCR1 = this->_duty;
            break;
            //TIM12_CH2
        case PH9:
            TIM12->CCR2 = this->_duty;
            break;
            //TIM5_CH1
        case PH10:
            TIM5->CCR1 = this->_duty;
            break;
            //TIM5_CH2
        case PH11:
            TIM5->CCR2 = this->_duty;
            break;
            //TIM5_CH3
        case PH12:
            TIM5->CCR3 = this->_duty;
            break;
            //TIM12_CH1
        case PB14:
            TIM12->CCR1 = this->_duty;
            break;
            //TIM12_CH2
        case PB15:
            TIM12->CCR2 = this->_duty;
            break;
            //TIM4_CH1
        case PD12:
            TIM4->CCR1 = this->_duty;
            break;
            //TIM4_CH2
        case PD13:
            TIM4->CCR2 = this->_duty;
            break;
            //TIM4_CH3
        case PD14:
            TIM4->CCR3 = this->_duty;
            break;
            //TIM4_CH4
        case PD15:
            TIM4->CCR4 = this->_duty;
            break;
            //TIM8_CH1 TIM3_CH1
        case PC6:
            TIM8->CCR1 = this->_duty;
            break;
            //TIM8_CH2 TIM3_CH2
        case PC7:
            TIM8->CCR2 = this->_duty;
            break;
            //TIM8_CH3 TIM3_CH3
        case PC8:
            TIM8->CCR3 = this->_duty;
            break;
            //TIM8_CH4 TIM3_CH4
        case PC9:
            TIM8->CCR4 = this->_duty;
            break;
            //TIM1_CH2
        case PA9:
            TIM1->CCR2 = this->_duty;
            break;
            //TIM1_CH3
        case PA10:
            TIM1->CCR3 = this->_duty;
            break;
            //TIM1_CH4
        case PA11:
            TIM1->CCR4 = this->_duty;
            break;
            //TIM5_CH4
        case PI0:
            TIM5->CCR4 = this->_duty;
            break;
            //TIM8_CH4
        case PI2:
            TIM8->CCR4 = this->_duty;
            break;
            //TIM9_CH1
        case PE5:
            TIM9->CCR1 = this->_duty;
            break;
            //TIM9_CH2
        case PE6:
            TIM9->CCR2 = this->_duty;
            break;
            //TIM10_CH1
        case PF6:
            TIM10->CCR1 = this->_duty;
            break;
            //TIM11-CH1
        case PF7:
            TIM11->CCR1 = this->_duty;
            break;
            //TIM13_CH1
        case PF8:
            TIM13->CCR1 = this->_duty;
            break;
            //TIM14_CH1
        case PF9:
            TIM14->CCR1 = this->_duty;
            break;
        default:
            break;
    }
}
