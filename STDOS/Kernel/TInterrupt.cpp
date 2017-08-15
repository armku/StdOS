#include "SerialPort.h"
#include "TInterrupt.h"
#include "Timer.h"

#ifdef STM32F0
    #include "stm32f0xx.h"
#elif defined STM32F1
    #include "stm32f10x.h"
#elif defined STM32F4
    #include "stm32f4xx.h"
#endif 

#ifdef STM32F0
	#define NVIC_VectTab_RAM             ((uint)0x20000000)
	#define NVIC_VectTab_FLASH           ((uint)0x08000000)
#endif

#ifdef STM32F0
	#define NVIC_OFFSET					 ((uint)0x0000)
#else
	#define NVIC_OFFSET					 ((uint)0x1000)
#endif
#define ISRADDR (NVIC_VectTab_RAM+NVIC_OFFSET)
#if defined(STM32F1) || defined(STM32F4)
	#define ISRLENGTH   100	//中断数量
#elif defined STM32F0
	#define ISRLENGTH   60	//中断数量
#endif

TInterrupt Interrupt;

class CInterrupt
{
    public:
        static void Default_Handler();
        static void SysTick_Handler(); //systick中断服务函数

        static void USART1_IRQHandler();
        static void USART2_IRQHandler();
        static void USART3_IRQHandler();
        static void UART4_IRQHandler();
        static void UART5_IRQHandler();

        static void TIM2_IRQHandler();
        static void TIM3_IRQHandler();
        static void TIM4_IRQHandler();
        static void TIM5_IRQHandler();
        static void TIM6_IRQHandler();
        static void TIM7_IRQHandler();

        static void EXTI0_IRQHandler();
        static void EXTI1_IRQHandler();
        static void EXTI2_IRQHandler();
        static void EXTI3_IRQHandler();
        static void EXTI4_IRQHandler();
        static void EXTI9_5_IRQHandler();
        static void EXTI15_10_IRQHandler();

};
#include "TTime.h"
void *onIsr[ISRLENGTH]; //中断

extern "C"
{
	#ifdef STM32F0
		uint *VectorTable;
		//uint VectorTable[ISRLENGTH] ;//__attribute__((at(ISRADDR)));
    #else
		uint VectorTable[ISRLENGTH] __attribute__((at(ISRADDR)));
    #endif
	uint *vsrom = (uint*)NVIC_VectTab_FLASH;
		
	void TIM7_IRQHandler()
	{
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update); //先清空中断标志位，以备下次使用。
        if (onIsr[TIM7_IRQn])
        {
            ((Timer*)onIsr[TIM7_IRQn])->OnInterrupt();
        }
	}
}

// 初始化中断向量表
void TInterrupt::Init()const
{
	#ifdef STM32F0
		VectorTable=(uint *)ISRADDR;
	#endif
    //复制中断向量表
    for (int i = 0; i < ISRLENGTH; i++)
    {
        VectorTable[i] = vsrom[i];
    }
    //中断向量表重映射
	#if defined(STM32F1) || defined(STM32F4)
		NVIC_SetVectorTable(NVIC_VectTab_RAM, NVIC_OFFSET);
	#elif defined STM32F0
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
		SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
	#endif

    VectorTable[15] = (uint) &(CInterrupt::SysTick_Handler);
    VectorTable[53] = (uint) &(CInterrupt::USART1_IRQHandler);
    VectorTable[54] = (uint) &(CInterrupt::USART2_IRQHandler);
    VectorTable[55] = (uint) &(CInterrupt::USART3_IRQHandler);
    VectorTable[68] = (uint) &(CInterrupt::UART4_IRQHandler);
    VectorTable[69] = (uint) &(CInterrupt::UART5_IRQHandler);

    VectorTable[44] = (uint) &(CInterrupt::TIM2_IRQHandler);
    VectorTable[45] = (uint) &(CInterrupt::TIM3_IRQHandler);
    VectorTable[46] = (uint) &(CInterrupt::TIM4_IRQHandler);
    VectorTable[66] = (uint) &(CInterrupt::TIM5_IRQHandler);
    VectorTable[70] = (uint) &(CInterrupt::TIM6_IRQHandler);
    VectorTable[71] = (uint) &(CInterrupt::TIM7_IRQHandler);

    VectorTable[22] = (uint) &(CInterrupt::EXTI0_IRQHandler);
    VectorTable[23] = (uint) &(CInterrupt::EXTI1_IRQHandler);
    VectorTable[24] = (uint) &(CInterrupt::EXTI2_IRQHandler);
    VectorTable[25] = (uint) &(CInterrupt::EXTI3_IRQHandler);
    VectorTable[26] = (uint) &(CInterrupt::EXTI4_IRQHandler);
    VectorTable[39] = (uint) &(CInterrupt::EXTI9_5_IRQHandler);
    VectorTable[56] = (uint) &(CInterrupt::EXTI15_10_IRQHandler);

}

