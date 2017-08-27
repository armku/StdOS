#include "Timer.h"
#include "TInterrupt.h"

#ifdef STM32F0
    #include "stm32f0xx.h"
#elif defined STM32F1
    #include "stm32f10x.h"
#elif defined STM32F4
    #include "stm32f4xx.h"
#endif 


uint Timer::GetCounter()
{
    return this->Period;
}
void Timer::SetCounter(uint cnt) // ���ü�����ֵ
{
    this->Period = cnt;
}

// ����Ƶ�ʣ��Զ�����Ԥ��Ƶ
void Timer::SetFrequency(uint frequency)
{
    // ��ȡ��ǰƵ��
    RCC_ClocksTypeDef clock;
    RCC_GetClocksFreq(&clock);

    #if defined(STM32F1) || defined(STM32F4)
        uint clk = clock.PCLK1_Frequency;
        if ((uint)this->_index &0x00010000)
            clk = clock.PCLK2_Frequency;
        clk <<= 1;
    #elif defined(STM32F0)
        uint clk = clock.PCLK_Frequency << 1;
    #endif 

    assert_param(frequency > 0 && frequency <= clk);

    uint s = 1;
    uint p = s / frequency;

    uint pre = clk / s; // prescaler

    //while (pre >= 0x10000 || p == 0) { // prescaler ̫��
    // ���ڸպ�Ϊ1Ҳ���У����õ�ʱ����Ҫ�ȼ�ȥ1���ͱ����0
    while (pre >= 0x10000 || p <= 1)
    {
        // prescaler ̫��
        if (p >= 0x80000000)
            return ;
        s *= 10;
        pre /= 10;
        p = s / frequency;
    }

    if (_index + 1 != 2 && _index + 1 != 5)
    {
        // 16 bit timer
        while (p >= 0x10000)
        {
            // period too large
            if (pre > 0x8000)
                return ;
            pre <<= 1;
            p >>= 1;
        }
    }

    Prescaler = pre - 1;
    Period = p;
    this->Open();
}
/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * �ж�����Ϊ = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 */
void Timer::Config()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Period = this->Period;
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    /* ʱ��Ԥ��Ƶ��Ϊ72 */
    TIM_TimeBaseStructure.TIM_Prescaler = this->Prescaler;

    /* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    switch (this->_index)
    {
        case Timer1:
            break;
        case Timer2:
            /* ����TIM2CLK Ϊ 72MHZ */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
            TIM_DeInit(TIM2);

            TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

            TIM_ClearFlag(TIM2, TIM_FLAG_Update);

            TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
            TIM_ClearFlag(TIM2, TIM_FLAG_Update); // �����־λ  ����Ҫ�У��� ���� �����ж������жϸ��㿴
            TIM_Cmd(TIM2, ENABLE);

            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE); /*�ȹرյȴ�ʹ��*/
            Interrupt.SetPriority(TIM2_IRQn, 3); 
            break;
        case Timer3:
            /* ����TIM2CLK Ϊ 72MHZ */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
            TIM_DeInit(TIM3);

            TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

            TIM_ClearFlag(TIM3, TIM_FLAG_Update);

            TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
            TIM_ClearFlag(TIM3, TIM_FLAG_Update); // �����־λ  ����Ҫ�У��� ���� �����ж������жϸ��㿴
            TIM_Cmd(TIM3, ENABLE);

            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE); /*�ȹرյȴ�ʹ��*/
            Interrupt.SetPriority(TIM3_IRQn, 3);
            break;
        case Timer4:
            #if defined(STM32F1) || defined(STM32F4)
                /* ����TIM2CLK Ϊ 72MHZ */
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
                TIM_DeInit(TIM4);

                TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

                TIM_ClearFlag(TIM4, TIM_FLAG_Update);

                TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
                TIM_ClearFlag(TIM4, TIM_FLAG_Update); // �����־λ  ����Ҫ�У��� ���� �����ж������жϸ��㿴
                TIM_Cmd(TIM4, ENABLE);

                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE); /*�ȹرյȴ�ʹ��*/
                Interrupt.SetPriority(TIM4_IRQn, 3);
            #endif 
            break;
        case Timer5:
            #if defined(STM32F1) || defined(STM32F4)
                /* ����TIM2CLK Ϊ 72MHZ */
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
                TIM_DeInit(TIM5);

                TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

                TIM_ClearFlag(TIM5, TIM_FLAG_Update);

                TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
                TIM_ClearFlag(TIM5, TIM_FLAG_Update); // �����־λ  ����Ҫ�У��� ���� �����ж������жϸ��㿴
                TIM_Cmd(TIM5, ENABLE);

                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, DISABLE); /*�ȹرյȴ�ʹ��*/
                Interrupt.SetPriority(TIM5_IRQn, 3);
            #endif 
            break;
        case Timer6:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
            TIM_DeInit(TIM6);
			TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
			TIM_ClearFlag(TIM6, TIM_FLAG_Update);
			TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
			TIM_ClearFlag(TIM6, TIM_FLAG_Update); // �����־λ  ����Ҫ�У��� ���� �����ж������жϸ��㿴
            TIM_Cmd(TIM6, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, DISABLE); /*�ȹرյȴ�ʹ��*/
                
			#if defined(STM32F1)
                Interrupt.SetPriority(TIM6_IRQn, 3);
            #elif defined STM32F4
				Interrupt.SetPriority(TIM6_DAC_IRQn, 3);
			#elif defined STM32F0
                Interrupt.SetPriority(TIM6_DAC_IRQn, 3);
            #endif 
            break;
        case Timer7:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
            TIM_DeInit(TIM7);
            TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
            TIM_ClearFlag(TIM7, TIM_FLAG_Update);
            TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
            TIM_ClearFlag(TIM7, TIM_FLAG_Update); // �����־λ  ����Ҫ�У��� ���� �����ж������жϸ��㿴
            TIM_Cmd(TIM7, ENABLE);
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, DISABLE); /*�ȹرյȴ�ʹ��*/
            Interrupt.SetPriority(TIM7_IRQn, 3); //TIM7_IRQn
            break;
        case Timer8:
            break;
        case Timer9:
            break;
        case Timer10:
            break;
        case Timer11:
            break;
        case Timer12:
            break;
        case Timer13:
            break;
        case Timer14:
            break;
        case Timer15:
            break;
        case Timer16:
            break;
        case Timer17:
            break;
        case Timer18:
            break;
        default:
            break;
    }
}
void Timer::OnInit(){}

