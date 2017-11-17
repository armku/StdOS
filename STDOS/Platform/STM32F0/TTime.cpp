/*
此文件需要优化掉
*/
#include "TTime.h"
#include "Device\RTC.h"
#include "Device\Timer.h"
#include "stm32f0xx.h"

extern Timer *timerTick;
extern Delegate < Timer & > abc;
extern int gTicks; //
extern byte fac_us; //us延时倍乘数 每个us需要的systick时钟数 	
void timTickrefesh(void *param);
void TTime::Init()
{
    // 初始化为输出
        this->Index = 6;
	
	gTicks = (Sys.Clock >> 3) / 0xF4240u;
	
    timerTick = new Timer((TIMER)this->Index);
    abc.Bind(timTickrefesh);
    timerTick->Register(abc);
    timerTick->Open();
    //timerTick->SetFrequency(1000);
    //        timer2->Config();

    //初始化延迟函数
    //SYSTICK的时钟固定为HCLK时钟的1/8
    //SYSCLK:系统时钟
        fac_us = SystemCoreClock / 8000000 * 8; //为系统时钟的1/8 //非OS下,代表每个us需要的systick时钟数 
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //选择外部时钟  HCLK/8
    SysTick_Config(9000); //配置SysTick tick is 9ms	9000
    NVIC_SetPriority(SysTick_IRQn, 0);
	switch(this->Index)
	{
		case 2:
			NVIC_SetPriority(TIM3_IRQn, 0);
			break;
		case 5:
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
	SmartIRQ irq;

    uint value = (SysTick->LOAD - SysTick->VAL);	
    return systickcnt + value;
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
// 微秒级延迟
void TTime::Delay(int nus)const
{
    uint ticks;
    uint told, tnow, tcnt = 0;
    uint reload = 0;
        reload = SysTick->LOAD; //LOAD的值
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
    }
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
