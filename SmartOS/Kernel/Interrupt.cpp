#include "Kernel\Sys.h"

#include "Interrupt.h"

extern InterruptCallback Vectors[];      // ������ж�������
extern void* VectorParams[];       // ÿһ���ж�������Ӧ�Ĳ���

TInterrupt Interrupt;

void TInterrupt::Init() const
{
	//OnInit();
}

bool TInterrupt::Activate(short irq, InterruptCallback isr, void* param)
{
	short irq2 = irq + 16; // exception = irq + 16
	Vectors[irq2] = isr;
	VectorParams[irq2] = param;

	return OnActivate(irq);
}

bool TInterrupt::Deactivate(short irq)
{
	short irq2 = irq + 16; // exception = irq + 16
	Vectors[irq2] = 0;
	VectorParams[irq2] = 0;

	return OnDeactivate(irq);
}
// �ؼ��Դ��룬�ŵ���ͷ
INROOT void TInterrupt::Process(uint num) const
{
	//auto& inter	= Interrupt;
	if (!Vectors[num]) return;

	// �ڴ���
#if DEBUG
	//Sys.CheckMemory();
#endif

	// �ҵ�Ӧ�ò��ж�ί�в�����
	auto isr = (InterruptCallback)Vectors[num];
	void* param = (void*)VectorParams[num];
	isr(num - 16, param);
}

// ϵͳ����
void TInterrupt::Halt()
{
#if DEBUG
	//TraceStack::Show();

	//auto sp	= SerialPort::GetMessagePort();
	//if(sp) sp->Flush();
#endif
	while (true);
}

// ����IRQ����ʼ��ʱ���ݣ�����ʱ��ԭ
INROOT SmartIRQ::SmartIRQ(bool enable)
{
	_state = TInterrupt::GlobalState();
	if (enable)
		TInterrupt::GlobalEnable();
	else
		TInterrupt::GlobalDisable();
}

INROOT SmartIRQ::~SmartIRQ()
{
	//__set_PRIMASK(_state);
	if (_state)
		TInterrupt::GlobalDisable();
	else
		TInterrupt::GlobalEnable();
}

