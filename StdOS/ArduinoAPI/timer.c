#include "timer.h"

typedef enum {
    TIMER1, TIMER2, TIMER3, TIMER4, 
    TIMER5, TIMER6, TIMER7, TIMER8, 
    TIMER_MAX
}TIMERx_Type;//��ʱ�����ö��

static Timer_CallbackFunction_t TIMx_Function[TIMER_MAX] = {0};//��ʱ�жϻص�����ָ������

/**
  * @brief  ��ʱ�ж�����
  * @param  TIMx:��ʱ����ַ
  * @param  InterruptTime_us: �ж�ʱ��(΢��)
  * @param  function: ��ʱ�жϻص�����
  * @retval ��
  */
void TimerSet(TIM_TypeDef* TIMx, uint32_t InterruptTime_us, Timer_CallbackFunction_t function)
{
    Timer_Init(
        TIMx, 
        InterruptTime_us, 
        function, 
        Timer_PreemptionPriority_Default, 
        Timer_SubPriority_Default
    );
}

/**
  * @brief  ���¶�ʱ�ж�ʱ��
  * @param  TIMx:��ʱ����ַ
  * @param  InterruptTime_us: �ж�ʱ��(΢��)
  * @retval ��
  */
void TimerSet_InterruptTimeUpdate(TIM_TypeDef* TIMx, uint32_t InterruptTime_us)
{
	uint32_t arr, psc;
	//Calculate TIM_Period and TIM_Prescaler
    InterruptTime_us *= CYCLES_PER_MICROSECOND;
    if(InterruptTime_us < CYCLES_PER_MICROSECOND * 30)
    {
        arr = 10;
        psc = InterruptTime_us / arr;
    }
    else if(InterruptTime_us < 65535 * 1000)
    {
        arr = InterruptTime_us / 1000;
        psc = InterruptTime_us / arr;
    }
    else
    {
        arr = InterruptTime_us / 20000;
        psc = InterruptTime_us / arr;
    }
	
	TIMx->ARR = arr - 1;
    TIMx->PSC = psc - 1;
	TIMx->EGR = TIM_PSCReloadMode_Immediate;
}

/**
  * @brief  ��ʱ�ж�����
  * @param  TIMx:��ʱ����ַ
  * @param  InterruptTime_us: �ж�ʱ��(΢��)
  * @param  function: ��ʱ�жϻص�����
  * @param  PreemptionPriority: ��ռ���ȼ�
  * @param  SubPriority: �����ȼ�
  * @retval ��
  */
void Timer_Init(TIM_TypeDef* TIMx, uint32_t InterruptTime_us, Timer_CallbackFunction_t function, uint8_t PreemptionPriority, uint8_t SubPriority)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    uint32_t arr, psc;
    uint32_t RCC_APBxPeriph_TIMx;
    IRQn_Type TIMx_IRQn;
    TIMERx_Type TIMERx;

    if(!IS_TIM_ALL_PERIPH(TIMx))return;

    if(TIMx == TIM1)
    {
        TIMERx = TIMER1;
        RCC_APBxPeriph_TIMx = RCC_APB2Periph_TIM1;
        TIMx_IRQn = TIM1_UP_IRQn;
    }
    else if(TIMx == TIM2)
    {
        TIMERx = TIMER2;
        RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM2;
        TIMx_IRQn = TIM2_IRQn;
    }
    else if(TIMx == TIM3)
    {
        TIMERx = TIMER3;
        RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM3;
        TIMx_IRQn = TIM3_IRQn;
    }
    else if(TIMx == TIM4)
    {
        TIMERx = TIMER4;
        RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM4;
        TIMx_IRQn = TIM4_IRQn;
    }
