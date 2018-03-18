#include "SerialPort.h"
#include "TInterrupt.h"
#include "Timer.h"

extern void *onIsr[]; //中断

// 初始化中断向量表
void TInterrupt::Init()const
{    
	this->OnInit();
}
// 注册中断函数（中断号，函数，参数）
bool TInterrupt::Activate(short irq, InterruptCallback isr, void *param)
{
	onIsr[irq] = param;
	return true;
}
