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