#ifdef STM32F10X_HD
    else if(TIMx == TIM5)
    {
        TIMERx = TIMER5;
        RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM5;
        TIMx_IRQn = TIM5_IRQn;
    }
    else if(TIMx == TIM6)
    {
        TIMERx = TIMER6;
        RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM6;
        TIMx_IRQn = TIM6_IRQn;
    }
    else if(TIMx == TIM7)
    {
        TIMERx = TIMER5;
        RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM7;
        TIMx_IRQn = TIM7_IRQn;
    }
    else if(TIMx == TIM8)
    {
        TIMERx = TIMER8;
        RCC_APBxPeriph_TIMx = RCC_APB2Periph_TIM8;
        TIMx_IRQn = TIM8_UP_IRQn;
    }
#endif

    TIMx_Function[TIMERx] = function;//Callback Functions

    //Calculate TIM_Period and TIM_Prescaler
    InterruptTime_us *= CYCLES_PER_MICROSECOND;
    if(InterruptTime_us < CYCLES_PER_MICROSECOND * 30)
    {
        arr = 10;
        psc = InterruptTime_us / arr;
    }
    else if(InterruptTime_us < 65535 * 1000)
    {
        arr = InterruptTime_us / 1000;
        psc = InterruptTime_us / arr;
    }
    else
    {
        arr = InterruptTime_us / 20000;
        psc = InterruptTime_us / arr;
    }

    //Enable PeriphClock
    TIM_DeInit(TIMx);
    if(TIMx == TIM1 || TIMx == TIM8)
    {
        RCC_APB2PeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE); //ʱ��ʹ��
    }
    
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseStructure.TIM_Period = arr - 1; 		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1; 	//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    /**********************************�����ж����ȼ�************************************/
    NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;  //TIM�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;  //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;  //�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
  
    TIM_ClearFlag(TIMx, TIM_FLAG_Update);
    TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
    TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);	//ʹ��TIM�ж�
}

/**
  * @brief  ��ʱ�ж���ڣ���ʱ��1
  * @param  ��
  * @retval ��
  */
void TIM1_UP_IRQHandler1(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) 		//���ָ����TIM�жϷ������:TIM �ж�Դ
    {
        if(TIMx_Function[TIMER1]) TIMx_Function[TIMER1]();
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  	//���TIMx���жϴ�����λ:TIM �ж�Դ
    }
}

/**
  * @brief  ��ʱ�ж���ڣ���ʱ��2
  * @param  ��
  * @retval ��
  */
void TIM2_IRQHandler1(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        if(TIMx_Function[TIMER2]) TIMx_Function[TIMER2]();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

/**
  * @brief  ��ʱ�ж���ڣ���ʱ��3
  * @param  ��
  * @retval ��
  */
void TIM3_IRQHandler1(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        if(TIMx_Function[TIMER3]) TIMx_Function[TIMER3]();
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

/**
  * @brief  ��ʱ�ж���ڣ���ʱ��4
  * @param  ��
  * @retval ��
  */
void TIM4_IRQHandler1(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        if(TIMx_Function[TIMER4]) TIMx_Function[TIMER4]();
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}

/**
  * @brief  ��ʱ�ж���ڣ���ʱ��5
  * @param  ��
  * @retval ��
  */
void TIM5_IRQHandler1(void)
{
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        if(TIMx_Function[TIMER5]) TIMx_Function[TIMER5]();
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    }
}

/**
  * @brief  ��ʱ�ж���ڣ���ʱ��6
  * @param  ��
  * @retval ��
  */
void TIM6_IRQHandler1(void)
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
        if(TIMx_Function[TIMER6]) TIMx_Function[TIMER6]();
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }
}

/**
  * @brief  ��ʱ�ж���ڣ���ʱ��7
  * @param  ��
  * @retval ��
  */
void TIM7_IRQHandler1(void)
{
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
    {
        if(TIMx_Function[TIMER7]) TIMx_Function[TIMER7]();
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    }
}

/**
  * @brief  ��ʱ�ж���ڣ���ʱ��8
  * @param  ��
  * @retval ��
  */
void TIM8_UP_IRQHandler1(void)
{
    if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
    {
        if(TIMx_Function[TIMER8]) TIMx_Function[TIMER8]();
        TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
    }
}
