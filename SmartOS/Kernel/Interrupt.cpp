#include "Kernel\Sys.h"

#include "Interrupt.h"

extern InterruptCallback Vectors[];      // 对外的中断向量表
extern void* VectorParams[];       // 每一个中断向量对应的参数

TInterrupt Interrupt;

void TInterrupt::Init() const
{
	//OnInit();
}
//bool TInterrupt::Activate(short irq, InterruptCallback isr, void* param)
//{
//	short irq2 = irq + 16; // exception = irq + 16
////	Vectors[irq2] = isr;
////	VectorParams[irq2] = param;

//	return OnActivate(irq);
//}
