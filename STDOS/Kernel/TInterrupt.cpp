#include "SerialPort.h"
#include "TInterrupt.h"
#include "Timer.h"

extern void *onIsr[]; //�ж�

// ��ʼ���ж�������
void TInterrupt::Init()const
{    
	this->OnInit();
}
// ע���жϺ������жϺţ�������������
bool TInterrupt::Activate(short irq, InterruptCallback isr, void *param)
{
	onIsr[irq] = param;
	return true;
}
