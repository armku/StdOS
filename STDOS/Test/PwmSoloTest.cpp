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
/************************** TIM ��������********************************/
void               TIMx_PWM_Init(void);

static void TIMx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ����TIM3CLK Ϊ 72MHZ */
	//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* GPIOA and GPIOB clock enable */
	RCC_APB2PeriphClockCmd(( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB ), ENABLE);

	/*GPIOA Configuration: TIM3 channel 1 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*GPIOB Configuration: TIM3 channel 2 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*GPIOB Configuration: TIM3 channel 3 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*GPIOB Configuration: TIM3 channel 4 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

/**
* @brief  ����TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
* @param  ��
* @retval ��
*/
/*
* TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT ���¼���
*                    TIMx_CCR(��ƽ�����仯)
* �ź�����=(TIMx_ARR +1 ) * ʱ������
* ռ�ձ�=TIMx_CCR/(TIMx_ARR +1)
*/
static void TIMx_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
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
	TIM_TimeBaseStructure.TIM_Period = 999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��1
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //ʹ��ͨ��2
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//����ͨ��3�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��3
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);	//ʹ��ͨ��4
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);			 // ʹ��TIM3���ؼĴ���ARR

													 /* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��3	
}

void TIMx_PWM_Init(void)
{
	TIMx_GPIO_Config();
	TIMx_Mode_Config();
}
#endif // _PWMSOLO_TEST_CPP
