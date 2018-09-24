#include "PwmSolo.h"
#include "Platform\stm32.h"

void PwmSolo::OnOpen2()
{
#if defined STM32F0
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//����һ��TIM_InitTypeDef���͵Ľṹ��
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	TIM_TimeBaseStructure.TIM_Prescaler = 0;//ʱ��Ƶ�ʵ�Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ��� 
	TIM_TimeBaseStructure.TIM_Period = this->_freq;//�Զ���װ�ؼĴ�������ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;//ʱ�ӷָ�ֵ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	//���ģʽ����
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//ʹ������Ƚ�״̬
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//ʹ������Ƚ�״̬
	TIM_OCInitStructure.TIM_OCPolarity = this->_invert ? TIM_OCPolarity_Low : TIM_OCPolarity_High;//TIM����Ƚϼ��Ե�
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;//��MOE=0ʱ������TIM����ȽϿ���״̬
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;//
	TIM_OCInitStructure.TIM_Pulse = 0;//ռ�ձȸ�ֵ

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
#elif defined STM32F1
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
	//u16 CCR1_Val = 500;       
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = this->_freq; //999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = this->_duty; //CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
	TIM_OCInitStructure.TIM_OCPolarity = this->_invert ? TIM_OCPolarity_Low : TIM_OCPolarity_High; //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
	switch (this->_Pin)
	{
		//TIM2_CH1
	case PA0:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel1 */
		TIM_OC1Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��2
		TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��2
		TIM_OC1Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��1
		break;
		//TIM2_CH2
	case PA1:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel2 */
		TIM_OC2Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��2
		TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��3	
		TIM_OC2Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��2
		break;
		//TIM2_CH3
	case PA2:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel3 */
		TIM_OC3Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��2
		TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��2
		TIM_OC3Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��3
		break;
		//TIM2_CH4
	case PA3:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel4 */
		TIM_OC4Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��4
		TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM2, ENABLE); // ʹ��TIM2���ؼĴ���ARR
		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��2	
		TIM_OC4Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��4
		break;
		//TIM3_CH1
	case PA6:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel1 */
		TIM_OC1Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��1
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��3	
		TIM_OC1Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��1
		break;
		//TIM3_CH2
	case PA7:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		/* PWM�źŵ�ƽ����ֵ */
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel2 */
		TIM_OC2Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��2
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��3	
		TIM_OC2Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��2
		break;
		//TIM3_CH3
	case PB0:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel3 */
		TIM_OC3Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��3
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��3	
		TIM_OC3Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��3
		break;
		//TIM3_CH4
	case PB1:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel4 */
		TIM_OC4Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��4
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM3, ENABLE); // ʹ��TIM3���ؼĴ���ARR
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��3            
		TIM_OC4Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��4
		break;
		//TIM4_CH1
	case PB6:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel1 */
		TIM_OC1Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��1
		TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* TIM4 enable counter */
		TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ��4	
		TIM_OC1Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��1
		break;
		//TIM4_CH2
	case PB7:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel2 */
		TIM_OC2Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��4
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ��4	
		TIM_OC2Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��2
		break;
		//TIM4_CH3
	case PB8:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel3 */
		TIM_OC3Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��4
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ��4	
		TIM_OC3Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��3
		break;
		//TIM4_CH4
	case PB9:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel4 */
		TIM_OC4Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��4
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM4, ENABLE); // ʹ��TIM4���ؼĴ���ARR
		/* TIM4 enable counter */
		TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ��4
		TIM_OC4Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��4
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
		TIM_OC1Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��2
		TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��2
		TIM_OC1Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��1
		break;
	case PB3:
		//TIM2-CH2 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel2 */
		TIM_OC2Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��2
		TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��3	
		TIM_OC2Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��2
		break;
	case PB10:
		//TIM2-CH3 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel3 */
		TIM_OC3Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��2
		TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��2
		TIM_OC3Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��3
		break;
	case PB11:
		//TIM2-CH4 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel4 */
		TIM_OC4Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��4
		TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM2, ENABLE); // ʹ��TIM2���ؼĴ���ARR
		TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��2
		TIM_OC4Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��4
		/* TIM2 enable counter */
		break;
	case PC6:
		//TIM3-CH1 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel1 */
		TIM_OC1Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��1
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��3	
		TIM_OC1Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��1
		break;
	case PC7:
		//TIM3-CH2 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel2 */
		TIM_OC2Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��2
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��3	
		TIM_OC2Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��2
		break;
	case PC8:
		//TIM3-CH3 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel3 */
		TIM_OC3Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��3
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��3	
		TIM_OC3Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��3
		break;
	case PC9:
		//TIM3-CH4 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel4 */
		TIM_OC4Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��4
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM3, ENABLE); // ʹ��TIM3���ؼĴ���ARR
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��3	
		TIM_OC4Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��4
		break;
	case PD12:
		//TIM4-CH1 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel1 */
		TIM_OC1Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��1
		TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* TIM4 enable counter */
		TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ��4	
		TIM_OC1Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��1
		break;
	case PD13:
		//TIM4-CH2 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel2 */
		TIM_OC2Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��4
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ��4	
		TIM_OC2Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��2
		break;
	case PD14:
		//TIM4-CH3 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel3 */
		TIM_OC3Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��4
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
		/* TIM3 enable counter */
		TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ��4	
		TIM_OC3Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��3
		break;
	case PD15:
		//TIM4-CH4 remap
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* PWM1 Mode configuration: Channel4 */
		TIM_OC4Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��4
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM4, ENABLE); // ʹ��TIM4���ؼĴ���ARR
		/* TIM4 enable counter */
		TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ��4
		TIM_OC4Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��4
		break;
	default:
		break;
	}
