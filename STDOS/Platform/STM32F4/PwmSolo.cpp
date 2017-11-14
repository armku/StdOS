#include "PwmSolo.h"
#include "stm32f4xx.h"


//TIM14 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ�� F8 TIM11-1
void TIM11_PWM_Init(u32 arr, u32 psc)
{

    //�˲������ֶ��޸�IO������

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE); //TIM11ʱ��ʹ��    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); //ʹ��PORTFʱ��	

    GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_TIM11); //GPIOF9����Ϊ��ʱ��11

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //GPIOF7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //�ٶ�100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
    GPIO_Init(GPIOF, &GPIO_InitStructure); //��ʼ��PF7

    TIM_TimeBaseStructure.TIM_Prescaler = psc; //��ʱ����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_Period = arr; //�Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure); //��ʼ����ʱ��11

    //��ʼ��TIM11 Channel1 PWMģʽ	 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
    TIM_OC1Init(TIM13, &TIM_OCInitStructure); //����Tָ���Ĳ�����ʼ������TIM1 4OC1

    TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable); //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���

    TIM_ARRPreloadConfig(TIM13, ENABLE); //ARPEʹ�� 

    TIM_Cmd(TIM13, ENABLE); //ʹ��TIM11
}

void TIM14_PWM_Init(u32 arr, u32 psc)
{

    //�˲������ֶ��޸�IO������

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

void PwmSolo::OnOpen2()
{
    #if 1

        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
        TIM_OCInitTypeDef TIM_OCInitStructure;

        TIM_TimeBaseStructure.TIM_Prescaler = 10000; //��ʱ����Ƶ
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
        TIM_TimeBaseStructure.TIM_Period = 500; //�Զ���װ��ֵ
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;



        //��ʼ��TIM11 Channel1 PWMģʽ	 
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�



        switch (this->_Pin)
        {
            //TIM10_CH1
            case PF6:
                RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE); //TIM10ʱ��ʹ��    
                GPIO_PinAFConfig(GPIOF, GPIO_PinSource6, GPIO_AF_TIM10); //GPIOF6����Ϊ��ʱ��10

                TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure); //��ʼ����ʱ��10
                TIM_OC1Init(TIM10, &TIM_OCInitStructure); //����Tָ���Ĳ�����ʼ������TIM10 OC1

                TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable); //ʹ��TIM10��CCR1�ϵ�Ԥװ�ؼĴ���

                TIM_ARRPreloadConfig(TIM10, ENABLE); //ARPEʹ�� 

                TIM_Cmd(TIM10, ENABLE); //ʹ��TIM10
                break;
                //TIM11-CH1
            case PF7:
                RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);    
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
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);    
                GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);

                TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
                TIM_OC1Init(TIM14, &TIM_OCInitStructure);

                TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
                TIM_ARRPreloadConfig(TIM14, ENABLE);
                TIM_Cmd(TIM14, ENABLE);
                break;
            default:
                break;
        }



















    #else 
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
    #endif 
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
