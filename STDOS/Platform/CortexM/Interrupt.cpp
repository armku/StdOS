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
void TInterrupt::GetPriority(short irq)const{

}
