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

// ��ʼ���ж�������
//IDA OK
void TInterrupt::Init()const
{    
	this->OnInit();
}

void TInterrupt::Process(uint num)const{

}

// ע���жϺ������жϺţ�������������
bool TInterrupt::Activate(short irq, InterruptCallback isr, void *param)
{
    onIsr[irq] = param;
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
		nvic.NVIC_IRQChannelPriority = priority;
    #endif 

    NVIC_Init(&nvic);
	NVIC_SetPriority((IRQn_Type)irq, priority);
}

// ��ȡ���ȼ�
void TInterrupt::GetPriority(short irq)const{

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
