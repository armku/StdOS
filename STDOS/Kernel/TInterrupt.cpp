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

#define NVIC_OFFSET					 ((uint)0x1000)
#define ISRADDR (NVIC_VectTab_RAM+NVIC_OFFSET)
#define ISRLENGTH   100	//�ж�����

TInterrupt Interrupt;

class CInterrupt
{
    public:
		static void Default_Handler();
		static void SysTick_Handler();//systick�жϷ�����
		
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

extern "C"
{
	uint   IsrBuf[ISRLENGTH]   __attribute__ ((at(ISRADDR)));
	uint *vsrom= (uint *) NVIC_VectTab_FLASH;
}

// ��ʼ���ж�������
void TInterrupt::Init()const
{
	//�����ж�������
	for(int i=0;i<ISRLENGTH;i++)
	{
		IsrBuf[i]=vsrom[i];
	}
	//�ж���������ӳ��
	NVIC_SetVectorTable(NVIC_VectTab_RAM, NVIC_OFFSET);
	
	IsrBuf[15]=(uint)&(CInterrupt::SysTick_Handler);
	IsrBuf[53]=(uint)&(CInterrupt::USART1_IRQHandler);
	IsrBuf[54]=(uint)&(CInterrupt::USART2_IRQHandler);
	IsrBuf[55]=(uint)&(CInterrupt::USART3_IRQHandler);
	IsrBuf[68]=(uint)&(CInterrupt::UART4_IRQHandler);
	IsrBuf[69]=(uint)&(CInterrupt::UART5_IRQHandler);
	
	IsrBuf[44]=(uint)&(CInterrupt::TIM2_IRQHandler);
	IsrBuf[45]=(uint)&(CInterrupt::TIM3_IRQHandler);
	IsrBuf[46]=(uint)&(CInterrupt::TIM4_IRQHandler);
	IsrBuf[66]=(uint)&(CInterrupt::TIM5_IRQHandler);
	IsrBuf[70]=(uint)&(CInterrupt::TIM6_IRQHandler);
	IsrBuf[71]=(uint)&(CInterrupt::TIM7_IRQHandler);
	
	IsrBuf[22]=(uint)&(CInterrupt::EXTI0_IRQHandler);
	IsrBuf[23]=(uint)&(CInterrupt::EXTI1_IRQHandler);
	IsrBuf[24]=(uint)&(CInterrupt::EXTI2_IRQHandler);
	IsrBuf[25]=(uint)&(CInterrupt::EXTI3_IRQHandler);
	IsrBuf[26]=(uint)&(CInterrupt::EXTI4_IRQHandler);
	IsrBuf[39]=(uint)&(CInterrupt::EXTI9_5_IRQHandler);
	IsrBuf[56]=(uint)&(CInterrupt::EXTI15_10_IRQHandler);
	
}

void TInterrupt::Process(uint num)const
{

}

void *onIsr[ISRLENGTH];//�ж�

Timer *onTimerPortRcv[18];

// ע���жϺ������жϺţ�������������
bool TInterrupt::Activate(short irq, InterruptCallback isr, void *param)
{
	onIsr[irq]=param;
    switch (irq)
    {
        case 25:
            //TIM1_UP_IRQn
            onTimerPortRcv[0] = (Timer*)param;
            break;
        case 28:
            //TIM2_IRQn
            onTimerPortRcv[1] = (Timer*)param;
            break;
        case 29:
            //TIM3_IRQn
            onTimerPortRcv[2] = (Timer*)param;
            break;
        case 30:
            //TIM4_IRQn
            onTimerPortRcv[3] = (Timer*)param;
            break;
        case 50:
            //TIM5_IRQn
            onTimerPortRcv[4] = (Timer*)param;
            break;
        case 54:
            //TIM6_IRQn
            onTimerPortRcv[5] = (Timer*)param;
            break;
        case 55:
            //TIM7_IRQn
            onTimerPortRcv[6] = (Timer*)param;
            break;
        default:
            break;
    }
    return true;
}
// ����ж�ע��
bool TInterrupt::Deactivate(short irq)
{
    return false;
}
// ���ж�
//bool TInterrupt::Enable(short irq) const{return false;}
// ���ж�
//bool TInterrupt::Disable(short irq) const{return false;}

// �Ƿ��ж�
//bool TInterrupt::EnableState(short irq) const{return false;}
// �Ƿ����
//bool TInterrupt::PendingState(short irq) const{return false;}
// �������ȼ�
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
// ��ȡ���ȼ�
void TInterrupt::GetPriority(short irq)const
{

}
// �������ȼ�
uint TInterrupt::EncodePriority(uint priorityGroup, uint preemptPriority, uint subPriority)const
{
    return 0;
}

// �������ȼ�
void TInterrupt::DecodePriority(uint priority, uint priorityGroup, uint *pPreemptPriority, uint *pSubPriority)const{

}
// ȫ���жϿ���״̬
bool TInterrupt::GlobalState()
{
    return false;
}
// ��ȫ���ж�
void TInterrupt::GlobalEnable()
{
    __ASM volatile("cpsie i");
}

// �ر�ȫ���ж�
void TInterrupt::GlobalDisable()
{
    __ASM volatile("cpsid i");
}


// �Ƿ����ж�����
bool TInterrupt::IsHandler()
{
    return false;
}
// ϵͳ����
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
//�����ж��ߴ���
void EXTI_IRQHandler(ushort num, void *param);
extern SerialPort *_printf_sp;
void OnUsartReceive(ushort num, void *param);
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void CInterrupt::Default_Handler()
{
    while (1);
}
   

#include "TTime.h"
void CInterrupt::SysTick_Handler()
{
    Time.Milliseconds++;
}

//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
void CInterrupt::USART1_IRQHandler()
{
	if (onIsr[37])
    {
        OnUsartReceive(0, onIsr[37]);
    }
}

void CInterrupt::USART2_IRQHandler()
{
    if (onIsr[38])
    {
        OnUsartReceive(1, onIsr[38]);
    }
}

void CInterrupt::USART3_IRQHandler()
{
    if (onIsr[39])
    {
        OnUsartReceive(2, onIsr[39]);
    }
}

void CInterrupt::UART4_IRQHandler()
{
    if (onIsr[52])
    {
        OnUsartReceive(3, onIsr[52]);
    }
}

void CInterrupt::UART5_IRQHandler()
{
    if (onIsr[53])
    {
        OnUsartReceive(4, onIsr[53]);
    }
}

void CInterrupt::TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        if (onTimerPortRcv[1])
        {
            onTimerPortRcv[1]->OnInterrupt();
        }
        TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
    }
}

