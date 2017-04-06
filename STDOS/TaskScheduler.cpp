#include "TaskScheduler.h"
#include "Sys.h"
#include <limits.h>
#include "TTime.h"

TaskScheduler::TaskScheduler(char *name)
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


// 创建任务，返回任务编号。dueTime首次调度时间ms，period调度间隔ms，-1表示仅处理一次
uint TaskScheduler::Add(Action func, void *param, long dueTime, long period, const char *name)
{
    if (dueTime > 0)
    {
        dueTime *= 1000;
    }
    if (period > 0)
    {
        period *= 1000;
    }

    Task *task = new Task(this);
    task->ID = mgid++;
    task->Callback = func;
    task->Param = param;
    task->Period = period;
    task->NextTime = Time.Current() + dueTime;
    task->Name = name;
    if (dueTime < 0)
    {
        task->Enable = false;
    }

    this->Count++;
    _Tasks.Add(task);
    #if DEBUG
        debug_printf("%s::添加%2d %-11s", Name, task->ID, task->Name);
        if (dueTime >= 1000000)
        {
            debug_printf("First = %3lds ", dueTime / 1000000);
        }
        else if (dueTime >= 1000)
        {
            debug_printf("First = %3ldms", dueTime / 1000);
        }
        else
        {
            debug_printf("First = %3ldus", dueTime);
        }

        // 输出长整型%ld，无符号长整型%llu
        if (period >= 1000000)
        {
            debug_printf(" Period = %3lds \r\n", period / 1000000);
        }
        else if (period >= 1000)
        {
            debug_printf(" Period = %3ldms\r\n", period / 1000);
        }
        else
        {
            debug_printf(" Period = %3ldus\r\n", period);
        }
    #endif 
    return task->ID;
}

//设置任务执行、就绪状态
void TaskScheduler::SetTask(uint taskid, bool onoff, long delaytime)
{
    if (taskid > this->Count)
    {
        //超范围，返回
        return ;
    }
    this->_Tasks[taskid]->Enable = onoff;
    if (onoff)
    {
        this->_Tasks[taskid]->NextTime = Time.Current() + delaytime * 1000;
    }
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

    #ifdef DEBUG
        Add(ShowStatus, this, 1 *1000, 30 *1000, "任务状态");
    #else 
        Add(ShowTime, this, 2 *1000, 20 *1000, "时间显示");
    #endif 

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
    UInt64 now;
    now = Time.Current(); // 当前时间。减去系统启动时间，避免修改系统时间后导致调度停摆	

    UInt64 min = UInt64_Max; // 最小时间，这个时间就会有任务到来
    UInt64 end = Time.Current() + usMax;

    // 需要跳过当前正在执行任务的调度
    //Task* _cur = Current;

    int i =  - 1;
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

            UInt64 now2 = Time.Current();
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
                debug_printf("Task::Execute 任务:%s %d [%d] 执行时间过长 %dus 睡眠 %dus\r\n", task->Name, task->ID, task->Times, cost, task->SleepTime);
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
void TaskScheduler::ShowTime(void *param)
{
    UInt64 curms = Time.Ms();
    debug_printf("Time: %02lld:%02lld:%02lld.%03lld\n", curms / 3600000, curms / 60000 % 60, curms / 1000 % 60, curms % 1000);
}

// 显示状态
void TaskScheduler::ShowStatus(void *param)
{
    static UInt64 runCounts = 0;
    float RunTimes = 0;
    float RunTimesAvg = 0;
    Task *tsk;
    byte buf[1];

    runCounts++;
    TaskScheduler *ts = (TaskScheduler*)param;
    UInt64 curms = Time.Ms();

    //统计运行时间
    RunTimes = 0;
    RunTimesAvg = 0;
    for (int j = 0; j < ts->Count; j++)
    {
        tsk = ts->_Tasks[j];
        RunTimes += tsk->Cost *tsk->Times;
        RunTimesAvg += tsk->Cost;
    }
    RunTimesAvg /= ts->Count;
    //SRAM   0X20000000-0X3FFFFFFF 共512MB
    //SCODE  0X00000000-0X1FFFFFFF 共512MB
	debug_printf("\r\n");
    debug_printf("Task::%s [%llu]", "ShowStatus", runCounts);
    debug_printf("负载 %0.2f%% ", RunTimes / 10 / curms);
    if (RunTimesAvg >= 1000)
    {
        debug_printf("平均 %3.1fms ", RunTimesAvg / ts->Count / 1000);
    }
    else
    {
        debug_printf("平均 %3.0fus ", RunTimesAvg / ts->Count);
    }
    debug_printf("当前 1970-01-01 23 00:00");
    debug_printf("启动 ");
    debug_printf("%02lld:%02lld:%02lld.%03lld ", curms / 3600000, curms / 60000 % 60, curms / 1000 % 60, curms % 1000);
    debug_printf("堆 %u/%u", &(buf[0]) - 0X20000000, 1024);
    debug_printf("\r\n");

    int i =  - 1;
    Task *task = ts->Current;
    task->ShowStatus();
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
    int i =  - 1;

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
