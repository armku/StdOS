#include "TTime.h"
#include "Device\RTC.h"
#ifdef STM32F1
    #include "stm32f10x.h"
#endif 
#ifdef STM32F4
    #include "stm32f4xx.h"
#endif 

extern byte fac_us; //每个us需要的systick时钟数 
TTime Time; //系统时间，不建议用户直接使用
extern UInt64 TicksPerms;

void TTime::OnHandler(ushort num, void *param){

}

TTime::TTime()
{
	this->BaseSeconds=0;
	this->Seconds=0;
}
void RtcRefresh(void *param)
{
	HardRTC *rtc = (HardRTC*)param;
    rtc->LoadTime();
}
// 使用RTC，必须在Init前调用
void TTime::UseRTC()
{
	HardRTC *rtc=HardRTC::Instance();
	rtc->LowPower = false;
    rtc->External = false;

	rtc->Init();
	Sys.AddTask(RtcRefresh, rtc, 100, 100, "Rtc");
}
void TTime::Init()
{

}
// 当前滴答时钟
uint TTime::CurrentTicks()const
{
    return 0;
}

// 当前毫秒数
UInt64 TTime::Current()const
{
    #if 1
        UInt64 ret = this->Milliseconds *1000;//先转换为us
        ret += (TicksPerms - SysTick->VAL) / fac_us;
        return ret;
    #else 
        return this->mCurrent *1000;
    #endif 
}

// 设置时间
void TTime::SetTime(UInt64 seconds){

}

void TTime::Sleep(int ms, bool *running)const
{
    this->Delay(ms *1000);
}

// 微秒级延迟
void TTime::Delay(int us)const
{
    uint ticks;
    uint told, tnow, tcnt = 0;
    uint reload = SysTick->LOAD; //LOAD的值
    ticks = us * fac_us; //需要的节拍数
    tcnt = 0;
    told = SysTick->VAL; //刚进入时的计数器值
    while (1)
    {
        tnow = SysTick->VAL;
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
    };
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

    //systick中断服务函数,使用ucos时用到
    void SysTick_Handler(void)
    {
        Time.Milliseconds++;
    }

    //以下为汇编函数
    void WFI_SET(void); //执行WFI指令    
    void MSR_MSP(uint addr); //设置堆栈地址
    //THUMB指令不支持汇编内联
    //采用如下方法实现执行汇编指令WFI  
    void WFI_SET(void)
    {
        __ASM volatile("wfi");
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
