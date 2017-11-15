#include "PwmSolo.h"
#include "stm32f4xx.h"

void PwmSolo::OnOpen2()
{    
	u32 arr=500-1;
	u32 psc=84-1;
	GPIO_InitTypeDef GPIO_InitStructure;
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
        TIM_OCInitTypeDef TIM_OCInitStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE); //TIM14时钟使能    
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); //使能PORTF时钟	

        GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14); //GPIOF9复用为定时器14

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //GPIOF9
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度100MHz
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
        GPIO_Init(GPIOF, &GPIO_InitStructure); //初始化PF9

        TIM_TimeBaseStructure.TIM_Prescaler = psc; //定时器分频
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
        TIM_TimeBaseStructure.TIM_Period = arr; //自动重装载值
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

        TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure); //初始化定时器14

        //初始化TIM14 Channel1 PWM模式	 
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
        TIM_OC1Init(TIM14, &TIM_OCInitStructure); //根据T指定的参数初始化外设TIM1 4OC1

        TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable); //使能TIM14在CCR1上的预装载寄存器

        TIM_ARRPreloadConfig(TIM14, ENABLE); //ARPE使能 

        TIM_Cmd(TIM14, ENABLE); //使能TIM14
}

//设置输出值
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
