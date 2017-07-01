/*
Timer6 Timer7 为基本定时器，只具备基础的定时功能
Timer2 Timer3 Timer4 Timer5 为通用定时器，可以脉宽测量PWM 具有编码器接口
Timer1 Timer8 高级定时器，具有基本定时器 通用定时器所有功能，三相六步电机，刹车功能，PWM死区控制
*/
#include "Timer.h"
#include "stm32f10x_rcc.h"
#include "TInterrupt.h"

static Timer **Timers = NULL; // 已经实例化的定时器对象
#define TIMS {TIM1,TIM2,TIM3,TIM4,TIM5}
static TIM_TypeDef *const g_Timers[] = TIMS;
const byte Timer::TimerCount = ArrayLength(g_Timers);

void Timer::SetHandler(bool set){

}

Timer::Timer(TIMER index)
{
    this->_index = index;
    this->Period = 1000; //默认1秒一次
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
        debug_printf("Timer%d::Start Prescaler=%d Period=%d Frequency=%d\r\n", _index + 1, Prescaler+1, Period, fre);
    #endif 
    Opened = true;
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

    Opened = false;
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
    switch (this->_index)
    {
        case Timer1:
            break;
        case Timer2:     

            /* 设置TIM2CLK 为 72MHZ */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
            //TIM_DeInit(TIM2);

            /* 自动重装载寄存器周期的值(计数值) */
            TIM_TimeBaseStructure.TIM_Period = this->Period;

            /* 累计 TIM_Period个频率后产生一个更新或者中断 */
            /* 时钟预分频数为72 */
            TIM_TimeBaseStructure.TIM_Prescaler = this->Prescaler;

            /* 对外部时钟进行采样的时钟分频,这里没有用到 */
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
            TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

            TIM_ClearFlag(TIM2, TIM_FLAG_Update);

            TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
            TIM_ClearFlag(TIM2, TIM_FLAG_Update); // 清除标志位  必须要有！！ 否则 开启中断立马中断给你看
            TIM_Cmd(TIM2, ENABLE);

            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE); /*先关闭等待使用*/
            Interrupt.SetPriority(28, 3);//TIM2_IRQn
            break;
        case Timer3:
			 /* 设置TIM2CLK 为 72MHZ */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
            //TIM_DeInit(TIM3);

            /* 自动重装载寄存器周期的值(计数值) */
            TIM_TimeBaseStructure.TIM_Period = this->Period;

            /* 累计 TIM_Period个频率后产生一个更新或者中断 */
            /* 时钟预分频数为72 */
            TIM_TimeBaseStructure.TIM_Prescaler = this->Prescaler;

            /* 对外部时钟进行采样的时钟分频,这里没有用到 */
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
            TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

            TIM_ClearFlag(TIM3, TIM_FLAG_Update);

            TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
            TIM_ClearFlag(TIM3, TIM_FLAG_Update); // 清除标志位  必须要有！！ 否则 开启中断立马中断给你看
            TIM_Cmd(TIM3, ENABLE);

            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE); /*先关闭等待使用*/
            Interrupt.SetPriority(29, 3);//TIM3_IRQn
            break;
        case Timer4:
             /* 设置TIM2CLK 为 72MHZ */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
            //TIM_DeInit(TIM4);

            /* 自动重装载寄存器周期的值(计数值) */
            TIM_TimeBaseStructure.TIM_Period = this->Period;

            /* 累计 TIM_Period个频率后产生一个更新或者中断 */
            /* 时钟预分频数为72 */
            TIM_TimeBaseStructure.TIM_Prescaler = this->Prescaler;

            /* 对外部时钟进行采样的时钟分频,这里没有用到 */
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
            TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

            TIM_ClearFlag(TIM4, TIM_FLAG_Update);

            TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
            TIM_ClearFlag(TIM4, TIM_FLAG_Update); // 清除标志位  必须要有！！ 否则 开启中断立马中断给你看
            TIM_Cmd(TIM4, ENABLE);

            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE); /*先关闭等待使用*/
            Interrupt.SetPriority(30, 3);//TIM4_IRQn
            break;
        case Timer5:
             /* 设置TIM2CLK 为 72MHZ */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
            //TIM_DeInit(TIM5);

            /* 自动重装载寄存器周期的值(计数值) */
            TIM_TimeBaseStructure.TIM_Period = this->Period;

            /* 累计 TIM_Period个频率后产生一个更新或者中断 */
            /* 时钟预分频数为72 */
            TIM_TimeBaseStructure.TIM_Prescaler = this->Prescaler;

            /* 对外部时钟进行采样的时钟分频,这里没有用到 */
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
            TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

            TIM_ClearFlag(TIM5, TIM_FLAG_Update);

            TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
            TIM_ClearFlag(TIM5, TIM_FLAG_Update); // 清除标志位  必须要有！！ 否则 开启中断立马中断给你看
            TIM_Cmd(TIM5, ENABLE);

            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, DISABLE); /*先关闭等待使用*/
            Interrupt.SetPriority(50, 3);//TIM5_IRQn
            break;
        case Timer6:
             /* 设置TIM2CLK 为 72MHZ */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
            //TIM_DeInit(TIM6);

            /* 自动重装载寄存器周期的值(计数值) */
            TIM_TimeBaseStructure.TIM_Period = this->Period;

            /* 累计 TIM_Period个频率后产生一个更新或者中断 */
            /* 时钟预分频数为72 */
            TIM_TimeBaseStructure.TIM_Prescaler = this->Prescaler;

            /* 对外部时钟进行采样的时钟分频,这里没有用到 */
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
            TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

            TIM_ClearFlag(TIM6, TIM_FLAG_Update);

            TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
            TIM_ClearFlag(TIM6, TIM_FLAG_Update); // 清除标志位  必须要有！！ 否则 开启中断立马中断给你看
            TIM_Cmd(TIM6, ENABLE);

            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, DISABLE); /*先关闭等待使用*/
            Interrupt.SetPriority(54, 3);//TIM6_IRQn
            break;
        case Timer7:
             /* 设置TIM2CLK 为 72MHZ */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
            //TIM_DeInit(TIM7);

            /* 自动重装载寄存器周期的值(计数值) */
            TIM_TimeBaseStructure.TIM_Period = this->Period;

            /* 累计 TIM_Period个频率后产生一个更新或者中断 */
            /* 时钟预分频数为72 */
            TIM_TimeBaseStructure.TIM_Prescaler = this->Prescaler;

            /* 对外部时钟进行采样的时钟分频,这里没有用到 */
            TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

            TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
            TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

            TIM_ClearFlag(TIM7, TIM_FLAG_Update);

            TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
            TIM_ClearFlag(TIM7, TIM_FLAG_Update); // 清除标志位  必须要有！！ 否则 开启中断立马中断给你看
            TIM_Cmd(TIM7, ENABLE);

            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, DISABLE); /*先关闭等待使用*/
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

// 设置频率，自动计算预分频
void Timer::SetFrequency(uint frequency)
{
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

    Prescaler = pre-1;
    Period = p;

    this->Open();
}

uint Timer::GetCounter()
{
    return this->Period;
}

void Timer::SetCounter(uint cnt) // 设置计数器值
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
//定时器测试
extern uint time2cnt;
Delegate<Timer&> abc;
void tim2refesh(void * param)
{
	time2cnt++;
}
Timer *timer2;
void TimeTest()
{
	// 初始化为输出
	timer2=new Timer(Timer7);
	abc.Bind(tim2refesh);
	timer2->Register(abc);	
    timer2->Open();
//	timer2->SetFrequency(1);
//	timer2->Config();
}
