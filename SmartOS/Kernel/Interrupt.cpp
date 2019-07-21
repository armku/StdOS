#include "Kernel\Sys.h"

#include "Interrupt.h"

extern InterruptCallback Vectors[];      // 对外的中断向量表
extern void* VectorParams[];       // 每一个中断向量对应的参数

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
// 关键性代码，放到开头
INROOT void TInterrupt::Process(uint num) const
{
	//auto& inter	= Interrupt;
	if (!Vectors[num]) return;

	// 内存检查
#if DEBUG
	//Sys.CheckMemory();
#endif

	// 找到应用层中断委托并调用
	auto isr = (InterruptCallback)Vectors[num];
	void* param = (void*)VectorParams[num];
	isr(num - 16, param);
}

// 系统挂起
void TInterrupt::Halt()
{
#if DEBUG
	//TraceStack::Show();

	//auto sp	= SerialPort::GetMessagePort();
	//if(sp) sp->Flush();
#endif
	while (true);
}

// 智能IRQ，初始化时备份，销毁时还原
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