void CInterrupt::TIM3_IRQHandler()
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        if (onTimerPortRcv[2])
        {
            onTimerPortRcv[2]->OnInterrupt();
        }
        TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
    }
}

void CInterrupt::TIM4_IRQHandler()
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        if (onTimerPortRcv[3])
        {
            onTimerPortRcv[3]->OnInterrupt();
        }
        TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
    }
}

void CInterrupt::TIM5_IRQHandler()
{
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        if (onTimerPortRcv[4])
        {
            onTimerPortRcv[4]->OnInterrupt();
        }
        TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
    }
}

void CInterrupt::TIM6_IRQHandler()
{
    #if defined(STM32F1) || defined(STM32F4)  
        if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
        {
            if (onTimerPortRcv[5])
            {
                onTimerPortRcv[5]->OnInterrupt();
            }
            TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
        }
    #elif defined STM32F0
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update); //������жϱ�־λ���Ա��´�ʹ�á�
        if (onTimerPortRcv[5])
        {
            onTimerPortRcv[5]->OnInterrupt();
        }
    #endif 
}

void CInterrupt::TIM7_IRQHandler()
{
    #if defined(STM32F1) || defined(STM32F4) 
        if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
        {
            if (onTimerPortRcv[6])
            {
                onTimerPortRcv[6]->OnInterrupt();
            }
            TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);
        }
    #elif defined STM32F0
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update); //������жϱ�־λ���Ա��´�ʹ�á�
        if (onTimerPortRcv[6])
        {
            onTimerPortRcv[6]->OnInterrupt();
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
