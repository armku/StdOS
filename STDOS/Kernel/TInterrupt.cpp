#include "SerialPort.h"
#include "TInterrupt.h"
#include "Timer.h"

extern void *onIsr[]; //中断

// 初始化中断向量表
//IDA OK
void TInterrupt::Init()const
{    
	this->OnInit();
}

void TInterrupt::Process(uint num)const{

}

// 解除中断注册
bool TInterrupt::Deactivate(short irq)
{
    return false;
}

// 系统挂起
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
void TInterrupt::GetPriority(short irq)const {

}

