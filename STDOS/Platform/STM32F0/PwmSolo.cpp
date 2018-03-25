#include "PwmSolo.h"
#include "Platform\stm32.h"

void PwmSolo::OnOpen2()
{	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//定义一个TIM_InitTypeDef类型的结构体
	TIM_OCInitTypeDef  TIM_OCInitStructure;	
	
	TIM_TimeBaseStructure.TIM_Prescaler = 0;//时钟频率的预分频值
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数 
	TIM_TimeBaseStructure.TIM_Period = this->_freq;//自动重装载寄存器周期值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;//时钟分割值
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	
	//输出模式配置
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//使能输出比较状态
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//使能输出比较状态
	TIM_OCInitStructure.TIM_OCPolarity =  this->_invert?TIM_OCPolarity_Low:TIM_OCPolarity_High;//TIM输出比较极性低
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;//当MOE=0时，设置TIM输出比较空闲状态
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;//
	TIM_OCInitStructure.TIM_Pulse = 0;//占空比赋值
		
	switch (this->_Pin)
    {
        //TIM16_CH1
        case PE0:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
			TIM_DeInit(TIM16);
			TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);
			TIM_OC1Init(TIM16, &TIM_OCInitStructure);
			TIM_Cmd(TIM16, ENABLE);
			TIM_CtrlPWMOutputs(TIM16, ENABLE);
            break;
		//TIM17_CH1
        case PE1:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);
			TIM_DeInit(TIM17);
			TIM_TimeBaseInit(TIM17, &TIM_TimeBaseStructure);
			TIM_OC1Init(TIM17, &TIM_OCInitStructure);
			TIM_Cmd(TIM17, ENABLE);
			TIM_CtrlPWMOutputs(TIM17, ENABLE);
            break;
		//TIM3_CH4
        case PE6:
//            RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//			TIM_DeInit(TIM3);
//			TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//			TIM_OC4Init(TIM3, &TIM_OCInitStructure);
//			TIM_Cmd(TIM3, ENABLE);
//			TIM_CtrlPWMOutputs(TIM3, ENABLE);
            break;
		//TIM1_CH1
        case PE9:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
			TIM_DeInit(TIM1);
			TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
			TIM_OC1Init(TIM1, &TIM_OCInitStructure);
			TIM_Cmd(TIM1, ENABLE);
			TIM_CtrlPWMOutputs(TIM1, ENABLE);
            break;
		//TIM1_CH2
        case PE11:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
			TIM_DeInit(TIM1);
			TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
			TIM_OC2Init(TIM1, &TIM_OCInitStructure);
			TIM_Cmd(TIM1, ENABLE);
			TIM_CtrlPWMOutputs(TIM1, ENABLE);
            break;
		//TIM1_CH3
        case PE13:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
			TIM_DeInit(TIM1);
			TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
			TIM_OC3Init(TIM1, &TIM_OCInitStructure);
			TIM_Cmd(TIM1, ENABLE);
			TIM_CtrlPWMOutputs(TIM1, ENABLE);
            break;
		//TIM1_CH4
        case PE14:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
			TIM_DeInit(TIM1);
			TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
			TIM_OC4Init(TIM1, &TIM_OCInitStructure);
			TIM_Cmd(TIM1, ENABLE);
			TIM_CtrlPWMOutputs(TIM1, ENABLE);
            break;
		//TIM15_CH1
        case PF9:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
			TIM_DeInit(TIM15);
			TIM_TimeBaseInit(TIM15, &TIM_TimeBaseStructure);
			TIM_OC1Init(TIM15, &TIM_OCInitStructure);
			TIM_Cmd(TIM15, ENABLE);
			TIM_CtrlPWMOutputs(TIM15, ENABLE);
            break;
		//TIM15_CH2
        case PF10:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
			TIM_DeInit(TIM15);
			TIM_TimeBaseInit(TIM15, &TIM_TimeBaseStructure);
			TIM_OC2Init(TIM15, &TIM_OCInitStructure);
			TIM_Cmd(TIM15, ENABLE);
			TIM_CtrlPWMOutputs(TIM15, ENABLE);
            break;
		default:
			break;
	}
}

