#include "TTime.h"
#include "Device\RTC.h"
#include "Device\Timer.h"

#ifdef STM32F0
    #include "stm32f0xx.h"
#elif defined STM32F1
    #include "stm32f10x.h"
#elif defined STM32F4
    #include "stm32f4xx.h"
#endif 

TTime Time; //系统时间，不建议用户直接使用

void TTime::OnHandler(ushort num, void *param){

}

TTime::TTime()
{
    this->BaseSeconds = 0;
    this->Seconds = 0;
}

void RtcRefresh(void *param)
{
    HardRTC *rtc = (HardRTC*)param;
    rtc->LoadTime();
}

// 使用RTC，必须在Init前调用
void TTime::UseRTC()
{
    HardRTC *rtc = HardRTC::Instance();
    rtc->LowPower = false;
    rtc->External = false;

    rtc->Init();
    Sys.AddTask(RtcRefresh, rtc, 100, 100, "Rtc");
}

extern uint time6cnt;
Timer *timerTick;
Delegate < Timer & > abc;
void timTickrefesh(void *param)
{
    Time.Milliseconds += 1000;
    Time.Seconds++;
    time6cnt++;
}

static byte fac_us = 0; //us延时倍乘数 每个us需要的systick时钟数 			   
static ushort fac_ms = 0; //ms延时倍乘数,在ucos下,代表每个节拍的ms数
void TTime::Init()
{

    // 初始化为输出
    #if defined STM32F0
        this->Index = 6;
    #elif defined STM32F1
        this->Index = 5;
    #elif defined STM32F4
        this->Index = 2;
    #endif 
    timerTick = new Timer((TIMER)this->Index);
    abc.Bind(timTickrefesh);
    timerTick->Register(abc);
    timerTick->Open();
    //timerTick->SetFrequency(1000);
    //        timer2->Config();

    //初始化延迟函数
    //SYSTICK的时钟固定为HCLK时钟的1/8
    //SYSCLK:系统时钟
    #if defined STM32F0
    #elif defined STM32F1
        SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //选择外部时钟  HCLK/8
        fac_us = SystemCoreClock / 8000000; //为系统时钟的1/8  
        fac_ms = (ushort)fac_us *1000; //代表每个ms需要的systick时钟数   
    #elif defined STM32F4
        uint SYSCLK = 168;
        SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
        fac_us = SYSCLK / 8; //不论是否使用OS,fac_us都需要使用

        fac_ms = (u16)fac_us *1000; //非OS下,代表每个ms需要的systick时钟数   
    #endif 

    #ifdef STM32F0
        fac_us = SystemCoreClock / 8000000 * 8; //为系统时钟的1/8 //非OS下,代表每个us需要的systick时钟数 
    #elif defined STM32F1
        fac_us = SystemCoreClock / 8000000 * 8; //为系统时钟的1/8 //非OS下,代表每个us需要的systick时钟数 
    #elif defined STM32F4
        fac_us = SystemCoreClock / 8000000 * 8; //为系统时钟的1/8 //非OS下,代表每个us需要的systick时钟数 
    #endif 
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //选择外部时钟  HCLK/8
    SysTick_Config(9000); //配置SysTick tick is 9ms	9000
    #ifdef STM32F0
    #elif defined STM32F1
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //关闭jtag，保留swd	
    #elif defined STM32F4
    #endif 
    NVIC_SetPriority(SysTick_IRQn, 0);
	switch(this->Index)
	{
		case 2:
			NVIC_SetPriority(TIM3_IRQn, 0);
			break;
		case 5:
			#if defined STM32F0
			#else
				NVIC_SetPriority(TIM6_IRQn, 0);
			#endif
			break;
		case 6:
			NVIC_SetPriority(TIM7_IRQn, 0);
			break;
		default:
			break;
	}

}

extern uint systickcnt;
// 当前滴答时钟
uint TTime::CurrentTicks()const
{
    return systickcnt;
}

