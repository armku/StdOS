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
extern void *onIsr[]; //�ж�


unsigned int GetIPSR(void)
{
//  return __get_CPSR();
	return 0;
}
// ע���жϺ������жϺţ�������������
bool TInterrupt::Activate(short irq, InterruptCallback isr, void *param)
{
    onIsr[irq] = param;
    return true;
}
// ȫ���жϿ���״̬
bool TInterrupt::GlobalState()
{
    return false;
}

//// ��ȫ���ж�
//void TInterrupt::GlobalEnable()
//{
//    __ASM volatile("cpsie i");
//}
//bool TInterrupt::OnDeactivate(short irq)
//{
//    return false;
//}
//// �ر�ȫ���ж�
//void TInterrupt::GlobalDisable()
//{
//    __ASM volatile("cpsid i");
//}

// �Ƿ����ж�����
bool TInterrupt::IsHandler()
{
    return false;
}
// ��ȡ���ȼ�
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
