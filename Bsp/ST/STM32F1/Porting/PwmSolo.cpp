#include "PwmSolo.h"
#include "../../stm32.h"

void PwmSolo::OnOpen2()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
	//u16 CCR1_Val = 500;       
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = (this->_freq==100?10000:this->_freq); //999;       //当定时器从0计数到999，即为1000次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = (this->_freq == 100 ? 72 : 0); //设置预分频：不预分频，即为72MHz,0:7.2k 72:10Hz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分频系数：不分频(这里用不到)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //配置为PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = this->_duty; //CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
	TIM_OCInitStructure.TIM_OCPolarity = this->_invert ? TIM_OCPolarity_Low : TIM_OCPolarity_High; //当定时器计数值小于CCR1_Val时为高电平
	switch (this->_Pin)
	{
		//TIM2_CH1
	case PA0:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel1 */
		TIM_OC1Init(TIM2, &TIM_OCInitStructure); //使能通道2
		TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE); //使能定时器2
		TIM_OC1Init(TIM2, &TIM_OCInitStructure); //使能通道1
		break;
		//TIM2_CH2
	case PA1:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel2 */
		TIM_OC2Init(TIM2, &TIM_OCInitStructure); //使能通道2
		TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE); //使能定时器3	
		TIM_OC2Init(TIM2, &TIM_OCInitStructure); //使能通道2
		break;
		//TIM2_CH3
	case PA2:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel3 */
		TIM_OC3Init(TIM2, &TIM_OCInitStructure); //使能通道2
		TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE); //使能定时器2
		TIM_OC3Init(TIM2, &TIM_OCInitStructure); //使能通道3
		break;
		//TIM2_CH4
	case PA3:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel4 */
		TIM_OC4Init(TIM2, &TIM_OCInitStructure); //使能通道4
		TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM2, ENABLE); // 使能TIM2重载寄存器ARR
		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE); //使能定时器2	
		TIM_OC4Init(TIM2, &TIM_OCInitStructure); //使能通道4
		break;
		//TIM3_CH1
	case PA6:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel1 */
		TIM_OC1Init(TIM3, &TIM_OCInitStructure); //使能通道1
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE); //使能定时器3	
		TIM_OC1Init(TIM3, &TIM_OCInitStructure); //使能通道1
		break;
		//TIM3_CH2
	case PA7:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		/* PWM信号电平跳变值 */
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel2 */
		TIM_OC2Init(TIM3, &TIM_OCInitStructure); //使能通道2
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE); //使能定时器3	
		TIM_OC2Init(TIM3, &TIM_OCInitStructure); //使能通道2
		break;
		//TIM3_CH3
	case PB0:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel3 */
		TIM_OC3Init(TIM3, &TIM_OCInitStructure); //使能通道3
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE); //使能定时器3	
		TIM_OC3Init(TIM3, &TIM_OCInitStructure); //使能通道3
		break;
		//TIM3_CH4
	case PB1:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel4 */
		TIM_OC4Init(TIM3, &TIM_OCInitStructure); //使能通道4
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM3, ENABLE); // 使能TIM3重载寄存器ARR
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE); //使能定时器3            
		TIM_OC4Init(TIM3, &TIM_OCInitStructure); //使能通道4
		break;
		//TIM4_CH1
	case PB6:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel1 */
		TIM_OC1Init(TIM4, &TIM_OCInitStructure); //使能通道1
		TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* TIM4 enable counter */
		TIM_Cmd(TIM4, ENABLE); //使能定时器4	
		TIM_OC1Init(TIM4, &TIM_OCInitStructure); //使能通道1
		break;
		//TIM4_CH2
	case PB7:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel2 */
		TIM_OC2Init(TIM4, &TIM_OCInitStructure); //使能通道4
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM4, ENABLE); //使能定时器4	
		TIM_OC2Init(TIM4, &TIM_OCInitStructure); //使能通道2
		break;
		//TIM4_CH3
	case PB8:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel3 */
		TIM_OC3Init(TIM4, &TIM_OCInitStructure); //使能通道4
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM4, ENABLE); //使能定时器4	
		TIM_OC3Init(TIM4, &TIM_OCInitStructure); //使能通道3
		break;
		//TIM4_CH4
	case PB9:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel4 */
		TIM_OC4Init(TIM4, &TIM_OCInitStructure); //使能通道4
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM4, ENABLE); // 使能TIM4重载寄存器ARR
		/* TIM4 enable counter */
		TIM_Cmd(TIM4, ENABLE); //使能定时器4
		TIM_OC4Init(TIM4, &TIM_OCInitStructure); //使能通道4
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
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel1 */
		TIM_OC1Init(TIM2, &TIM_OCInitStructure); //使能通道2
		TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE); //使能定时器2
		TIM_OC1Init(TIM2, &TIM_OCInitStructure); //使能通道1
		break;
	case PB3:
		//TIM2-CH2 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel2 */
		TIM_OC2Init(TIM2, &TIM_OCInitStructure); //使能通道2
		TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE); //使能定时器3	
		TIM_OC2Init(TIM2, &TIM_OCInitStructure); //使能通道2
		break;
	case PB10:
		//TIM2-CH3 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel3 */
		TIM_OC3Init(TIM2, &TIM_OCInitStructure); //使能通道2
		TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE); //使能定时器2
		TIM_OC3Init(TIM2, &TIM_OCInitStructure); //使能通道3
		break;
	case PB11:
		//TIM2-CH4 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel4 */
		TIM_OC4Init(TIM2, &TIM_OCInitStructure); //使能通道4
		TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM2, ENABLE); // 使能TIM2重载寄存器ARR
		TIM_Cmd(TIM2, ENABLE); //使能定时器2
		TIM_OC4Init(TIM2, &TIM_OCInitStructure); //使能通道4
		/* TIM2 enable counter */
		break;
	case PC6:
		//TIM3-CH1 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel1 */
		TIM_OC1Init(TIM3, &TIM_OCInitStructure); //使能通道1
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE); //使能定时器3	
		TIM_OC1Init(TIM3, &TIM_OCInitStructure); //使能通道1
		break;
	case PC7:
		//TIM3-CH2 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel2 */
		TIM_OC2Init(TIM3, &TIM_OCInitStructure); //使能通道2
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE); //使能定时器3	
		TIM_OC2Init(TIM3, &TIM_OCInitStructure); //使能通道2
		break;
	case PC8:
		//TIM3-CH3 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel3 */
		TIM_OC3Init(TIM3, &TIM_OCInitStructure); //使能通道3
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE); //使能定时器3	
		TIM_OC3Init(TIM3, &TIM_OCInitStructure); //使能通道3
		break;
	case PC9:
		//TIM3-CH4 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel4 */
		TIM_OC4Init(TIM3, &TIM_OCInitStructure); //使能通道4
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM3, ENABLE); // 使能TIM3重载寄存器ARR
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE); //使能定时器3	
		TIM_OC4Init(TIM3, &TIM_OCInitStructure); //使能通道4
		break;
	case PD12:
		//TIM4-CH1 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel1 */
		TIM_OC1Init(TIM4, &TIM_OCInitStructure); //使能通道1
		TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* TIM4 enable counter */
		TIM_Cmd(TIM4, ENABLE); //使能定时器4	
		TIM_OC1Init(TIM4, &TIM_OCInitStructure); //使能通道1
		break;
	case PD13:
		//TIM4-CH2 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel2 */
		TIM_OC2Init(TIM4, &TIM_OCInitStructure); //使能通道4
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM4, ENABLE); //使能定时器4	
		TIM_OC2Init(TIM4, &TIM_OCInitStructure); //使能通道2
		break;
	case PD14:
		//TIM4-CH3 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel3 */
		TIM_OC3Init(TIM4, &TIM_OCInitStructure); //使能通道4
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM4, ENABLE); //使能定时器4	
		TIM_OC3Init(TIM4, &TIM_OCInitStructure); //使能通道3
		break;
	case PD15:
		//TIM4-CH4 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel4 */
		TIM_OC4Init(TIM4, &TIM_OCInitStructure); //使能通道4
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM4, ENABLE); // 使能TIM4重载寄存器ARR
		/* TIM4 enable counter */
		TIM_Cmd(TIM4, ENABLE); //使能定时器4
		TIM_OC4Init(TIM4, &TIM_OCInitStructure); //使能通道4
		break;
	default:
		break;
	}
}

//设置输出值
void PwmSolo::SetOut(int pwmValue)
{
	this->_duty = pwmValue;
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
}
PwmSolo::PwmSolo(uint32_t frq, uint32_t duty, bool invert)
{
	this->_freq = frq;
	this->_duty = duty;
	this->_invert = invert;
}
void PwmSolo::Open()
{
	this->mode(AF_OD_PU);
	this->OnOpen2();
}
//设置输出比例，0-100
void PwmSolo::SetOutPercent(float per)
{
	this->_duty = (this->_freq == 100 ? 10000 : this->_freq) *per / 100;
	this->SetOut(this->_duty);
}
