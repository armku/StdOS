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
//#include "_Core.h"
#include "stm32f10x.h"

uint Timer::GetCounter()
{
    return this->Period;
}
void Timer::SetCounter(uint cnt) // 设置计数器值
{
    this->Period = cnt;
}

// 设置频率，自动计算预分频
void Timer::SetFrequency(uint frequency)
{
    // 获取当前频率
    RCC_ClocksTypeDef clock;
    RCC_GetClocksFreq(&clock);

        uint clk = clock.PCLK1_Frequency;
        if ((uint)this->_index &0x00010000)
            clk = clock.PCLK2_Frequency;
        clk <<= 1;
    
    assert_param(frequency > 0 && frequency <= clk);

    uint s = 1;
    uint p = s / frequency;

    uint pre = clk / s; // prescaler

    //while (pre >= 0x10000 || p == 0) { // prescaler 太大
    // 周期刚好为1也不行，配置的时候需要先减去1，就变成了0
    while (pre >= 0x10000 || p <= 1)
    {
        // prescaler 太大
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
 * 中断周期为 = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数 
 */
void Timer::Config()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Period = this->Period;
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    /* 时钟预分频数为72 */
    TIM_TimeBaseStructure.TIM_Prescaler = this->Prescaler;

    /* 对外部时钟进行采样的时钟分频,这里没有用到 */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    switch (this->_index)
    {
        case Timer1:
            break;
        case Timer2:
            /* 设置TIM2CLK 为 72MHZ */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
            TIM_DeInit(TIM2);

            TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

            TIM_ClearFlag(TIM2, TIM_FLAG_Update);

            TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
            TIM_ClearFlag(TIM2, TIM_FLAG_Update); // 清除标志位  必须要有！！ 否则 开启中断立马中断给你看
            TIM_Cmd(TIM2, ENABLE);

            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE); /*先关闭等待使用*/
            Interrupt.SetPriority(TIM2_IRQn, 3); 
            break;
        case Timer3:
            /* 设置TIM2CLK 为 72MHZ */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
            TIM_DeInit(TIM3);

            TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

            TIM_ClearFlag(TIM3, TIM_FLAG_Update);

            TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
            TIM_ClearFlag(TIM3, TIM_FLAG_Update); // 清除标志位  必须要有！！ 否则 开启中断立马中断给你看
            TIM_Cmd(TIM3, ENABLE);

            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE); /*先关闭等待使用*/
            Interrupt.SetPriority(TIM3_IRQn, 3);
            break;
        case Timer4:
                /* 设置TIM2CLK 为 72MHZ */
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
                TIM_DeInit(TIM4);

                TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

                TIM_ClearFlag(TIM4, TIM_FLAG_Update);

                TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
                TIM_ClearFlag(TIM4, TIM_FLAG_Update); // 清除标志位  必须要有！！ 否则 开启中断立马中断给你看
                TIM_Cmd(TIM4, ENABLE);

                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE); /*先关闭等待使用*/
                Interrupt.SetPriority(TIM4_IRQn, 3);
            break;
        case Timer5:
                /* 设置TIM2CLK 为 72MHZ */
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
                TIM_DeInit(TIM5);

                TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

                TIM_ClearFlag(TIM5, TIM_FLAG_Update);

                TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
                TIM_ClearFlag(TIM5, TIM_FLAG_Update); // 清除标志位  必须要有！！ 否则 开启中断立马中断给你看
                TIM_Cmd(TIM5, ENABLE);

                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, DISABLE); /*先关闭等待使用*/
				#ifndef STM32F10X_MD
                Interrupt.SetPriority(TIM5_IRQn, 3);
				#endif
            break;
        case Timer6:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
            TIM_DeInit(TIM6);
			TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
			TIM_ClearFlag(TIM6, TIM_FLAG_Update);
			TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
			TIM_ClearFlag(TIM6, TIM_FLAG_Update); // 清除标志位  必须要有！！ 否则 开启中断立马中断给你看
            TIM_Cmd(TIM6, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, DISABLE); /*先关闭等待使用*/
            #ifndef STM32F10X_MD
			    Interrupt.SetPriority(TIM6_IRQn, 3);
			#endif
            break;
        case Timer7:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
            TIM_DeInit(TIM7);
            TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
            TIM_ClearFlag(TIM7, TIM_FLAG_Update);
            TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
            TIM_ClearFlag(TIM7, TIM_FLAG_Update); // 清除标志位  必须要有！！ 否则 开启中断立马中断给你看
            TIM_Cmd(TIM7, ENABLE);
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, DISABLE); /*先关闭等待使用*/
			#ifndef STM32F10X_MD
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
		this->Prescaler=36000-1;//则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=系统时钟/(分频系数+1) =1M
		this->Period=2000-1;
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
    this->Opened = true;
	#if DEBUG
        // 获取当前频率
        RCC_ClocksTypeDef clock;
        RCC_GetClocksFreq(&clock);

            uint clk = clock.PCLK1_Frequency;
            if ((uint)this->_index &0x00010000)
                clk = clock.PCLK2_Frequency;
            clk <<= 1;
        
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
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, st);
            break;
        case Timer5:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, st);
            break;
        case Timer6:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, st);
            break;
        case Timer7:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, st);
            break;
        case Timer8:
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, st);
            break;
        case Timer9:
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, st);
            break;
        case Timer10:
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, st);
            break;
        case Timer11:
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, st);
            break;
        case Timer12:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, st);
            break;
        case Timer13:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, st);
            break;
        case Timer14:
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, st);
            break;
        case Timer15:
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, st);
            break;
        case Timer16:
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, st);
            break;
        case Timer17:
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, st);
            break;
        case Timer18:
			    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM18, st);
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
//////////////////////////////////////////////////////////////
/////////////////////////////////以下为添加////////////////////
/* 定时器针脚 ------------------------------------------------------------------*/
    #define TIMS {TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8}
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

static TIM_TypeDef *const g_Timers[] = TIMS;
const byte Timer::TimerCount = ArrayLength(g_Timers);
extern Timer **Timers; // 已经实例化的定时器对象
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
                Interrupt.Activate(TIM4_IRQn, Timer::OnHandler, this);
            break;
        case Timer5:
			#ifndef STM32F10X_MD
                Interrupt.Activate(TIM5_IRQn, Timer::OnHandler, this);
			#endif
            break;
        case Timer6:
			#ifndef STM32F10X_MD
                Interrupt.Activate(TIM6_IRQn, Timer::OnHandler, this);
			#endif
			break;
        case Timer7:
			#ifndef STM32F10X_MD
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
