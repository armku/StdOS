/*
硬件定时器驱动，兼容STM32F0/F1/F4/GD32F10x/GD32F1x0
F0
基本定时器 
TIM6 TIM7 16为向上计数，仅仅计数功能
通用定时器 
TIM2	32位上下计数器，4捕获通道 0互补输出
TIM3	16位上下计数器，4捕获通道	0互补输出
TIM14	16位上计数器，1捕获通道	0互补输出
TIM15	16位上计数器，2捕获通道	1互补输出
TIM16 TIM17	16位上计数器，1捕获通道	1互补输出
高级定时器
TIM1	16位上下计数器，4捕获通道 3互补输出

F1:
Timer6 Timer7 为基本定时器，只具备基础的定时功能
Timer2 Timer3 Timer4 Timer5 为通用定时器，可以脉宽测量PWM 具有编码器接口
Timer1 Timer8 高级定时器，具有基本定时器 通用定时器所有功能，三相六步电机，刹车功能，PWM死区控制

F4
基本定时器
TIM6 TIM7
通用定时器
TIM2-TIM5 TIM9-TIM14
高级定时器
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

/* 定时器针脚 ------------------------------------------------------------------*/
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

static Timer **Timers = NULL; // 已经实例化的定时器对象
static TIM_TypeDef *const g_Timers[] = TIMS;
const byte Timer::TimerCount = ArrayLength(g_Timers);

void Timer::SetHandler(bool set){

}

Timer::Timer(TIMER index)
{
    this->_index = index;
    this->Period = 1000; //默认1秒一次
	#if defined(STM32F0)
		this->Prescaler = 47999; //48000分频
	#elif defined(STM32F1)
		this->Prescaler=36000-1;//则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=系统时钟/(分频系数+1) =1M
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

void Timer::Open() // 开始定时器
{
    this->Config();
    this->OnOpen();
    #if DEBUG
        // 获取当前频率
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

void Timer::Close() // 停止定时器
{
    if (!Opened)
        return ;

    debug_printf("Timer%d::Stop\r\n", _index + 1);

    // 关闭时钟
    ClockCmd(_index, false);
    //    TIM_ITConfig(_port, TIM_IT_Update, DISABLE);
    //    TIM_ClearITPendingBit(_port, TIM_IT_Update); // 仅清除中断标志位 关闭不可靠
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

// 创建指定索引的定时器，如果已有则直接返回，默认0xFF表示随机分配
Timer *Timer::Create(byte index)
{
    // 特殊处理随机分配
    if (index == 0xFF)
    {
        // 初始化静态数组
        if (!Timers)
        {
            Timers = new Timer *[TimerCount];
            #if 0
                ArrayZero2(Timers, TimerCount);
            #endif 
        }

        // 找到第一个可用的位置，没有被使用，并且该位置定时器存在
        byte i = 0;
        for (; i < TimerCount && (Timers[i] || !g_Timers[i]); i++)
            ;

        if (i >= TimerCount)
        {
            debug_printf("Timer::Create 失败！没有空闲定时器可用！\r\n");
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
//定时器测试
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
        // 初始化为输出
        timer2 = new Timer(Timer7);
        abc.Bind(tim2refesh);
        timer2->Register(abc);
        timer2->Open();
        //	timer2->SetFrequency(1);
        //	timer2->Config();
    }
#endif