//设置输出值
void PwmSolo::SetOut(ushort pwmValue)
{
    this->_duty = pwmValue;
	
	switch (this->_Pin)
    {
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
		//TIM14_CH1
        case PA4:
            TIM14->CCR1 = this->_duty;
            break;
		//TIM3_CH1 TIM16_CH1
        case PA6:
            TIM3->CCR1 = this->_duty;
            break;
		//TIM3_CH2 TIM14_CH1 TIM17_CH1
        case PA7:
            TIM3->CCR2 = this->_duty;
            break;
		//TIM1_CH1
        case PA8:
            TIM1->CCR1 = this->_duty;
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
		//TIM3_CH3
        case PB0:
            TIM3->CCR3 = this->_duty;
            break;
		//TIM3_CH4 TIM14_CH1
        case PB1:
            TIM3->CCR4 = this->_duty;
            break;
		//TIM2_CH2
        case PB3:
            TIM2->CCR2 = this->_duty;
            break;
		//TIM3_CH1
        case PB4:
            TIM3->CCR1 = this->_duty;
            break;
		//TIM3_CH2
        case PB5:
            TIM3->CCR2 = this->_duty;
            break;
		//TIM16_CH1
        case PB8:
            TIM16->CCR1 = this->_duty;
            break;
		//TIM17_CH1
        case PB9:
            TIM17->CCR1 = this->_duty;
            break;
		//TIM2_CH3
        case PB10:
            TIM2->CCR3 = this->_duty;
            break;
		//TIM2_CH4
        case PB11:
            TIM2->CCR4 = this->_duty;
            break;
		//TIM15_CH1
        case PB14:
            TIM15->CCR1 = this->_duty;
            break;
		//TIM15_CH2
        case PB15:
            TIM15->CCR2 = this->_duty;
            break;
		//TIM3_CH1
        case PC6:
            TIM3->CCR1 = this->_duty;
            break;
		//TIM3_CH2
        case PC7:
            TIM3->CCR2 = this->_duty;
            break;
		//TIM3_CH3
        case PC8:
            TIM3->CCR3 = this->_duty;
            break;
		//TIM3_CH4
        case PC9:
            TIM3->CCR4 = this->_duty;
            break;
		//TIM16_CH1
        case PE0:
            TIM16->CCR1 = this->_duty;
            break;
		//TIM17_CH1
        case PE1:
            TIM17->CCR1 = this->_duty;
            break;
		//TIM3_CH1
        case PE3:
            TIM3->CCR1 = this->_duty;
            break;
		//TIM3_CH2
        case PE4:
            TIM3->CCR2 = this->_duty;
            break;
		//TIM3_CH3
        case PE5:
            TIM3->CCR3 = this->_duty;
            break;
		//TIM3_CH4
        case PE6:
            TIM3->CCR4 = this->_duty;
            break;
		//TIM1_CH1
        case PE9:
            TIM1->CCR1 = this->_duty;
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
		//TIM15_CH1
        case PF9:
            TIM15->CCR1 = this->_duty;
            break;
		//TIM15_CH2
        case PF10:
            TIM15->CCR2 = this->_duty;
            break;
		default:
			break;
	}	
}
PwmSolo::PwmSolo(uint32_t frq, uint32_t duty, bool invert)
{
	this->_freq = frq;
	this->_duty = duty;
	this->_invert = invert;
}
void PwmSolo::Open()
{
	AlternatePort::Open();
	this->OnOpen2();
}
//设置输出比例，0-100
void PwmSolo::SetOutPercent(float per)
{
	this->_duty = this->_freq *per / 100;
	this->SetOut(this->_duty);
}
