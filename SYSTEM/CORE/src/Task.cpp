#include <stdio.h>
#include "Task.h"

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
