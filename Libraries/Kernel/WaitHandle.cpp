#include "WaitHandle.h"
#include "Task.h"

WaitHandle::WaitHandle()
{
	this->Result=false;
	this->TaskID=Task::Current().ID;
	this->State = NULL;
}
// 等待一个
bool WaitHandle::WaitOne(int ms)
{
    if (ms ==  - 1)
        ms = 0x7FFFFFFF;
    Task::Scheduler()->ExecuteForWait(ms, Result);
    return this->Result;
}
	
//void Reset();
// 设置结果
void WaitHandle::Set()
{
	this->Result=true;
}	
