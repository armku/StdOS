#include "pwm.h"
#include "Arduino.h"

/**
  * @brief  ��ʱ����������ʼ��
  * @param  TIMx: ��ʱ����ַ
  * @param  arr: �Զ���װֵ
  * @param  psc: ʱ��Ԥ��Ƶ��
  * @param  TimerChannel: ��ʱ��ͨ��
  * @retval ��
  */
void TIMx_Init(TIM_TypeDef* TIMx, uint16_t arr, uint16_t psc, uint8_t TimerChannel)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    if(TIMx == TIM1)RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    else if(TIMx == TIM2)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    else if(TIMx == TIM3)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    else if(TIMx == TIM4)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	else if(TIMx == TIM8)RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	if(TIMx == TIM1 || TIMx == TIM8)
	{
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	}
    switch(TimerChannel)
    {
    case 1:
        TIM_OC1Init(TIMx, &TIM_OCInitStructure);
        TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
        break;
    case 2:
        TIM_OC2Init(TIMx, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
        break;
    case 3:
        TIM_OC3Init(TIMx, &TIM_OCInitStructure);
        TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
        break;
    case 4:
        TIM_OC4Init(TIMx, &TIM_OCInitStructure);
        TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
        break;
    }

    TIM_Cmd(TIMx, ENABLE);
    if(TIMx == TIM1 || TIMx == TIM8)TIM_CtrlPWMOutputs(TIMx, ENABLE);
}

/**
  * @brief  PWM�����ʼ��
  * @param  Pin:���ű��
  * @param  PWM_DutyCycle: PWM�ּ���
  * @param  PWM_Frequency: PWMƵ��
  * @retval ���Ŷ�Ӧ�Ķ�ʱ��ͨ��
  */
uint8_t PWM_Init(uint8_t Pin, uint16_t PWM_DutyCycle, uint16_t PWM_Frequency)
{
    uint32_t arr, psc;

    if(!IS_PWM_PIN(Pin))return 0;

    pinMode(Pin, GPIO_Mode_AF_PP);

    arr = PWM_DutyCycle;
    psc = F_CPU / PWM_DutyCycle / PWM_Frequency;

    TIM_Cmd(PIN_MAP[Pin].TIMx, DISABLE);
    TIMx_Init(PIN_MAP[Pin].TIMx, arr - 1, psc - 1, PIN_MAP[Pin].TimerChannel);
    return PIN_MAP[Pin].TimerChannel;
}

/**
  * @brief  ���PWM�ź�
  * @param  Pin: ���ű��
  * @param  val:PWMռ�ձ�ֵ
  * @retval PWMռ�ձ�ֵ
  */
uint16_t pwmWrite(uint8_t Pin, uint16_t val)
{
    switch(PIN_MAP[Pin].TimerChannel)
    {
    case 1:
        PIN_MAP[Pin].TIMx->CCR1 = val;
        break;
    case 2:
        PIN_MAP[Pin].TIMx->CCR2 = val;
        break;
    case 3:
        PIN_MAP[Pin].TIMx->CCR3 = val;
        break;
    case 4:
        PIN_MAP[Pin].TIMx->CCR4 = val;
        break;
    }
    return val;
}

/**
  * @brief  ��ȡ����ֵ
  * @param  TIMx: ��ʱ����ַ
  * @param  TimerChannel: ��ʱ��ͨ��
  * @retval ����ֵ
  */
uint16_t timer_get_compare(TIM_TypeDef* TIMx, uint8_t TimerChannel)
{
    uint16_t compare = 0;
    switch(TimerChannel)
    {
    case 1:
        compare = TIMx->CCR1;
        break;
    case 2:
        compare = TIMx->CCR2;
        break;
    case 3:
        compare = TIMx->CCR3;
        break;
    case 4:
        compare = TIMx->CCR4;
        break;
    }
    return compare;
}

/**
  * @brief  ���¶�ʱ��ʱ��Ԥ��Ƶ��
  * @param  TIMx: ��ʱ����ַ
  * @param  psc: ʱ��Ԥ��Ƶ��
  * @retval ��
  */
void timer_set_prescaler(TIM_TypeDef* TIMx, uint16_t psc)
{
    TIMx->PSC = psc;
}

/**
  * @brief  ���¶�ʱ���Զ���װֵ
  * @param  TIMx: ��ʱ����ַ
  * @param  arr: �Զ���װֵ
  * @retval ��
  */
void timer_set_reload(TIM_TypeDef* TIMx, uint16_t arr)
{
    TIMx->ARR = arr;
}

/**
  * @brief  Ӧ�ö�ʱ������
  * @param  TIMx: ��ʱ����ַ
  * @retval ��
  */
void timer_generate_update(TIM_TypeDef* TIMx)
{
    TIMx->EGR = TIM_PSCReloadMode_Immediate;
}
