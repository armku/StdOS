﻿#include "Kernel\Sys.h"

#include "Kernel\Interrupt.h"
//#include "SerialPort.h"

#include "..\stm32.h"

#define IS_IRQ(irq) (irq >= -16 && irq <= VectorySize - 16)

//#include "..\CMSIS\Interrupt.cpp"

void TInterrupt::OnInit() const
{
    // 禁用所有中断
    NVIC->ICER[0] = 0xFFFFFFFF;
#if defined(STM32F1) || defined(STM32F4)
    NVIC->ICER[1] = 0xFFFFFFFF;
    NVIC->ICER[2] = 0xFFFFFFFF;
#endif // defined(STM32F1) || defined(STM32F4)

    // 清除所有中断位
    NVIC->ICPR[0] = 0xFFFFFFFF;
#if defined(STM32F1) || defined(STM32F4)
    NVIC->ICPR[1] = 0xFFFFFFFF;
    NVIC->ICPR[2] = 0xFFFFFFFF;
#endif	// defined(STM32F1) || defined(STM32F4)

#if defined(BOOT) || defined(APP)
	StrBoot.pUserHandler = RealHandler;;
#endif

#if defined(APP)
	GlobalEnable();
#endif

#ifdef STM32F4
    /*SCB->AIRCR = (0x5FA << SCB_AIRCR_VECTKEY_Pos) // 解锁
               | (7 << SCB_AIRCR_PRIGROUP_Pos);   // 没有优先组位*/
	// 中断优先级分配方案4，四位都是抢占优先级。其实上面的寄存器操作就是设置优先级方案为0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	// 打开系统异常，否则这些异常都会跑到硬件中断里面
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk
                | SCB_SHCSR_BUSFAULTENA_Msk
                | SCB_SHCSR_MEMFAULTENA_Msk;
#endif	// STM32F4
#ifdef STM32F1
    /*SCB->AIRCR = (0x5FA << SCB_AIRCR_VECTKEY_Pos) // 解锁
               | (7 << SCB_AIRCR_PRIGROUP_Pos);   // 没有优先组位*/
	// 中断优先级分配方案4，四位都是抢占优先级。其实上面的寄存器操作就是设置优先级方案为0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	// 打开系统异常，否则这些异常都会跑到硬件中断里面
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA
                | SCB_SHCSR_BUSFAULTENA
                | SCB_SHCSR_MEMFAULTENA;
#endif	// STM32F1

	// 初始化EXTI中断线为默认值
	EXTI_DeInit();
}

#ifdef STM32F1
uint TInterrupt::EncodePriority (uint priorityGroup, uint preemptPriority, uint subPriority) const
{
    return NVIC_EncodePriority(priorityGroup, preemptPriority, subPriority);
}

void TInterrupt::DecodePriority (uint priority, uint priorityGroup, uint* pPreemptPriority, uint* pSubPriority) const
{
    NVIC_DecodePriority(priority, priorityGroup, (uint32_t*)pPreemptPriority, (uint32_t*)pSubPriority);
}
#endif

#if !defined(TINY)

extern "C"
{
	void FAULT_SubHandler(uint* registers, uint exception)
	{
		debug_printf("LR=%p PC=%p PSR=%p SP=%p\r\n", registers[13], registers[14], registers[15], registers[16]);
		for(int i=0; i<=12; i++)
		{
			debug_printf("R%d=%p\r\n", i, registers[i]);
		}

		TInterrupt::Halt();
	}

#if defined ( __CC_ARM   )
	__asm void FaultHandler()
	{
		IMPORT FAULT_SubHandler

		//on entry, we have an exception frame on the stack:
		//SP+00: R0
		//SP+04: R1
		//SP+08: R2
		//SP+12: R3
		//SP+16: R12
		//SP+20: LR
		//SP+24: PC
		//SP+28: PSR
		//R0-R12 are not overwritten yet
#if defined(STM32F0) || defined(GD32F150)
		//add      sp,sp,#16             // remove R0-R3
		push     {r4-r7}              // store R0-R11
#else
		add      sp,sp,#16             // remove R0-R3
		push     {r0-r11}              // store R0-R11
#endif
		mov      r0,sp
		//R0+00: R0-R12
		//R0+52: LR
		//R0+56: PC
		//R0+60: PSR
		mrs      r1,IPSR               // exception number
		b        FAULT_SubHandler
		//never expect to return
	}
#elif (defined (__GNUC__))
	void FaultHandler()
	{
		__asm volatile (
#if defined(STM32F0) || defined(GD32F150)
			"push	{r4-r7}		\n\t"
#else
			"add	sp,sp,#16	\n\t"
			"push	{r0-r11}	\n\t"
#endif
			"mov	r0,sp	\n\t"
			"mrs	r1,IPSR	\n\t"
			"b	FAULT_SubHandler	\n\t");
	}
#endif
}
#endif