void TInterrupt::Process(uint num)const{

}

// 注册中断函数（中断号，函数，参数）
bool TInterrupt::Activate(short irq, InterruptCallback isr, void *param)
{
    onIsr[irq] = param;
    return true;
}

// 解除中断注册
bool TInterrupt::Deactivate(short irq)
{
    return false;
}

// 开中断
//bool TInterrupt::Enable(short irq) const{return false;}
// 关中断
//bool TInterrupt::Disable(short irq) const{return false;}

// 是否开中断
//bool TInterrupt::EnableState(short irq) const{return false;}
// 是否挂起
//bool TInterrupt::PendingState(short irq) const{return false;}
// 设置优先级
void TInterrupt::SetPriority(short irq, uint priority)const
{
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    nvic.NVIC_IRQChannel = irq;

    #if defined(STM32F1) || defined(STM32F4)
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
        nvic.NVIC_IRQChannelPreemptionPriority = 1;
        nvic.NVIC_IRQChannelSubPriority = priority;
    #elif defined STM32F0
        nvic.NVIC_IRQChannelPriority = 2;
        nvic.NVIC_IRQChannelCmd = ENABLE;
    #endif 

    NVIC_Init(&nvic);
}

// 获取优先级
void TInterrupt::GetPriority(short irq)const{

}
// 编码优先级
uint TInterrupt::EncodePriority(uint priorityGroup, uint preemptPriority, uint subPriority)const
{
    return 0;
}

// 解码优先级
void TInterrupt::DecodePriority(uint priority, uint priorityGroup, uint *pPreemptPriority, uint *pSubPriority)const{

}
// 全局中断开关状态
bool TInterrupt::GlobalState()
{
    return false;
}

// 打开全局中断
void TInterrupt::GlobalEnable()
{
    __ASM volatile("cpsie i");
}

// 关闭全局中断
void TInterrupt::GlobalDisable()
{
    __ASM volatile("cpsid i");
}


// 是否在中断里面
bool TInterrupt::IsHandler()
{
    return false;
}

// 系统挂起
void TInterrupt::Halt(){}
void TInterrupt::OnInit()const{}
bool TInterrupt::OnActivate(short irq)
{
    return false;
}

bool TInterrupt::OnDeactivate(short irq)
{
    return false;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
Lock::Lock(int &ref){}
Lock::~Lock(){}

bool Lock::Wait(int ms)
{
    return false;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//所有中断线处理
void EXTI_IRQHandler(ushort num, void *param);
extern SerialPort *_printf_sp;
void OnUsartReceive(ushort num, void *param);
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void CInterrupt::Default_Handler()
{
    while (1)
        ;
}



void CInterrupt::SysTick_Handler()
{
    Time.Milliseconds++;
}

//注意,读取USARTx->SR能避免莫名其妙的错误
void CInterrupt::USART1_IRQHandler()
{
    if (onIsr[USART1_IRQn])
    {
        OnUsartReceive(0, onIsr[USART1_IRQn]);
    }
}

void CInterrupt::USART2_IRQHandler()
{
    if (onIsr[USART2_IRQn])
    {
        OnUsartReceive(1, onIsr[USART2_IRQn]);
    }
}

void CInterrupt::USART3_IRQHandler()
{
	#if defined(STM32F1) || defined(STM32F4)
    if (onIsr[USART3_IRQn])
    {
        OnUsartReceive(2, onIsr[USART3_IRQn]);
    }
	#endif
}

void CInterrupt::UART4_IRQHandler()
{
	#if defined(STM32F1) || defined(STM32F4)
    if (onIsr[UART4_IRQn])
    {
        OnUsartReceive(3, onIsr[UART4_IRQn]);
    }
	#endif
}

void CInterrupt::UART5_IRQHandler()
{
	#if defined(STM32F1) || defined(STM32F4)
    if (onIsr[UART5_IRQn])
    {
        OnUsartReceive(4, onIsr[UART5_IRQn]);
    }
	#endif
}

void CInterrupt::TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        if (onIsr[TIM2_IRQn])
        {
            ((Timer*)onIsr[TIM2_IRQn])->OnInterrupt();
        }
        TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
    }
}

