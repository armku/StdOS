#include "Thread.h"
// 检查Sleep是否过期
bool Thread::CheckExpire()
{
	return false;
}
// 检查栈是否溢出
void Thread::CheckStack()	
{
}
Thread::Thread(Action callback, void* state, uint stackSize)
{
	
}
Thread::~Thread()
{
	
}

void Thread::Start()
{
}
void Thread::Stop()
{
}
void Thread::Suspend()
{
}
void Thread::Resume()
{
}

// 睡眠指定毫秒数。
void Thread::Sleep(uint ms)	
{
}
void Thread::Add(Thread* thread)
{
}
void Thread::Remove(Thread* thread)
{
}
// 系统滴答时钟定时调用该方法
void Thread::OnTick()
{
}

void Thread::Init()
{
}
void Thread::OnInit()
{
}
// 每个线程结束时执行该方法，销毁线程
void Thread::OnEnd()	
{
}
// 准备就绪队列
byte Thread::BuildReady()
{
	return 0;
}
// 系统线程调度开始
void Thread::Schedule()
{
}
void Thread::OnSchedule()
{
}
	// 切换线程，马上切换时间片给下一个线程
void Thread::Switch()
{
}

bool Thread::CheckPend()
{
	return false;
}
void Thread::OnSwitch()
{
}
void Thread::QueueUserWorkItem(Action func, void* param)
{
}
