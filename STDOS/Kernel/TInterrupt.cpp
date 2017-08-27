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

// 开中断
//bool TInterrupt::Enable(short irq) const{return false;}
// 关中断
//bool TInterrupt::Disable(short irq) const{return false;}

// 是否开中断
//bool TInterrupt::EnableState(short irq) const{return false;}
// 是否挂起
//bool TInterrupt::PendingState(short irq) const{return false;}
// 设置优先级

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
