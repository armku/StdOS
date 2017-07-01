/*
Timer6 Timer7 Ϊ������ʱ����ֻ�߱������Ķ�ʱ����
Timer2 Timer3 Timer4 Timer5 Ϊͨ�ö�ʱ���������������PWM ���б������ӿ�
Timer1 Timer8 �߼���ʱ�������л�����ʱ�� ͨ�ö�ʱ�����й��ܣ��������������ɲ�����ܣ�PWM��������
*/
#include "Timer.h"
#include "stm32f10x_rcc.h"
#include "TInterrupt.h"

static Timer **Timers = NULL; // �Ѿ�ʵ�����Ķ�ʱ������
#define TIMS {TIM1,TIM2,TIM3,TIM4,TIM5}
static TIM_TypeDef *const g_Timers[] = TIMS;
const byte Timer::TimerCount = ArrayLength(g_Timers);

void Timer::SetHandler(bool set){

}

Timer::Timer(TIMER index)
{
    this->_index = index;
    this->Period = 1000; //Ĭ��1��һ��
    this->Prescaler = 71;
}

Timer::~Timer()
{
    if (Opened)
        Close();
    #if 0
        if (_Handler)
            Register(NULL);
    #endif 
    Timers[_index] = NULL;
}

void Timer::Open() // ��ʼ��ʱ��
{
    this->Config();
    this->OnOpen();
    #if DEBUG
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

        uint fre = clk / (Prescaler + 1) / Period;
        debug_printf("Timer%d::Start Prescaler=%d Period=%d Frequency=%d\r\n", _index + 1, Prescaler+1, Period, fre);
    #endif 
    Opened = true;
}

