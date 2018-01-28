/*
Ӳ����ʱ������������STM32F0/F1/F4/GD32F10x/GD32F1x0
F0
������ʱ�� 
TIM6 TIM7 16Ϊ���ϼ�����������������
ͨ�ö�ʱ�� 
TIM2	32λ���¼�������4����ͨ�� 0�������
TIM3	16λ���¼�������4����ͨ��	0�������
TIM14	16λ�ϼ�������1����ͨ��	0�������
TIM15	16λ�ϼ�������2����ͨ��	1�������
TIM16 TIM17	16λ�ϼ�������1����ͨ��	1�������
�߼���ʱ��
TIM1	16λ���¼�������4����ͨ�� 3�������

F1:
Timer6 Timer7 Ϊ������ʱ����ֻ�߱������Ķ�ʱ����
Timer2 Timer3 Timer4 Timer5 Ϊͨ�ö�ʱ���������������PWM ���б������ӿ�
Timer1 Timer8 �߼���ʱ�������л�����ʱ�� ͨ�ö�ʱ�����й��ܣ��������������ɲ�����ܣ�PWM��������

F4
������ʱ��
TIM6 TIM7
ͨ�ö�ʱ��
TIM2-TIM5 TIM9-TIM14
�߼���ʱ��
TIM1 TIM8

*/
#include "Timer.h"
#include "TInterrupt.h"
#include "Platform\stm32.h"

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

        uint clk = clock.PCLK_Frequency << 1;
    
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
            break;
        case Timer5:
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
                
			   Interrupt.SetPriority(TIM6_DAC_IRQn, 3);
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
			#if 0
            Interrupt.SetPriority(TIM7_IRQn, 3); //TIM7_IRQn
			#endif
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
void Timer::OnInit()
{
		this->Prescaler = 47999; //48000��Ƶ	
}

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
            break;
        case Timer5:
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
	#if DEBUG
        // ��ȡ��ǰƵ��
        RCC_ClocksTypeDef clock;
        RCC_GetClocksFreq(&clock);

            uint clk = clock.PCLK_Frequency << 1;

        uint fre = clk / (Prescaler + 1) / Period;
        debug_printf("Timer%d::Start Prescaler=%d Period=%d Frequency=%d\r\n", _index + 1, Prescaler + 1, Period, fre);
    #endif 
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
            break;
        case Timer5:
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
            break;
        case Timer5:
            break;
        case Timer6:
            break;
        case Timer7:
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
/////////////////////////////////����Ϊ���////////////////////

static TIM_TypeDef *const g_Timers[] = TIMS;
const byte Timer::TimerCount = ArrayLength(g_Timers);
extern Timer **Timers; // �Ѿ�ʵ�����Ķ�ʱ������
// ����ָ�������Ķ�ʱ�������������ֱ�ӷ��أ�Ĭ��0xFF��ʾ�������
Timer *Timer::Create(byte index)
{
    // ���⴦���������
    if (index == 0xFF)
    {
        // ��ʼ����̬����
        if (!Timers)
        {
            Timers = new Timer *[TimerCount];
            #if 0
                ArrayZero2(Timers, TimerCount);
            #endif 
        }

        // �ҵ���һ�����õ�λ�ã�û�б�ʹ�ã����Ҹ�λ�ö�ʱ������
        byte i = 0;
        for (; i < TimerCount && (Timers[i] || !g_Timers[i]); i++)
            ;

        if (i >= TimerCount)
        {
            debug_printf("Timer::Create ʧ�ܣ�û�п��ж�ʱ�����ã�\r\n");
            return NULL;
        }

        index = i;
    }

    assert_param(index < TimerCount);

    if (Timers[index])
        return Timers[index];
    else
        return new Timer((TIMER)index);
}

void Timer::Register(const Delegate < Timer & >  &dlg)
{
    this->OnTick = dlg;
    switch (this->_index)
    {
        case Timer1:
            break;
        case Timer2:
            Interrupt.Activate(TIM2_IRQn, Timer::OnHandler, this);
            break;
        case Timer3:
            Interrupt.Activate(TIM3_IRQn, Timer::OnHandler, this);
            break;
        case Timer4:
            break;
        case Timer5:
            break;
        case Timer6:
            	Interrupt.Activate(TIM6_DAC_IRQn, Timer::OnHandler, this);
            break;
        case Timer7:
			#if 0
            Interrupt.Activate(TIM7_IRQn, Timer::OnHandler, this);
			#endif
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
				Interrupt.Activate(TIM14_IRQn, Timer::OnHandler, this);
            break;
        case Timer15:
            	Interrupt.Activate(TIM15_IRQn, Timer::OnHandler, this);
            break;
        case Timer16:
            	Interrupt.Activate(TIM16_IRQn, Timer::OnHandler, this);
            break;
        case Timer17:
            	Interrupt.Activate(TIM17_IRQn, Timer::OnHandler, this);
            break;
        case Timer18:
            break;
        default:
            break;
    }
}
