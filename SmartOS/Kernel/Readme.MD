#include "Task.h"
extern TaskScheduler *_Scheduler;
//#define _TASKEST_CPP
#ifdef _TASKEST_CPP
void TaskTestRoutin(void * param)
{
	debug_printf("00TaskTest ID:%d\n", _Scheduler->Current->ID);
	Sys.Sleep(900);
	debug_printf("01TaskTest ID:%d\n", _Scheduler->Current->ID);
	Sys.Sleep(800);
	debug_printf("02TaskTest ID:%d\n", _Scheduler->Current->ID);
}
void Demo(void* param)
{
	debug_printf("Demo ID:%d\n", _Scheduler->Current->ID);
}

void TaskTest()
{
	Sys.AddTask(TaskTestRoutin, 0, 1000, 1000, "TaskTestRoutin");
	Sys.AddTask(Demo, 0, 1100, 5000, "Demo");
}
#endif
