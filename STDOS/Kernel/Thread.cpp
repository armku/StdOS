#include "Thread.h"
// ���Sleep�Ƿ����
bool Thread::CheckExpire()
{
	return false;
}
// ���ջ�Ƿ����
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

// ˯��ָ����������
void Thread::Sleep(uint ms)	
{
}
void Thread::Add(Thread* thread)
{
}
void Thread::Remove(Thread* thread)
{
}
// ϵͳ�δ�ʱ�Ӷ�ʱ���ø÷���
void Thread::OnTick()
{
}

void Thread::Init()
{
}
void Thread::OnInit()
{
}
// ÿ���߳̽���ʱִ�и÷����������߳�
void Thread::OnEnd()	
{
}
// ׼����������
byte Thread::BuildReady()
{
	return 0;
}
// ϵͳ�̵߳��ȿ�ʼ
void Thread::Schedule()
{
}
void Thread::OnSchedule()
{
}
	// �л��̣߳������л�ʱ��Ƭ����һ���߳�
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
