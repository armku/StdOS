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

#ifdef STM32F0
    #include "stm32f0xx.h"
#elif defined STM32F1
    #include "stm32f10x.h"
#elif defined STM32F4
    #include "stm32f4xx.h"
#endif 

/* ��ʱ����� ------------------------------------------------------------------*/
#if defined(STM32F1) || defined(STM32F4) 
    #define TIMS {TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8}
#elif defined STM32F0
    #define TIMS {TIM1, TIM2, TIM3}
#endif 
#define TIM_PINS {\
PA8, PA9, PA10,PA11,/* TIM1 */	\
PA0, PA1, PA2, PA3, /* TIM2 */	\
PA6, PA7, PB0, PB1, /* TIM3 */	\
PB6, PB7, PB8, PB9, /* TIM4 */	\
PA0, PA1, PA2, PA3, /* TIM5 */	\
P0,  P0,  P0,  P0,	/* TIM6 */	\
P0,  P0,  P0,  P0,	/* TIM7 */	\
PC6, PC7, PC8, PC9	/* TIM8 */	\
}
#define TIM_PINS_FULLREMAP {\
PE9, PE11,PE13,PE14,/* TIM1 AFIO_MAPR_TIM1_REMAP_FULLREMAP */	\
PA15,PB3, PB10,PB11,/* TIM2 AFIO_MAPR_TIM2_REMAP_FULLREMAP */	\
PC6, PC7, PC8, PC9, /* TIM3 AFIO_MAPR_TIM3_REMAP_FULLREMAP */	\
PD12,PD13,PD14,PD15,/* TIM4 AFIO_MAPR_TIM4_REMAP */	\
PA0, PA1, PA2, PA3, /* TIM5 */	\
P0,  P0,  P0,  P0,	/* TIM6 */	\
P0,  P0,  P0,  P0,	/* TIM7 */	\
PC6, PC7, PC8, PC9	/* TIM8 */	\
}

static Timer **Timers = NULL; // �Ѿ�ʵ�����Ķ�ʱ������
static TIM_TypeDef *const g_Timers[] = TIMS;
const byte Timer::TimerCount = ArrayLength(g_Timers);

void Timer::SetHandler(bool set){

}

Timer::Timer(TIMER index)
{
    this->_index = index;
    this->Period = 1000; //Ĭ��1��һ��
	#if defined(STM32F0)
		this->Prescaler = 47999; //48000��Ƶ
	#elif defined(STM32F1)
		this->Prescaler=36000-1;//��������������ʱ��CK_CNT = CK_INT / (71+1)=ϵͳʱ��/(��Ƶϵ��+1) =1M
		this->Period=2000-1;
	#elif defined(STM32F4)
		this->Prescaler = 5000-1;
		this->Period= 8400-1;
	#endif    
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
        debug_printf("Timer%d::Start Prescaler=%d Period=%d Frequency=%d\r\n", _index + 1, Prescaler + 1, Period, fre);
    #endif 
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
    this->OnClose();
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
            #if defined(STM32F1) || defined(STM32F4)
                Interrupt.Activate(TIM4_IRQn, Timer::OnHandler, this);
            #endif 
            break;
        case Timer5:
            #if defined(STM32F1) || defined(STM32F4)
                Interrupt.Activate(TIM5_IRQn, Timer::OnHandler, this);
            #endif 
            break;
        case Timer6:
            #if defined(STM32F1) 
                Interrupt.Activate(TIM6_IRQn, Timer::OnHandler, this);
			#elif defined STM32F0 || defined(STM32F4)
				Interrupt.Activate(TIM6_DAC_IRQn, Timer::OnHandler, this);
            #endif 
            break;
        case Timer7:
            Interrupt.Activate(TIM7_IRQn, Timer::OnHandler, this);
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
			#if defined STM32F0
				Interrupt.Activate(TIM14_IRQn, Timer::OnHandler, this);
            #endif 
            break;
        case Timer15:
            #if defined STM32F0
				Interrupt.Activate(TIM15_IRQn, Timer::OnHandler, this);
            #endif 
            break;
        case Timer16:
            #if defined STM32F0
				Interrupt.Activate(TIM16_IRQn, Timer::OnHandler, this);
            #endif 
            break;
        case Timer17:
            #if defined STM32F0
				Interrupt.Activate(TIM17_IRQn, Timer::OnHandler, this);
            #endif 
            break;
        case Timer18:
            break;
        default:
            break;
    }
}

void Timer::OnInterrupt()
{
    if (this->OnTick)
    {
        ((Action)this->OnTick.Method)(this->OnTick.Target);
    }
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
//��ʱ������
#if 0
    extern uint time2cnt;
    Delegate < Timer & > abc;
    void tim2refesh(void *param)
    {
        time2cnt++;
    }
    Timer *timer2;
    void TimeTest()
    {
        // ��ʼ��Ϊ���
        timer2 = new Timer(Timer7);
        abc.Bind(tim2refesh);
        timer2->Register(abc);
        timer2->Open();
        //	timer2->SetFrequency(1);
        //	timer2->Config();
    }
#endif
