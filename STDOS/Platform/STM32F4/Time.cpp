/*
此文件需要优化掉
*/
#include "TTime.h"
#include "Device\RTC.h"
#include "Device\Timer.h"
#include "Platform\stm32.h"

extern Timer *timerTick;
extern Delegate < Timer & > abc;
extern int gTicks; //
void timTickrefesh(void *param);
void TTime::Init()
{
    // 初始化为输出
        this->Index = 2;
	
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
//        uint SYSCLK = 168;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);  
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
// 当前滴答时钟
uint TTime::CurrentTicks()const
{
	return SysTick->LOAD - SysTick->VAL;
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

//us延时，100us以下精确
void TTime::DelayUs(int nus) const
{
	/*
	F407 168MHz 750us->759us
	*/
	for(int i=0;i<nus;i++)
	{
		for(int j=0;j<40;j++);
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