void Timer::OnOpen()
{
    switch (this->_index)
    {
        case Timer1:
            break;
        case Timer2:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
            break;
        case Timer3:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
            break;
        case Timer4:
            #if defined(STM32F1) || defined(STM32F4)
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
            #endif 
            break;
        case Timer5:
            #if defined(STM32F1) || defined(STM32F4)
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
            #endif 
            break;
        case Timer6:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
            break;
        case Timer7:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
            break;
        case Timer8:
            break;
        case Timer9:
            break;
        case Timer10:
            break;
        case Timer11:
            break;
        case Timer12:
            break;
        case Timer13:
            break;
        case Timer14:
            break;
        case Timer15:
            break;
        case Timer16:
            break;
        case Timer17:
            break;
        case Timer18:
            break;
        default:
            break;
    }
    this->Opened = true;
}

void Timer::OnClose()
{
    switch (this->_index)
    {
        case Timer1:
            break;
        case Timer2:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
            break;
        case Timer3:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);
            break;
        case Timer4:
            #if defined(STM32F1) || defined(STM32F4)
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE);
            #endif 
            break;
        case Timer5:
            #if defined(STM32F1) || defined(STM32F4)
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, DISABLE);
            #endif 
            break;
        case Timer6:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, DISABLE);
            break;
        case Timer7:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, DISABLE);
            break;
        case Timer8:
            break;
        case Timer9:
            break;
        case Timer10:
            break;
        case Timer11:
            break;
        case Timer12:
            break;
        case Timer13:
            break;
        case Timer14:
            break;
        case Timer15:
            break;
        case Timer16:
            break;
        case Timer17:
            break;
        case Timer18:
            break;
        default:
            break;
    }
    Opened = false;
}

void Timer::ClockCmd(int idx, bool state)
{
    FunctionalState st = state ? ENABLE : DISABLE;
    switch (idx)
    {
        case Timer1:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, st);
            break;
        case Timer2:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, st);
            break;
        case Timer3:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, st);
            break;
        case Timer4:
            #if defined(STM32F1) && defined(STM32F4)
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, st);
            #endif 
            break;
        case Timer5:
            #if defined(STM32F1) && defined(STM32F4)
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, st);
            #endif 
            break;
        case Timer6:
            #if defined(STM32F1) && defined(STM32F4)
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, st);
            #endif 
            break;
        case Timer7:
            #if defined(STM32F1) && defined(STM32F4)
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, st);
            #endif 
            break;
        case Timer8:
            #if defined(STM32F1) && defined(STM32F4)
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, st);
            #endif 
            break;
        case Timer9:
            #if defined(STM32F1) && defined(STM32F4)
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, st);
            #endif 
            break;
        case Timer10:
            #if defined(STM32F1) && defined(STM32F4)
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, st);
            #endif 
            break;
        case Timer11:
            #if defined(STM32F1) && defined(STM32F4)
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, st);
            #endif 
            break;
        case Timer12:
            #if defined(STM32F1) && defined(STM32F4)
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, st);
            #endif 
            break;
        case Timer13:
            #if defined(STM32F1) && defined(STM32F4)
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, st);
            #endif 
            break;
        case Timer14:
            #if defined(STM32F1) && defined(STM32F4)
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, st);
            #endif 
            break;
        case Timer15:
            #if defined(STM32F1) && defined(STM32F4)
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, st);
            #endif 
            break;
        case Timer16:
            #if defined(STM32F1) && defined(STM32F4)
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, st);
            #endif 
            break;
        case Timer17:
            #if defined(STM32F1) && defined(STM32F4)
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, st);
            #endif 
            break;
        case Timer18:
			#if defined(STM32F1) && defined(STM32F4)
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM18, st);
            #endif 
            break;
		default:
            break;
    }
}
const void *Timer::GetTimer(byte idx)
{
    return nullptr;
}

void Timer::OnHandler(ushort num, void *param)
{
    Timer *timer = (Timer*)param;
    if (timer)
        timer->OnInterrupt();
}
