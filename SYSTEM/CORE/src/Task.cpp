#include <stdio.h>
#include "Task.h"
#include "Sys.h"

template <typename T1,typename T2>
T1 sum(T1 x,T2 y)
{
	return x+y;
}

Task::Task(TaskScheduler *scheduler)
{
    _Scheduler = scheduler;

    Times = 0;
    CpuTime = 0;
    SleepTime = 0;
    Cost = 0;
    Enable = true;
}

/*Task::~Task()
{
if(ID) _Scheduler->Remove(ID);
}*/
// ��ʾ״̬
void Task::ShowStatus()
{
    debug_printf("Task::Status ���� %d [%d] ִ�� %dus ƽ�� %dus\r\n", ID, Times, CpuTime, Cost);
}