// 当前毫秒数
UInt64 TTime::Current()const
{
    __IO ushort ms = 0;
    switch (this->Index)
    {
		case 2:
			ms=(TIM3->CNT) >> 1;
			break;
        case 5:
            ms = (TIM6->CNT) >> 1;
            break;
        case 6:
            ms = (TIM7->CNT) >> 1;
            break;
        default:
            break;
    }
    return this->Milliseconds + ms;
}

// 设置时间
void TTime::SetTime(UInt64 seconds){

}
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void TTime::Sleep(int nms, bool *running)const
{
    this->Delay(nms *1000);
}

// 微秒级延迟
void TTime::Delay(int nus)const
{
    uint ticks;
    uint told, tnow, tcnt = 0;
    uint reload = 0;
    #if defined(STM32F1) || defined(STM32F4)
        reload = SysTick->LOAD; //LOAD的值
    #elif defined STM32F0
        reload = SysTick->LOAD; //LOAD的值
    #endif 
    ticks = nus * fac_us; //需要的节拍数
    tcnt = 0;
    #if defined(STM32F1) || defined(STM32F4)
        told = SysTick->VAL; //刚进入时的计数器值
    #elif defined STM32F0
        told = SysTick->VAL; //刚进入时的计数器值
    #endif 
    while (1)
    {
        #if defined(STM32F1) || defined(STM32F4)
            tnow = SysTick->VAL;
        #elif defined STM32F0
            tnow = SysTick->VAL;
        #endif 
        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;
            //这里注意一下SYSTICK是一个递减的计数器就可以了.
            else
                tcnt += reload - tnow + told;
            told = tnow;
            if (tcnt >= ticks)
                break;
            //时间超过/等于要延迟的时间,则退出.
        }
    }
}

uint TTime::TicksToUs(uint ticks)const
{
    if (!ticks)
    {
        return 0;
    }
    //1000000 = 64 * 15625 = 2^6 * 15625
    //(0xFFFFFFFF / (1000000 >> 6)) = 274877.90688
    //如果TICKS<274877,那么乘法不会溢出
    if (ticks <= (0xffffffff / (1000000 >> 6)))
    {
        return (ticks *(1000000 >> 6)) >> (15-6);
    }
    else
    {
        return ((UInt64)ticks *(1000000 >> 6)) >> (16-6);
    }
}

uint TTime::UsToTicks(uint us)const
{
    if (!us)
    {
        return 0;
    }

    //1000000 = 64 * 15625 = 2^6 * 15625
    //((0xffffffff +1) >> (15-6)) = 8388608 = 0x800000 = 1 << (32 - 9)
    //如果ticks<274877,那么乘法不会溢出
    if (us < (1 << (32-(15-6))))
    {
        return (us << (15-6)) / (1000000 >> 6);
    }
    else
    {
        return ((UInt64)us << (15-6)) / (1000000 >> 6);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
TimeWheel::TimeWheel(uint ms){

}

void TimeWheel::Reset(uint ms){

}
// 是否已过期
bool TimeWheel::Expired()
{
    return true;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
TimeCost::TimeCost()
{
    this->Start = 0;
}

void TimeCost::Reset(){}
// 逝去的时间，微秒
int TimeCost::Elapsed()const
{
    uint ret = 12;
    ret -= this->Start;
    return ret;
}

void TimeCost::Show(cstring format)const
{
    printf("执行 %d 微妙\r\n", 12);
}




#ifdef __cplusplus
    extern "C"
    {
    #endif 

    //以下为汇编函数
    //THUMB指令不支持汇编内联
    //采用如下方法实现执行汇编指令WFI  
    void WFI_SET(void)
    {
        #if defined(STM32F1) || defined(STM32F4)
            __ASM volatile("wfi");
        #elif defined STM32F0
            __ASM volatile("wfi");
        #endif 
    }

    //设置栈顶地址
    //addr:栈顶地址
    __asm void MSR_MSP(uint addr)
    {
        MSR MSP, r0  //set Main Stack value
        BX r14
    }
    #ifdef __cplusplus
    }
#endif
