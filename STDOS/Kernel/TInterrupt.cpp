#include "SerialPort.h"
#include "TInterrupt.h"
#include "Timer.h"

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
#include "SerialPort.h"
#include "TInterrupt.h"
#include "Timer.h"

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

// �Ƿ����ж�����
bool TInterrupt::IsHandler()
{
	return false;
}
// ��ȡ���ȼ�
void TInterrupt::GetPriority(short irq)const {

}

