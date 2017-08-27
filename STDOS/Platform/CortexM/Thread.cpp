#include "Thread.h"


//int PendSV_Handler()
//{
//  int v0; // r4@0
//  int v1; // r5@0
//  int v2; // r6@0
//  int v3; // r7@0
//  int v4; // r8@0
//  int v5; // r9@0
//  int v6; // r10@0
//  int v7; // r11@0
//  unsigned int *v8; // r2@1
//  unsigned int v9; // r0@2
//  int v10; // r4@3
//  int result; // r0@3

//  __disable_irq();
//  v8 = (unsigned int *)curStack;
//  if ( curStack )
//  {
//    v9 = __get_CPSR() - 32;
//    *(_DWORD *)v9 = v0;
//    *(_DWORD *)(v9 + 4) = v1;
//    *(_DWORD *)(v9 + 8) = v2;
//    *(_DWORD *)(v9 + 12) = v3;
//    *(_DWORD *)(v9 + 16) = v4;
//    *(_DWORD *)(v9 + 20) = v5;
//    *(_DWORD *)(v9 + 24) = v6;
//    *(_DWORD *)(v9 + 28) = v7;
//    *v8 = v9;
//  }
//  v10 = *newStack;
//  result = newStack + 32;
//  __asm { MSR.W   PSP, R0 }
//  __enable_irq();
//  return result;
//}
//void __fastcall QueueUserWorkItem(void (__cdecl *a1)(void *), void *a2)
//{
//  ;
//}
// 检查栈是否溢出
void Thread::CheckStack()
{
}

void Thread::OnSchedule()
{
}
// 每个线程结束时执行该方法，销毁线程
void Thread::OnEnd()
{
}	
void Thread::OnInit()
{
}
void Thread::OnSwitch()
{
}
bool Thread::CheckPend()
{
}
