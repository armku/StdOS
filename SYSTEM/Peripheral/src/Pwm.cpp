#include "Pwm.h"
#include "stm32f10x.h"


PWM::PWM(PinPort pin, uint frq, uint duty): Port(pin)
{
    this->freq = frq;
    this->duty = duty;
}

void PWM::Init()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    switch (this->pin)
    {
		//TIM2_CH1
		case PA0:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

            /* PWM�źŵ�ƽ����ֵ */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
            TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel1 */
            TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //����ΪPWMģʽ1
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
            TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
            TIM_OC1Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��2
            TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
            /* TIM3 enable counter */
            TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��2
            break;
        //TIM2_CH2
		case PA1:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

            /* PWM�źŵ�ƽ����ֵ */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
            TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel2 */
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
            TIM_OC2Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��2
            TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
            /* TIM3 enable counter */
            TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��3	
            break;
        //TIM2_CH3
		case PA2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

            /* PWM�źŵ�ƽ����ֵ */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
            TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel3 */
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //����ͨ��3�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
            TIM_OC3Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��2
            TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
            /* TIM3 enable counter */
            TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��2
            break;
        //TIM2_CH4
		case PA3:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

            /* PWM�źŵ�ƽ����ֵ */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
            TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel4 */
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
            TIM_OC4Init(TIM2, &TIM_OCInitStructure); //ʹ��ͨ��4
            TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
            TIM_ARRPreloadConfig(TIM2, ENABLE); // ʹ��TIM2���ؼĴ���ARR
            /* TIM2 enable counter */
            TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��2	
            break;
        //TIM3_CH1
		case PA6:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

            /* PWM�źŵ�ƽ����ֵ */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
            TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel1 */
            TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //����ΪPWMģʽ1
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
            TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
            TIM_OC1Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��1
            TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
            /* TIM3 enable counter */
            TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��3	
            break;
        //TIM3_CH2
		case PA7:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

            /* PWM�źŵ�ƽ����ֵ */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
            TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel2 */
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
            TIM_OC2Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��2
            TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
            /* TIM3 enable counter */
            TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��3	
            break;
        //TIM3_CH3
		case PB0:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

            /* PWM�źŵ�ƽ����ֵ */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
            TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel3 */
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //����ͨ��3�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
            TIM_OC3Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��3
            TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
            /* TIM3 enable counter */
            TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��3	
            break;
        //TIM3_CH4
		case PB1:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

            /* PWM�źŵ�ƽ����ֵ */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
            TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel4 */
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
            TIM_OC4Init(TIM3, &TIM_OCInitStructure); //ʹ��ͨ��4
            TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
            TIM_ARRPreloadConfig(TIM3, ENABLE); // ʹ��TIM3���ؼĴ���ARR
            /* TIM3 enable counter */
            TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��3	
            break;
        //TIM4_CH1
		case PB6:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

            /* PWM�źŵ�ƽ����ֵ */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
            TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel1 */
            TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //����ΪPWMģʽ1
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
            TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
            TIM_OC1Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��1
            TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
            /* TIM4 enable counter */
            TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ��4	
            break;
		//TIM4_CH2
		case PB7:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

            /* PWM�źŵ�ƽ����ֵ */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
            TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel2 */
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
            TIM_OC2Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��4
            TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
            /* TIM3 enable counter */
            TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ��4	
            break;
		//TIM4_CH3
		case PB8:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

            /* PWM�źŵ�ƽ����ֵ */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
            TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel3 */
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
            TIM_OC3Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��4
            TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
            /* TIM3 enable counter */
            TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ��4	
            break;
		//TIM4_CH4
		case PB9:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

            /* PWM�źŵ�ƽ����ֵ */
            //u16 CCR1_Val = 500;        
            /* Time base configuration */
            TIM_TimeBaseStructure.TIM_Period = this->freq; //999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
            TIM_TimeBaseStructure.TIM_Prescaler = 0; //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
            TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

            /* PWM1 Mode configuration: Channel4 */
            TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
            TIM_OCInitStructure.TIM_Pulse = this->duty; //����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
            TIM_OC4Init(TIM4, &TIM_OCInitStructure); //ʹ��ͨ��4
            TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
            TIM_ARRPreloadConfig(TIM4, ENABLE); // ʹ��TIM4���ؼĴ���ARR
            /* TIM4 enable counter */
            TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ��4
            break;
		default:
            break;
    }

    this->SetModeAF_PP();
}

//�������ֵ
void PWM::SetOut(ushort pwmValue)
{
    this->duty = pwmValue;
    switch (this->pin)
    {
        //TIM2_CH1
		case PA0:
			TIM2->CCR1=this->duty;
            break;
		//TIM2_CH2
		case PA1:
			TIM2->CCR2=this->duty;
            break;
		//TIM2_CH3
		case PA2:
			TIM2->CCR3=this->duty;
            break;
		//TIM2_CH4
		case PA3:
			TIM2->CCR4=this->duty;
            break;
		//TIM3_CH1
		case PA6:
			TIM3->CCR1=this->duty;
            break;
        //TIM3_CH2
		case PA7:
			TIM3->CCR2=this->duty;
            break;
        //TIM3_CH3
		case PB0:
			TIM3->CCR3=this->duty;
            break;
        //TIM3_CH4
		case PB1:
			TIM3->CCR4=this->duty;
            break;
        //TIM4_CH1
		case PB6:
			TIM3->CCR1=this->duty;
            break;
		//TIM4_CH2
		case PB7:
			TIM3->CCR2=this->duty;
            break;
		//TIM4_CH3
		case PB8:
			TIM3->CCR3=this->duty;
            break;
		//TIM4_CH4
		case PB9:
			TIM3->CCR4=this->duty;
            break;
		default:
            break;
    }
}

//�������������0-100
void PWM::SetOutPercent(float per)
{
    this->duty = this->freq *per / 100;
    this->SetOut(this->duty);
}
