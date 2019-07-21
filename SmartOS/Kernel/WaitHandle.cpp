#include "Task.h"

#include "WaitHandle.h"

WaitHandle::WaitHandle()
{
	Result		= false;

	TaskID = Task::Current().ID;
	State = nullptr;
}

bool WaitHandle::WaitOne(int ms)
{
	// ʵ�ʿ���ʱ�䣬-1��ʾ���޵ȴ�
	if(ms == -1) ms	= 0x7FFFFFFF;

	Task::Scheduler()->ExecuteForWait(ms, Result);

	return Result;
}

void WaitHandle::Set()
{
	Result	= true;
}
