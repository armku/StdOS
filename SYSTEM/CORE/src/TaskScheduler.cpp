#include "TaskScheduler.h"
#include "Sys.h"
#include <limits.h>

#define UInt64_Max LONG_MAX

TaskScheduler::TaskScheduler(char* name)
{
    this->Name = name;

    this->mgid = 1;

    this->Running = false;
    this->Current = NULL;
    this->Count = 0;
}

TaskScheduler::~TaskScheduler()
{
    this->Current = NULL;
    #if 0
        _Tasks.DeleteAll().Clear();
    #endif 
}


// 创建任务，返回任务编号。dueTime首次调度时间us，period调度间隔us，-1表示仅处理一次
uint TaskScheduler::Add(Action func, void *param, ulong dueTime, long period, const char *name)
{
    Task *task = new Task(this);
    task->ID = mgid++;
    task->Callback = func;
    task->Param = param;
    task->Period = period;
    task->NextTime = Time.Current() + dueTime;
	task->Name=name;

    this->Count++;
    _Tasks.Add(task);
    // 输出长整型%ld，无符号长整型%llu
    //debug_printf("%s::添加任务%d 0x%08x FirstTime=%llums Period=%ldms\r\n", Name,task->ID, func, dueTime, period);
	debug_printf("%s::添加%2d %-10s FirstTime = %8llums Period = %8ldms\r\n", Name,task->ID, task->Name, dueTime, period);

    return task->ID;
}

void TaskScheduler::Remove(uint taskid)
{
    int i =  - 1;

    while (_Tasks.MoveNext(i))
    {
        Task *task = _Tasks[i];
        if (task->ID == taskid)
        {            
            _Tasks.RemoveAt(i);
            debug_printf("%s::删除任务%d 0x%08x\r\n", Name, task->ID, (unsigned int)task->Callback);
            // 首先清零ID，避免delete的时候再次删除
            task->ID = 0;
            delete task;
            break;
        }
    }
}

void TaskScheduler::Start()
{
    if (Running)
    {
        return ;
    }

    Add(ShowTime, NULL, 2*1000*1000, 30*1000*1000,"时间显示");
    //Add(ShowStatus, this, 3*1000*1000, 30*1000*1000,"任务显示");

    debug_printf("%s::准备就绪 开始循环处理%d个任务！\r\n\r\n", Name, Count);

    Running = true;
    while (Running)
    {
        Execute(0xFFFFFFFF);
    }
    debug_printf("%s停止调度，共有%d个任务！\r\n", Name, Count);
}

void TaskScheduler::Stop()
{
    debug_printf("%s停止！\r\n", Name);
    Running = false;
}

// 执行一次循环。指定最大可用时间
void TaskScheduler::Execute(uint usMax)
{	
	ulong now;	
	now= Time.Current() - Sys.StartTime; // 当前时间。减去系统启动时间，避免修改系统时间后导致调度停摆	
			
    ulong min = UInt64_Max; // 最小时间，这个时间就会有任务到来
    ulong end = Time.Current() + usMax;

    // 需要跳过当前正在执行任务的调度
    //Task* _cur = Current;

    int i =   -1;
    while (_Tasks.MoveNext(i))
    {		
        Task *task = _Tasks[i];
        //if(task && task != _cur && task->Enable && task->NextTime <= now)
        if (task && task->Enable && task->NextTime <= now)
        {
            // 不能通过累加的方式计算下一次时间，因为可能系统时间被调整
            task->NextTime = now + task->Period;
            if (task->NextTime < min)
            {
                min = task->NextTime;
            }

            ulong now2 = Time.Current();
            task->SleepTime = 0;

            this->Current = task;
            task->Callback(task->Param);
            this->Current = NULL;

            // 累加任务执行次数和时间
            task->Times++;
            int cost = (int)(Time.Current() - now2);
            if (cost < 0)
            {
                cost =  - cost;
            }
            //if(cost > 0)
            {
                task->CpuTime += cost - task->SleepTime;
                task->Cost = task->CpuTime / task->Times;
            }
            if (cost > 500000)
            {
                debug_printf("Task::Execute 任务 %d [%d] 执行时间过长 %dus 睡眠 %dus\r\n", task->ID, task->Times, cost, task->SleepTime);
            }
            // 如果只是一次性任务，在这里清理
            if (task->Period < 0)
            {
                Remove(task->ID);
            }
        }

        // 如果已经超出最大可用时间，则退出
        if (!usMax || Time.Current() > end)
        {
            return ;
        }		
    }

    // 如果有最小时间，睡一会吧
    now = Time.Current(); // 当前时间
    if (min != UInt64_Max && min > now)
    {
        min -= now;
        //debug_printf("TaskScheduler::Execute 等待下一次任务调度 %uus\r\n", (uint)min);           
        //// 最大只允许睡眠1秒，避免Sys.Delay出现设计错误，同时也更人性化
        //if(min > 1000000) min = 1000000;
        //Sys.Delay(min);
        Time.Sleep(min);
    }
}
//显示时间
void TaskScheduler::ShowTime(void * param)
{
	UInt64 curms=Time.Ms();
	debug_printf("Time: %02lld:%02lld:%02lld\n",curms/3600000,curms/60000%60,curms/1000%60);
}
// 显示状态
void TaskScheduler::ShowStatus(void *param)
{
    TaskScheduler *ts = (TaskScheduler*)param;

    int i =  -1;	
	
    while (ts->_Tasks.MoveNext(i))
    {
        Task *task = ts->_Tasks[i];
        if (task)
        {
            task->ShowStatus();
        }
    }		
}

Task *TaskScheduler::operator[](int taskid)
{
    int i =  -1;

    while (_Tasks.MoveNext(i))
    {
        Task *task = _Tasks[i];
        if (task && task->ID == taskid)
        {
            return task;
        }
    }
    return NULL;
}
