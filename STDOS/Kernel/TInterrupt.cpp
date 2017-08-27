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

// ϵͳ����
void TInterrupt::Halt(){}

bool TInterrupt::OnActivate(short irq)
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
