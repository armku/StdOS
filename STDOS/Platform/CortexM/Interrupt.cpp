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
extern void *onIsr[]; //�ж�


unsigned int GetIPSR(void)
{
//  return __get_CPSR();
}
//int UserHandler()
//{
////  unsigned int v0; // r0@1

////  v0 = GetIPSR();
////  return TInterrupt::Process((TInterrupt *)&Interrupt, v0);
//}
//int assert_failed(unsigned __int8 *a1, unsigned int a2)
//{
////  TInterrupt *v2; // r0@1

////  v2 = (TInterrupt *)SmartOS_printf("Assert Failed! Line %d, %s\r\n");
////  return TInterrupt::Halt(v2);
//}
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

// ��ȫ���ж�
void TInterrupt::GlobalEnable()
{
    __ASM volatile("cpsie i");
}
bool TInterrupt::OnDeactivate(short irq)
{
    return false;
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
// ��ȡ���ȼ�
void TInterrupt::GetPriority(short irq)const{

}

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
		nvic.NVIC_IRQChannelPriority = priority;
    #endif 

    NVIC_Init(&nvic);
	NVIC_SetPriority((IRQn_Type)irq, priority);
}
