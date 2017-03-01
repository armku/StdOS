#include <stdio.h>
#include "Task.h"
#include "Sys.h"

template <typename T1, typename T2> T1 sum(T1 x, T2 y)
{
    return x + y;
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
// 显示状态
void Task::ShowStatus()
{
	debug_printf("Task::%-12s %2d", Name, ID);
	if(Times>1000000)
	{
		debug_printf("[%d]",Times);
	}
	else if(Times>100000)
	{
		debug_printf("[%d] ",Times);
	}
	else if(Times>10000)
	{
		debug_printf("[%d]  ",Times);
	}
	else if(Times>1000)
	{
		debug_printf("[%d]   ",Times);
	}
	else if(Times>100)
	{
		debug_printf("[%d]    ",Times);
	}
	else if(Times>10)
	{
		debug_printf("[%d]     ",Times);
	}
	else
	{
		debug_printf("[%d]      ",Times);
	}
	if(Cost>1000)
	{
		debug_printf(" 平均 %3ums", Cost/1000);
	}
	else
	{
		debug_printf(" 平均 %3uus", Cost);
	}
	if(CpuTime>1000000)
	{
		debug_printf(" 最大 %3ds ", CpuTime/1000000);
	}
	else if(CpuTime>1000)
	{
		debug_printf(" 最大 %3dms", CpuTime/1000);
	}
	else
	{
		debug_printf(" 最大 %3dus", CpuTime);
	}
    if (this->Period >= 1000000)
    {
        debug_printf(" 周期 %3lds ",  Period / 1000000);
    }
    else if (this->Period >= 1000)
    {
        debug_printf(" 周期 %3ldms",  Period / 1000);
    }
    else
    {
        debug_printf(" 周期 %3ldus", Period);
    }
	debug_printf(" %s\r\n",this->Enable ? " " : "禁用");
}

bool Task::operator == (Task &tsk)
{
    if (tsk.ID == this->ID)
    {
        return true;
    }
    return false;
}