void CInterrupt::TIM3_IRQHandler()
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        if (onIsr[TIM3_IRQn])
        {
            ((Timer*)onIsr[TIM3_IRQn])->OnInterrupt();
        }
        TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
    }
}

void CInterrupt::TIM4_IRQHandler()
{
	#if defined(STM32F1) || defined(STM32F4)  
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        if (onIsr[TIM4_IRQn])
        {
            ((Timer*)onIsr[TIM4_IRQn])->OnInterrupt();
        }
        TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
    }
	#endif
}

void CInterrupt::TIM5_IRQHandler()
{
	#if defined(STM32F1) || defined(STM32F4)  
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        if (onIsr[TIM5_IRQn])
        {
            ((Timer*)onIsr[TIM5_IRQn])->OnInterrupt();
        }
        TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
    }
	#endif
}

void CInterrupt::TIM6_IRQHandler()
{
    #if defined(STM32F1) || defined(STM32F4)  
        if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
        {
            if (onIsr[TIM6_IRQn])
            {
                ((Timer*)onIsr[TIM6_IRQn])->OnInterrupt();
            }
            TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
        }
    #elif defined STM32F0
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update); //先清空中断标志位，以备下次使用。
        if (onIsr[TIM6_DAC_IRQn])
        {
            ((Timer*)onIsr[TIM6_DAC_IRQn])->OnInterrupt();
        }
            
    #endif 
}

void CInterrupt::TIM7_IRQHandler()
{
    #if defined(STM32F1) || defined(STM32F4) 
        if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
        {
            if (onIsr[TIM7_IRQn])
            {
                ((Timer*)onIsr[TIM7_IRQn])->OnInterrupt();
            }
            TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);
        }
    #elif defined STM32F0
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update); //先清空中断标志位，以备下次使用。
        if (onIsr[TIM7_IRQn])
        {
            ((Timer*)onIsr[TIM7_IRQn])->OnInterrupt();
        }
    #endif 
}

void CInterrupt::EXTI0_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        #if defined(STM32F1) || defined(STM32F4)			
            EXTI_IRQHandler(EXTI0_IRQn, 0);
        #elif defined STM32F0
            EXTI_IRQHandler(EXTI0_1_IRQn, 0);
        #endif 
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void CInterrupt::EXTI1_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        #if defined(STM32F1) || defined(STM32F4)			
            EXTI_IRQHandler(EXTI0_IRQn, 0);
        #elif defined STM32F0
            EXTI_IRQHandler(EXTI0_1_IRQn, 0);
        #endif 
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void CInterrupt::EXTI2_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line2) != RESET)
    {
        #if defined(STM32F1) || defined(STM32F4)			
            EXTI_IRQHandler(EXTI2_IRQn, 0);
        #elif defined STM32F0
            EXTI_IRQHandler(EXTI2_3_IRQn, 0);
        #endif 
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
}

void CInterrupt::EXTI3_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        #if defined(STM32F1) || defined(STM32F4)			
            EXTI_IRQHandler(EXTI3_IRQn, 0);
        #elif defined STM32F0
            EXTI_IRQHandler(EXTI2_3_IRQn, 0);
        #endif 
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}

