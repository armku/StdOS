#include "SerialPort.h"
#include "TInterrupt.h"
#include "Timer.h"

extern void *onIsr[]; //中断

unsigned int GetIPSR(void)
{
//  return __get_CPSR();
	return 0;
}
// 注册中断函数（中断号，函数，参数）
bool TInterrupt::Activate(short irq, InterruptCallback isr, void *param)
{
    onIsr[irq] = param;
    return true;
}
// 全局中断开关状态
bool TInterrupt::GlobalState()
{
    return false;
}

// 是否在中断里面
bool TInterrupt::IsHandler()
{
    return false;
}
// 获取优先级
void TInterrupt::GetPriority(short irq)const{

}