void Timer::Close() // ֹͣ��ʱ��
{
    if (!Opened)
        return ;

    debug_printf("Timer%d::Stop\r\n", _index + 1);

    // �ر�ʱ��
    ClockCmd(_index, false);
    //    TIM_ITConfig(_port, TIM_IT_Update, DISABLE);
    //    TIM_ClearITPendingBit(_port, TIM_IT_Update); // ������жϱ�־λ �رղ��ɿ�
    //    TIM_Cmd(_port, DISABLE);

    Opened = false;
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
    switch (this->_index)
    {
        case Timer1:
            break;
        case Timer2:     

            /* ����TIM2CLK Ϊ 72MHZ */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
            //TIM_DeInit(TIM2);

            /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
            TIM_TimeBaseStructure.TIM_Period = this->Period;

            /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
            /* ʱ��Ԥ��Ƶ��Ϊ72 */
            TIM_TimeBaseStructure.TIM_Prescaler = this->Prescaler;

            /* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
            TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

            TIM_ClearFlag(TIM2, TIM_FLAG_Update);

            TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
            TIM_ClearFlag(TIM2, TIM_FLAG_Update); // �����־λ  ����Ҫ�У��� ���� �����ж������жϸ��㿴
            TIM_Cmd(TIM2, ENABLE);

            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE); /*�ȹرյȴ�ʹ��*/
            Interrupt.SetPriority(28, 3);//TIM2_IRQn
            break;
        case Timer3:
			 /* ����TIM2CLK Ϊ 72MHZ */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
            //TIM_DeInit(TIM3);

            /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
            TIM_TimeBaseStructure.TIM_Period = this->Period;

            /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
            /* ʱ��Ԥ��Ƶ��Ϊ72 */
            TIM_TimeBaseStructure.TIM_Prescaler = this->Prescaler;

            /* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
            TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

            TIM_ClearFlag(TIM3, TIM_FLAG_Update);

            TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
            TIM_ClearFlag(TIM3, TIM_FLAG_Update); // �����־λ  ����Ҫ�У��� ���� �����ж������жϸ��㿴
            TIM_Cmd(TIM3, ENABLE);

            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE); /*�ȹرյȴ�ʹ��*/
            Interrupt.SetPriority(29, 3);//TIM3_IRQn
            break;
        case Timer4:
             /* ����TIM2CLK Ϊ 72MHZ */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
            //TIM_DeInit(TIM4);

            /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
            TIM_TimeBaseStructure.TIM_Period = this->Period;

            /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
            /* ʱ��Ԥ��Ƶ��Ϊ72 */
            TIM_TimeBaseStructure.TIM_Prescaler = this->Prescaler;

            /* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
            TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

            TIM_ClearFlag(TIM4, TIM_FLAG_Update);

            TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
            TIM_ClearFlag(TIM4, TIM_FLAG_Update); // �����־λ  ����Ҫ�У��� ���� �����ж������жϸ��㿴
            TIM_Cmd(TIM4, ENABLE);

            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE); /*�ȹرյȴ�ʹ��*/
            Interrupt.SetPriority(30, 3);//TIM4_IRQn
            break;
        case Timer5:
             /* ����TIM2CLK Ϊ 72MHZ */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
            //TIM_DeInit(TIM5);

            /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
            TIM_TimeBaseStructure.TIM_Period = this->Period;

            /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
            /* ʱ��Ԥ��Ƶ��Ϊ72 */
            TIM_TimeBaseStructure.TIM_Prescaler = this->Prescaler;

            /* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
            TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

            TIM_ClearFlag(TIM5, TIM_FLAG_Update);

            TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
            TIM_ClearFlag(TIM5, TIM_FLAG_Update); // �����־λ  ����Ҫ�У��� ���� �����ж������жϸ��㿴
            TIM_Cmd(TIM5, ENABLE);

            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, DISABLE); /*�ȹرյȴ�ʹ��*/
            Interrupt.SetPriority(50, 3);//TIM5_IRQn
            break;
        case Timer6:
             /* ����TIM2CLK Ϊ 72MHZ */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
            //TIM_DeInit(TIM6);

            /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
            TIM_TimeBaseStructure.TIM_Period = this->Period;

            /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
            /* ʱ��Ԥ��Ƶ��Ϊ72 */
            TIM_TimeBaseStructure.TIM_Prescaler = this->Prescaler;

            /* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
            TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

            TIM_ClearFlag(TIM6, TIM_FLAG_Update);

            TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
            TIM_ClearFlag(TIM6, TIM_FLAG_Update); // �����־λ  ����Ҫ�У��� ���� �����ж������жϸ��㿴
            TIM_Cmd(TIM6, ENABLE);

            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, DISABLE); /*�ȹرյȴ�ʹ��*/
            Interrupt.SetPriority(54, 3);//TIM6_IRQn
            break;
        case Timer7:
             /* ����TIM2CLK Ϊ 72MHZ */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
            //TIM_DeInit(TIM7);

            /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
            TIM_TimeBaseStructure.TIM_Period = this->Period;

            /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
            /* ʱ��Ԥ��Ƶ��Ϊ72 */
            TIM_TimeBaseStructure.TIM_Prescaler = this->Prescaler;

            /* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
            TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

            TIM_ClearFlag(TIM7, TIM_FLAG_Update);

            TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
            TIM_ClearFlag(TIM7, TIM_FLAG_Update); // �����־λ  ����Ҫ�У��� ���� �����ж������жϸ��㿴
            TIM_Cmd(TIM7, ENABLE);

            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, DISABLE); /*�ȹرյȴ�ʹ��*/
            Interrupt.SetPriority(55, 3);//TIM7_IRQn
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

    Prescaler = pre-1;
    Period = p;

    this->Open();
}

uint Timer::GetCounter()
{
    return this->Period;
}

void Timer::SetCounter(uint cnt) // ���ü�����ֵ
{
    this->Period = cnt;
}

void Timer::Register(const Delegate < Timer & >  &dlg)
{
    this->OnTick = dlg;
    switch (this->_index)
    {
        case Timer1:
            break;
        case Timer2:
            Interrupt.Activate(28, Timer::OnHandler, this);
            break;
        case Timer3:
            Interrupt.Activate(29, Timer::OnHandler, this);
            break;
        case Timer4:
            Interrupt.Activate(30, Timer::OnHandler, this);
            break;
        case Timer5:
            Interrupt.Activate(50, Timer::OnHandler, this);
            break;
        case Timer6:
            Interrupt.Activate(54, Timer::OnHandler, this);
            break;
        case Timer7:
            Interrupt.Activate(55, Timer::OnHandler, this);
            break;
        case Timer8:
            break;
        default:
            break;
    }
    //
}

void Timer::OnInterrupt()
{
    if (this->OnTick)
    {
        ((Action)this->OnTick.Method)(this->OnTick.Target);
    }
}

void Timer::ClockCmd(int idx, bool state)
{
    FunctionalState st = state ? ENABLE : DISABLE;
    switch (idx + 1)
    {
        case 1:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, st);
            break;
        case 2:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, st);
            break;
        case 3:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, st);
            break;
            #if defined(STM32F1) && defined(STM32F4)
            case 4:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, st);
                break;
            case 5:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, st);
                break;
            #endif 
        case 6:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, st);
            break;
            #if defined(STM32F1) && defined(STM32F4)
            case 7:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, st);
                break;
            case 8:
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, st);
                break;
            #endif 
            #ifdef STM32F4
            case 9:
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, st);
                break;
            case 10:
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, st);
                break;
            case 11:
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, st);
                break;
            case 12:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, st);
                break;
            case 13:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, st);
                break;
            case 14:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, st);
                break;
            #endif 
            #if defined(STM32F0)
            case 14:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, st);
                break;
            case 15:
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, st);
                break;
            case 16:
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, st);
                break;
            case 17:
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, st);
                break;
            #endif 
    }
}

void Timer::OnHandler(ushort num, void *param)
{
    Timer *timer = (Timer*)param;
    if (timer)
        timer->OnInterrupt();
}

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
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
            break;
        case Timer5:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
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
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE);
            break;
        case Timer5:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, DISABLE);
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
}

const void *Timer::GetTimer(byte idx)
{
    return nullptr;
}
//��ʱ������
extern uint time2cnt;
Delegate<Timer&> abc;
void tim2refesh(void * param)
{
	time2cnt++;
}
Timer *timer2;
void TimeTest()
{
	// ��ʼ��Ϊ���
	timer2=new Timer(Timer7);
	abc.Bind(tim2refesh);
	timer2->Register(abc);	
    timer2->Open();
//	timer2->SetFrequency(1);
//	timer2->Config();
}
