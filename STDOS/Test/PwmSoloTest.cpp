#include "Device\PwmSolo.h"

#define _PWMSOLO_TEST_CPP
#ifdef _PWMSOLO_TEST_CPP

PwmSolo pwm(PA6, 10000, 2700);

void PwmTestRoutin123(void *param)
{
	static int i = 0;
	if (i++ > 100)
		i = 0;
	pwm.SetOutPercent(i);
}

void PwmSoloTestInit()
{
	pwm.SetOutPercent(0);
	pwm.Open();
	pwm.SetPin(PA6);
	Sys.AddTask(PwmTestRoutin123,0,0,100,"pwmtest");
}

#include "stm32f10x.h"


static void TIMx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 设置TIM3CLK 为 72MHZ */
	//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* GPIOA and GPIOB clock enable */
	RCC_APB2PeriphClockCmd(( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB ), ENABLE);

	/*GPIOA Configuration: TIM3 channel 1 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	///*GPIOB Configuration: TIM3 channel 2 as alternate function push-pull */
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	//GPIO_Init(GPIOA, &GPIO_InitStructure);

	///*GPIOB Configuration: TIM3 channel 3 as alternate function push-pull */
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	//GPIO_Init(GPIOB, &GPIO_InitStructure);

	///*GPIOB Configuration: TIM3 channel 4 as alternate function push-pull */
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	//GPIO_Init(GPIOB, &GPIO_InitStructure);

}

