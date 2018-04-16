/*
此文件需要优化掉
 */
#include "TTime.h"
//#include "_Core.h"
#include "Platform\stm32.h"
#include "BspPlatform\BspPlatform.h"

extern int gTicks; //
void TTime::Init()
{
    // 初始化为输出
    this->Index = 5;

    gTicks = (Sys.Clock >> 3) / 0xF4240u;
    
    SysTick_Config(9000); //配置SysTick tick is 9ms	9000
	SysTick->CTRL  = SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //关闭jtag，保留swd	
}

// 当前滴答时钟
uint32_t TTime::CurrentTicks()const
{
    return CurrentTicks1();
}

// 当前毫秒数 计数值2000
uint64_t TTime::Current()const
{    
    return this->Milliseconds + CurrentTick();
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