void CInterrupt::EXTI4_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line4) != RESET)
    {
        #if defined(STM32F1) || defined(STM32F4)			
            EXTI_IRQHandler(EXTI4_IRQn, 0);
        #elif defined STM32F0
            EXTI_IRQHandler(EXTI4_15_IRQn, 0);
        #endif 
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
}

void CInterrupt::EXTI9_5_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
        #if defined(STM32F1) || defined(STM32F4)			
            EXTI_IRQHandler(EXTI9_5_IRQn, 0);
        #elif defined STM32F0
            EXTI_IRQHandler(EXTI4_15_IRQn, 0);
        #endif 
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
    if (EXTI_GetITStatus(EXTI_Line6) != RESET)
    {
        #if defined(STM32F1) || defined(STM32F4)			
            EXTI_IRQHandler(EXTI9_5_IRQn, 0);
        #elif defined STM32F0
            EXTI_IRQHandler(EXTI4_15_IRQn, 0);
        #endif 
        EXTI_ClearITPendingBit(EXTI_Line6);
    }
    if (EXTI_GetITStatus(EXTI_Line7) != RESET)
    {
        #if defined(STM32F1) || defined(STM32F4)			
            EXTI_IRQHandler(EXTI9_5_IRQn, 0);
        #elif defined STM32F0
            EXTI_IRQHandler(EXTI4_15_IRQn, 0);
        #endif 
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
    if (EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
        #if defined(STM32F1) || defined(STM32F4)			
            EXTI_IRQHandler(EXTI9_5_IRQn, 0);
        #elif defined STM32F0
            EXTI_IRQHandler(EXTI4_15_IRQn, 0);
        #endif 
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
    if (EXTI_GetITStatus(EXTI_Line9) != RESET)
    {
        #if defined(STM32F1) || defined(STM32F4)			
            EXTI_IRQHandler(EXTI9_5_IRQn, 0);
        #elif defined STM32F0
            EXTI_IRQHandler(EXTI4_15_IRQn, 0);
        #endif 
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}

void CInterrupt::EXTI15_10_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line10) != RESET)
    {
        #if defined(STM32F1) || defined(STM32F4)			
            EXTI_IRQHandler(EXTI15_10_IRQn, 0);
        #elif defined STM32F0
            EXTI_IRQHandler(EXTI4_15_IRQn, 0);
        #endif 
        EXTI_ClearITPendingBit(EXTI_Line10);
    }
    if (EXTI_GetITStatus(EXTI_Line11) != RESET)
    {
        #if defined(STM32F1) || defined(STM32F4)			
            EXTI_IRQHandler(EXTI15_10_IRQn, 0);
        #elif defined STM32F0
            EXTI_IRQHandler(EXTI4_15_IRQn, 0);
        #endif 
        EXTI_ClearITPendingBit(EXTI_Line11);
    }
    if (EXTI_GetITStatus(EXTI_Line12) != RESET)
    {
        #if defined(STM32F1) || defined(STM32F4)			
            EXTI_IRQHandler(EXTI15_10_IRQn, 0);
        #elif defined STM32F0
            EXTI_IRQHandler(EXTI4_15_IRQn, 0);
        #endif 
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
    if (EXTI_GetITStatus(EXTI_Line13) != RESET)
    {
        #if defined(STM32F1) || defined(STM32F4)			
            EXTI_IRQHandler(EXTI15_10_IRQn, 0);
        #elif defined STM32F0
            EXTI_IRQHandler(EXTI4_15_IRQn, 0);
        #endif 
        EXTI_ClearITPendingBit(EXTI_Line13);
    }
    if (EXTI_GetITStatus(EXTI_Line14) != RESET)
    {
        #if defined(STM32F1) || defined(STM32F4)			
            EXTI_IRQHandler(EXTI15_10_IRQn, 0);
        #elif defined STM32F0
            EXTI_IRQHandler(EXTI4_15_IRQn, 0);
        #endif 
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
    if (EXTI_GetITStatus(EXTI_Line15) != RESET)
    {
        #if defined(STM32F1) || defined(STM32F4)			
            EXTI_IRQHandler(EXTI15_10_IRQn, 0);
        #elif defined STM32F0
            EXTI_IRQHandler(EXTI4_15_IRQn, 0);
        #endif 
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
}
