#include "SerialPort.h"
#include "TInterrupt.h"
#include "Timer.h"

//#ifdef STM32F0
//    #include "stm32f0xx.h"
//#elif defined STM32F1
//    #include "stm32f10x.h"
//#elif defined STM32F4
//    #include "stm32f4xx.h"
//#endif 
extern void *onIsr[]; //中断


unsigned int GetIPSR(void)
{
//  return __get_CPSR();
	return 0;
}
// 注册中断函数（中断号，函数，参数）
bool TInterrupt::Activate(short irq, InterruptCallback isr, void *param)
{
    onIsr[irq] = param;
    return true;
}
// 全局中断开关状态
bool TInterrupt::GlobalState()
{
    return false;
}

//// 打开全局中断
//void TInterrupt::GlobalEnable()
//{
//    __ASM volatile("cpsie i");
//}
//bool TInterrupt::OnDeactivate(short irq)
//{
//    return false;
//}
//// 关闭全局中断
//void TInterrupt::GlobalDisable()
//{
//    __ASM volatile("cpsid i");
//}

// 是否在中断里面
bool TInterrupt::IsHandler()
{
    return false;
}
// 获取优先级
void TInterrupt::GetPriority(short irq)const{

}

//void TInterrupt::SetPriority(short irq, uint priority)const
//{
//    NVIC_InitTypeDef nvic;
//	
//    nvic.NVIC_IRQChannelCmd = ENABLE;
//    nvic.NVIC_IRQChannel = irq;

//    #if defined(STM32F1) || defined(STM32F4)
//        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//        nvic.NVIC_IRQChannelPreemptionPriority = 1;
//        nvic.NVIC_IRQChannelSubPriority = priority;
//    #elif defined STM32F0
//		nvic.NVIC_IRQChannelPriority = priority;
//    #endif 

//    NVIC_Init(&nvic);
//	NVIC_SetPriority((IRQn_Type)irq, priority);
//}
