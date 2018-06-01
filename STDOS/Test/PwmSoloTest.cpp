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
void TIMx_PWM_Init(void);
void PwmSoloTestInit()
{
	pwm.SetOutPercent(0);
	pwm.Open();
	TIMx_PWM_Init();
	Sys.AddTask(PwmTestRoutin123,0,0,100,"pwmtest");
}

#include "stm32f10x.h"


/********************TIM参数定义，只限TIM2、3、4或5**************************/
#define             macTIMx                                TIM3
#define             macTIM_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             macTIM_CLK                             RCC_APB1Periph_TIM3
#define             macTIM_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macTIM_GPIO_CLK                        ( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB )
#define             macTIM_CH1_PORT                        GPIOA
#define             macTIM_CH1_PIN                         GPIO_Pin_6
#define             macTIM_CH2_PORT                        GPIOA
#define             macTIM_CH2_PIN                         GPIO_Pin_7
#define             macTIM_CH3_PORT                        GPIOB
#define             macTIM_CH3_PIN                         GPIO_Pin_0
#define             macTIM_CH4_PORT                        GPIOB
#define             macTIM_CH4_PIN                         GPIO_Pin_1


/************************** TIM 函数声明********************************/
void               TIMx_PWM_Init(void);
/**
* @brief  配置TIM3复用输出PWM时用到的I/O
* @param  无
* @retval 无
*/
static void TIMx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 设置TIM3CLK 为 72MHZ */
	//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	macTIM_APBxClock_FUN(macTIM_CLK, ENABLE);

	/* GPIOA and GPIOB clock enable */
	macTIM_GPIO_APBxClock_FUN(macTIM_GPIO_CLK, ENABLE);

	/*GPIOA Configuration: TIM3 channel 1 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = macTIM_CH1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(macTIM_CH1_PORT, &GPIO_InitStructure);

	/*GPIOB Configuration: TIM3 channel 2 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = macTIM_CH2_PIN;
	GPIO_Init(macTIM_CH2_PORT, &GPIO_InitStructure);

	/*GPIOB Configuration: TIM3 channel 3 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = macTIM_CH3_PIN;
	GPIO_Init(macTIM_CH3_PORT, &GPIO_InitStructure);

	/*GPIOB Configuration: TIM3 channel 4 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = macTIM_CH4_PIN;
	GPIO_Init(macTIM_CH4_PORT, &GPIO_InitStructure);

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
	TIM_TimeBaseInit(macTIMx, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
	TIM_OC1Init(macTIMx, &TIM_OCInitStructure);	 //使能通道1
	TIM_OC1PreloadConfig(macTIMx, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //设置通道2的电平跳变值，输出另外一个占空比的PWM
	TIM_OC2Init(macTIMx, &TIM_OCInitStructure);	  //使能通道2
	TIM_OC2PreloadConfig(macTIMx, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//设置通道3的电平跳变值，输出另外一个占空比的PWM
	TIM_OC3Init(macTIMx, &TIM_OCInitStructure);	 //使能通道3
	TIM_OC3PreloadConfig(macTIMx, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//设置通道4的电平跳变值，输出另外一个占空比的PWM
	TIM_OC4Init(macTIMx, &TIM_OCInitStructure);	//使能通道4
	TIM_OC4PreloadConfig(macTIMx, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(macTIMx, ENABLE);			 // 使能TIM3重载寄存器ARR

													 /* TIM3 enable counter */
	TIM_Cmd(macTIMx, ENABLE);                   //使能定时器3	
}

/**
* @brief  TIM3 输出PWM信号初始化，只要调用这个函数
*         TIM3的四个通道就会有PWM信号输出
* @param  无
* @retval 无
*/
void TIMx_PWM_Init(void)
{
	TIMx_GPIO_Config();
	TIMx_Mode_Config();
}
#endif // _PWMSOLO_TEST_CPP