#elif defined STM32F4
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	TIM_TimeBaseStructure.TIM_Prescaler = 2; //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = this->_freq; //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = this->_invert ? TIM_OCPolarity_Low : TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�

	switch (this->_Pin)
	{
		//TIM2_CH1
	case PA0:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		TIM_OC1Init(TIM2, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM2, ENABLE);
		TIM_Cmd(TIM2, ENABLE);
		break;
		//TIM5_CH2
	case PA1:
		//            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 	
		//			GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
		//			TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
		//			TIM_OC2Init(TIM5, &TIM_OCInitStructure);
		//			TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
		//			TIM_ARRPreloadConfig(TIM5, ENABLE);
		//			TIM_Cmd(TIM5, ENABLE);
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
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		TIM_OC3Init(TIM3, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM3, ENABLE);
		TIM_Cmd(TIM3, ENABLE);
		break;
		//TIM3_CH4
	case PB1:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		TIM_OC4Init(TIM3, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM3, ENABLE);
		TIM_Cmd(TIM3, ENABLE);
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
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_TIM12);
		TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);
		TIM_OC1Init(TIM12, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM12, ENABLE);
		TIM_Cmd(TIM12, ENABLE);
		break;
		//TIM12_CH2
	case PB15:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM12);
		TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);
		TIM_OC2Init(TIM12, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM12, ENABLE);
		TIM_Cmd(TIM12, ENABLE);
		break;
		//TIM4_CH1
	case PD12:
		break;
		//TIM4_CH2
	case PD13:
		//            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 	
		//			GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
		//			TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
		//			TIM_OC2Init(TIM4, &TIM_OCInitStructure);
		//			TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
		//			TIM_ARRPreloadConfig(TIM4, ENABLE);
		//			TIM_Cmd(TIM4, ENABLE);
		break;
		//TIM4_CH3
	case PD14:
		break;
		//TIM4_CH4
	case PD15:
		break;
		//TIM8_CH1 TIM3_CH1
	case PC6:
		//            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); 	
		//			GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
		//			TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
		//			TIM_OC1Init(TIM8, &TIM_OCInitStructure);
		//			TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
		//			TIM_ARRPreloadConfig(TIM8, ENABLE);
		//			TIM_Cmd(TIM8, ENABLE);
		break;
		//TIM8_CH2 TIM3_CH2
	case PC7:
		//            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); 	
		//			GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
		//			TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
		//			TIM_OC2Init(TIM8, &TIM_OCInitStructure);
		//			TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
		//			TIM_ARRPreloadConfig(TIM8, ENABLE);
		//			TIM_Cmd(TIM8, ENABLE);
		break;
		//TIM8_CH3 TIM3_CH3
	case PC8:
		//            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); 	
		//			GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
		//			TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
		//			TIM_OC3Init(TIM8, &TIM_OCInitStructure);
		//			TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);
		//			TIM_ARRPreloadConfig(TIM8, ENABLE);
		//			TIM_Cmd(TIM8, ENABLE);
		break;
		//TIM8_CH4 TIM3_CH4
	case PC9:
		//            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); 	
		//			GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
		//			TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
		//			TIM_OC4Init(TIM8, &TIM_OCInitStructure);
		//			TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);
		//			TIM_ARRPreloadConfig(TIM8, ENABLE);
		//			TIM_Cmd(TIM8, ENABLE);
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
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_TIM9);
		TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);
		TIM_OC1Init(TIM9, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM9, ENABLE);
		TIM_Cmd(TIM9, ENABLE);
		break;
		//TIM9_CH2
	case PE6:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_TIM9);
		TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);
		TIM_OC2Init(TIM9, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM9, ENABLE);
		TIM_Cmd(TIM9, ENABLE);
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
#endif
}

//�������ֵ
void PwmSolo::SetOut(int pwmValue)
{
#if defined STM32F0
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
#elif defined STM32F1
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
#elif defined STM32F4
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
#endif
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
//�������������0-100
void PwmSolo::SetOutPercent(float per)
{
	this->_duty = this->_freq *per / 100;
	this->SetOut(this->_duty);
}
