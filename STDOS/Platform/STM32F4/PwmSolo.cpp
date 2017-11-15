#include "PwmSolo.h"
#include "stm32f4xx.h"

void PwmSolo::OnOpen2()
{    
	u32 arr=500-1;
	u32 psc=84-1;
	GPIO_InitTypeDef GPIO_InitStructure;
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
        TIM_OCInitTypeDef TIM_OCInitStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE); //TIM14ʱ��ʹ��    
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); //ʹ��PORTFʱ��	

        GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14); //GPIOF9����Ϊ��ʱ��14

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //GPIOF9
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //���ù���
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //�ٶ�100MHz
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
        GPIO_Init(GPIOF, &GPIO_InitStructure); //��ʼ��PF9

        TIM_TimeBaseStructure.TIM_Prescaler = psc; //��ʱ����Ƶ
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
        TIM_TimeBaseStructure.TIM_Period = arr; //�Զ���װ��ֵ
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

        TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure); //��ʼ����ʱ��14

        //��ʼ��TIM14 Channel1 PWMģʽ	 
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
        TIM_OC1Init(TIM14, &TIM_OCInitStructure); //����Tָ���Ĳ�����ʼ������TIM1 4OC1

        TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable); //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���

        TIM_ARRPreloadConfig(TIM14, ENABLE); //ARPEʹ�� 

        TIM_Cmd(TIM14, ENABLE); //ʹ��TIM14
}

//�������ֵ
void PwmSolo::SetOut(ushort pwmValue)
{
    this->_duty = pwmValue;
    #if 1
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
    #else 
        switch (this->_Pin)
        {
            //TIM2_CH1
            case PA0:
                TIM2->CCR1 = this->_duty;
                break;
                //TIM2_CH2
            case PA1:
                TIM2->CCR2 = this->_duty;
                break;
                //TIM2_CH3
            case PA2:
                TIM2->CCR3 = this->_duty;
                break;
                //TIM2_CH4
            case PA3:
                TIM2->CCR4 = this->_duty;
                break;
                //TIM3_CH1
            case PA6:
                TIM3->CCR1 = this->_duty;
                break;
                //TIM3_CH2
            case PA7:
                TIM3->CCR2 = this->_duty;
                break;
                //TIM3_CH3
            case PB0:
                TIM3->CCR3 = this->_duty;
                break;
                //TIM3_CH4
            case PB1:
                TIM3->CCR4 = this->_duty;
                break;
                //TIM4_CH1
            case PB6:
                TIM3->CCR1 = this->_duty;
                break;
                //TIM4_CH2
            case PB7:
                TIM3->CCR2 = this->_duty;
                break;
                //TIM4_CH3
            case PB8:
                TIM3->CCR3 = this->_duty;
                break;
                //TIM4_CH4
            case PB9:
                TIM3->CCR4 = this->_duty;
                break;
            case PE9:
                //TIM1-CH1 remap
                break;
            case PE11:
                //TIM1-CH2 remap
                break;
            case PE13:
                //TIM1-CH3 remap
                break;
            case PE14:
                //TIM1-CH4 remap
                break;
            case PA15:
                //TIM2-CH1 remap
                TIM2->CCR1 = this->_duty;
                break;
            case PB3:
                //TIM2-CH2 remap
                TIM2->CCR2 = this->_duty;
                break;
            case PB10:
                //TIM2-CH3 remap
                TIM2->CCR3 = this->_duty;
                break;
            case PB11:
                //TIM2-CH4 remap
                TIM2->CCR4 = this->_duty;
                break;
            case PC6:
                //TIM3-CH1 remap
                TIM3->CCR1 = this->_duty;
                break;
            case PC7:
                //TIM3-CH2 remap			
                TIM3->CCR2 = this->_duty;
                break;
            case PC8:
                //TIM3-CH3 remap
                TIM3->CCR3 = this->_duty;
                break;
            case PC9:
                //TIM3-CH4 remap
                TIM3->CCR4 = this->_duty;
                break;
            case PD12:
                //TIM4-CH1 remap
                TIM4->CCR1 = this->_duty;
                break;
            case PD13:
                //TIM4-CH2 remap
                TIM4->CCR2 = this->_duty;
                break;
            case PD14:
                //TIM4-CH3 remap
                TIM4->CCR3 = this->_duty;
                break;
            case PD15:
                //TIM4-CH4 remap
                TIM4->CCR4 = this->_duty;
                break;
            default:
                break;
        }
    #endif 
}
