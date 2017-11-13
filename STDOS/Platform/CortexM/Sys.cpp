/*
系统能自动识别常见芯片类型、主频、Flash大小、Ram大小
Sys.ID 是12字节芯片唯一标识、也就是ChipID，同一批芯片仅前面几个字节不同
毫秒级睡眠期间，系统将会安排执行其他耗时较短的任务。如果没有可用任务，系统将会进入低功耗模式，以节省能耗

 */
#include "TTime.h"
#include "Task.h"
#include <string.h>
#include "Array.h"
#include "TInterrupt.h"
#include "SerialPort.h"

String *CPUName;
int SmartOS_Log(const String *str)
{
    int ret;

    if (Sys.Clock && Sys.MessagePort != COM_NONE)
    {
        auto sp = SerialPort::GetMessagePort();
        if (sp && sp->ByteTime)
            ret = sp->Write(*str);
        else
            ret = 0;
    }
    else
    {
        ret = 0;
    }
    return ret;
}

void TSys::SetStackTop(uint addr)
{
	//return __set_MSP(addr);
}	

void TSys::OnStart()
{
	
}	
// 初始化系统时钟
void TSys::InitClock()
{	    
    //    this->Inited = 1;
}	

bool TSys::CheckMemory() const
{
	return true;
}		

uint TSys::HeapBase() const	// 堆起始地址，前面是静态分配内存
{
//	return &_heap_base;
	return 0;
}	
uint TSys::StackTop() const	// 栈顶，后面是初始化不清零区域
{
	//return (*((_WORD *)this + 31) << 10) + 536870656;
	return 0;
}	
