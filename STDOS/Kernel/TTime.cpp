#include "TTime.h"
#include "stm32f10x.h"

extern byte fac_us; //每个us需要的systick时钟数 
#ifdef __cplusplus
    extern "C"
    {
    #endif 

    //systick中断服务函数,使用ucos时用到
    void SysTick_Handler(void)
    {
        Time.mCurrent++;
    }

    //延时nus
    //nus为要延时的us数.
    void delay_us(uint nus)
    {
        uint ticks;
        uint told, tnow, tcnt = 0;
        uint reload = SysTick->LOAD; //LOAD的值
        ticks = nus * fac_us; //需要的节拍数
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
    //以下为汇编函数
    void WFI_SET(void); //执行WFI指令
    void INTX_DISABLE(void); //关闭所有中断
    void INTX_ENABLE(void); //开启所有中断
    void MSR_MSP(uint addr); //设置堆栈地址
    //THUMB指令不支持汇编内联
    //采用如下方法实现执行汇编指令WFI  
    void WFI_SET(void)
    {
        __ASM volatile("wfi");
    }

    //关闭所有中断
    void INTX_DISABLE(void)
    {
        __ASM volatile("cpsid i");
    }

    //开启所有中断
    void INTX_ENABLE(void)
    {
        __ASM volatile("cpsie i");
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
TTime Time; //系统时间，不建议用户直接使用

TTime::TTime(){}
//返回当前毫秒数
UInt64 TTime::Ms()const
{
    return this->mCurrent;
}

//设置系统当前时间,Unix格式秒数
void TTime::SetTime(UInt64 seconds){

}
//系统时间
uint TTime::Seconds()
{
    return 0;
}

//延时ms
void TTime::Sleep(uint us)
{
    delay_us(us);
}
extern UInt64 TicksPerms;
//系统启动以来的毫秒
UInt64 TTime::Current()const
{
	UInt64 ret=this->mCurrent *1000;
	ret+=(TicksPerms-SysTick->VAL)/fac_us;
    return ret;
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

/*
1us=30ticks
 */
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
