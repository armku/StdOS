/*
此文件需要优化掉
 */
#include "TTime.h"
#include "Device\Timer.h"
//#include "_Core.h"
#include "Platform\stm32.h"

extern Timer *timerTick;
extern Delegate < Timer & > abc;
extern int gTicks; //
void timTickrefesh(void *param);
void TTime::Init()
{
    // 初始化为输出
    this->Index = 5;

    gTicks = (Sys.Clock >> 3) / 0xF4240u;

    timerTick = new Timer((TIMER)this->Index);
    abc.Bind(timTickrefesh);
    timerTick->Register(abc);
    timerTick->Open();
    //timerTick->SetFrequency(1000);
    //        timer2->Config();
   
    SysTick_Config(9000); //配置SysTick tick is 9ms	9000
	SysTick->CTRL  = SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //关闭jtag，保留swd	
    NVIC_SetPriority(SysTick_IRQn, 0);
    switch (this->Index)
    {
        case 2:
            NVIC_SetPriority(TIM3_IRQn, 0);
            break;
        case 5:
			#ifndef STM32F10X_MD
            NVIC_SetPriority(TIM6_IRQn, 0);
			#endif
            break;
        case 6:
			#ifndef STM32F10X_MD
            NVIC_SetPriority(TIM7_IRQn, 0);
			#endif
            break;
        default:
            break;
    }
}

// 当前滴答时钟
uint32_t TTime::CurrentTicks()const
{
    return SysTick->LOAD - SysTick->VAL;
}

// 当前毫秒数 计数值2000
uint64_t TTime::Current()const
{
    __IO uint16_t ms = 0;
    switch (this->Index)
    {
        case 2:
            ms = (TIM3->CNT) >> 1;
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
	F103 72MHz  750us->753us
	*/
	for(int i=0;i<nus;i++)
	{
		for(int j=0;j<10;j++);		
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
    __asm void MSR_MSP(uint32_t addr)
    {
        MSR MSP, r0  //set Main Stack value
        BX r14
    }
    #ifdef __cplusplus
    }
#endif