/**
* @brief  配置TIM3输出的PWM信号的模式，如周期、极性、占空比
* @param  无
* @retval 无
*/
/*
* TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT 重新计数
*                    TIMx_CCR(电平发生变化)
* 信号周期=(TIMx_ARR +1 ) * 时钟周期
* 占空比=TIMx_CCR/(TIMx_ARR +1)
*/
static void TIMx_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
	u16 CCR1_Val = 500;
	u16 CCR2_Val = 375;
	u16 CCR3_Val = 250;
	u16 CCR4_Val = 125;

	/* -----------------------------------------------------------------------
	macTIMx Channel1 duty cycle = (macTIMx_CCR1/ macTIMx_ARR+1)* 100% = 50%
	macTIMx Channel2 duty cycle = (macTIMx_CCR2/ macTIMx_ARR+1)* 100% = 37.5%
	macTIMx Channel3 duty cycle = (macTIMx_CCR3/ macTIMx_ARR+1)* 100% = 25%
	macTIMx Channel4 duty cycle = (macTIMx_CCR4/ macTIMx_ARR+1)* 100% = 12.5%
	----------------------------------------------------------------------- */

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 999;       //当定时器从0计数到999，即为1000次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //设置预分频：不预分频，即为72MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//设置时钟分频系数：不分频(这里用不到)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //使能通道1
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	///* PWM1 Mode configuration: Channel2 */
	//TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //设置通道2的电平跳变值，输出另外一个占空比的PWM
	//TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //使能通道2
	//TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	///* PWM1 Mode configuration: Channel3 */
	//TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//设置通道3的电平跳变值，输出另外一个占空比的PWM
	//TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //使能通道3
	//TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	///* PWM1 Mode configuration: Channel4 */
	//TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//设置通道4的电平跳变值，输出另外一个占空比的PWM
	//TIM_OC4Init(TIM3, &TIM_OCInitStructure);	//使能通道4
	//TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	//TIM_ARRPreloadConfig(TIM3, ENABLE);			 // 使能TIM3重载寄存器ARR

													 /* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);                   //使能定时器3	
}

void PwmSolo::SetPin(Pin pin)
{
	switch (pin)
	{
	case P0:
		break;
	case PA0:
		break;
	case PA1:
		break;
	case PA2:
		break;
	case PA3:
		break;
	case PA4:
		break;
	case PA5:
		break;
	case PA6:
		TIMx_GPIO_Config();
		TIMx_Mode_Config();
		break;
	case PA7:
		break;
	case PA8:
		break;
	case PA9:
		break;
	case PA10:
		break;
	case PA11:
		break;
	case PA12:
		break;
	case PA13:
		break;
	case PA14:
		break;
	case PA15:
		break;
	case PB0:
		break;
	case PB1:
		break;
	case PB2:
		break;
	case PB3:
		break;
	case PB4:
		break;
	case PB5:
		break;
	case PB6:
		break;
	case PB7:
		break;
	case PB8:
		break;
	case PB9:
		break;
	case PB10:
		break;
	case PB11:
		break;
	case PB12:
		break;
	case PB13:
		break;
	case PB14:
		break;
	case PB15:
		break;
	case PC0:
		break;
	case PC1:
		break;
	case PC2:
		break;
	case PC3:
		break;
	case PC4:
		break;
	case PC5:
		break;
	case PC6:
		break;
	case PC7:
		break;
	case PC8:
		break;
	case PC9:
		break;
	case PC10:
		break;
	case PC11:
		break;
	case PC12:
		break;
	case PC13:
		break;
	case PC14:
		break;
	case PC15:
		break;
	case PD0:
		break;
	case PD1:
		break;
	case PD2:
		break;
	case PD3:
		break;
	case PD4:
		break;
	case PD5:
		break;
	case PD6:
		break;
	case PD7:
		break;
	case PD8:
		break;
	case PD9:
		break;
	case PD10:
		break;
	case PD11:
		break;
	case PD12:
		break;
	case PD13:
		break;
	case PD14:
		break;
	case PD15:
		break;
	case PE0:
		break;
	case PE1:
		break;
	case PE2:
		break;
	case PE3:
		break;
	case PE4:
		break;
	case PE5:
		break;
	case PE6:
		break;
	case PE7:
		break;
	case PE8:
		break;
	case PE9:
		break;
	case PE10:
		break;
	case PE11:
		break;
	case PE12:
		break;
	case PE13:
		break;
	case PE14:
		break;
	case PE15:
		break;
	case PF0:
		break;
	case PF1:
		break;
	case PF2:
		break;
	case PF3:
		break;
	case PF4:
		break;
	case PF5:
		break;
	case PF6:
		break;
	case PF7:
		break;
	case PF8:
		break;
	case PF9:
		break;
	case PF10:
		break;
	case PF11:
		break;
	case PF12:
		break;
	case PF13:
		break;
	case PF14:
		break;
	case PF15:
		break;
	case PG0:
		break;
	case PG1:
		break;
	case PG2:
		break;
	case PG3:
		break;
	case PG4:
		break;
	case PG5:
		break;
	case PG6:
		break;
	case PG7:
		break;
	case PG8:
		break;
	case PG9:
		break;
	case PG10:
		break;
	case PG11:
		break;
	case PG12:
		break;
	case PG13:
		break;
	case PG14:
		break;
	case PG15:
		break;
	case PH0:
		break;
	case PH1:
		break;
	case PH2:
		break;
	case PH3:
		break;
	case PH4:
		break;
	case PH5:
		break;
	case PH6:
		break;
	case PH7:
		break;
	case PH8:
		break;
	case PH9:
		break;
	case PH10:
		break;
	case PH11:
		break;
	case PH12:
		break;
	case PH13:
		break;
	case PH14:
		break;
	case PH15:
		break;
	case PI0:
		break;
	case PI1:
		break;
	case PI2:
		break;
	case PI3:
		break;
	case PI4:
		break;
	case PI5:
		break;
	case PI6:
		break;
	case PI7:
		break;
	case PI8:
		break;
	case PI9:
		break;
	case PI10:
		break;
	case PI11:
		break;
	case PI12:
		break;
	case PI13:
		break;
	case PI14:
		break;
	case PI15:
		break;
	default:
		break;
	}
}
#endif // _PWMSOLO_TEST_CPP
