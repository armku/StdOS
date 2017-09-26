#include "Task.h"
#include "TTime.h"

Task::Task()
{
    this->Init();
}

Task::~Task()
{
    if (this->ID)
        this->Host->Remove(this->ID);
}

// 执行任务。返回是否正常执行。
bool Task::Execute(UInt64 now)
{
    if (this->Deepth < this->MaxDeepth)
    {
        this->Deepth++;
        if (this->Event)
            this->Enable = false;
        else
            this->NextTime = this->Period + now;

        TimeCost costms;
        this->SleepTime = 0;

        // v7 = *(_DWORD *)(*(_DWORD *)pthis + 48);
        //*(_DWORD *)(*(_DWORD *)pthis + 48) = pthis;
        this->Callback(this->Param);
        //*(_DWORD *)(*(_DWORD *)pthis + 48) = v7;

        this->Times++;
        int costMsCurrent = costms.Elapsed() - this->SleepTime;
        if (this->MaxCost < costMsCurrent)
            this->MaxCost = costMsCurrent;
        this->Cost = (5 *this->Cost + 3 * costMsCurrent) >> 3;
        this->CostMs = this->Cost / 1000;

        if (costMsCurrent > 500000)
            debug_printf("Task::Execute 任务:%s %d [%d] 执行时间过长 %dus 睡眠 %dus\r\n", this->Name, this->ID, this->Times, costMsCurrent, this->SleepTime);
        if (!this->Event && this->Period < 0)
            Task::Scheduler()->Remove(this->ID);
        this->Deepth--;
        return true;
    }
    else
    {
        return false;
    }
}

// 设置任务的开关状态，同时运行指定任务最近一次调度的时间，0表示马上调度
void Task::Set(bool enable, int msNextTime)
{
    if (msNextTime >= 0)
    {
        this->NextTime = msNextTime + Sys.Ms() *1000;
    }
    this->Enable = enable;
    if (enable)
    {
        Task::Scheduler()->SkipSleep();
    }
}

// 显示状态
void Task::ShowStatus()
{
    debug_printf("Task::%-12s %2d", this->Name, this->ID);
    if (this->Times >= 1000000)
    {
        debug_printf("[%d]", this->Times);
    }
    else if (this->Times >= 100000)
    {
        debug_printf("[%d] ", this->Times);
    }
    else if (this->Times >= 10000)
    {
        debug_printf("[%d]  ", this->Times);
    }
    else if (this->Times >= 1000)
    {
        debug_printf("[%d]   ", this->Times);
    }
    else if (this->Times >= 100)
    {
        debug_printf("[%d]    ", this->Times);
    }
    else if (this->Times >= 10)
    {
        debug_printf("[%d]     ", this->Times);
    }
    else
    {
        debug_printf("[%d]      ", this->Times);
    }
    float cpuPercent = 0;
    cpuPercent = 0;
    cpuPercent = this->Cost *this->Times / 10;
    cpuPercent /= Sys.Ms();

    if (cpuPercent >= 10)
    {
        debug_printf(" 平均[%4.1f%%] ", cpuPercent);
    }
    else
    {
        debug_printf(" 平均[%0.2f%%] ", cpuPercent);
    }

    if (Cost >= 1000)
    {
        debug_printf("%3ums", this->Cost / 1000);
    }
    else
    {
        debug_printf("%3uus", this->Cost);
    }
    if (this->MaxCost >= 1000000)
    {
        debug_printf(" 最大 %3ds ", this->MaxCost / 1000000);
    }
    else if (this->MaxCost >= 1000)
    {
        debug_printf(" 最大 %3dms", this->MaxCost / 1000);
    }
    else
    {
        debug_printf(" 最大 %3dus", this->MaxCost);
    }
    if (this->Period >= 1000000)
    {
        debug_printf(" 周期 %3lds ", this->Period / 1000000);
    }
    else if (this->Period >= 1000)
    {
        debug_printf(" 周期 %3ldms", this->Period / 1000);
    }
    else
    {
        debug_printf(" 周期 %3ldus", this->Period);
    }
    debug_printf(" %s\r\n", this->Enable ? " " : "禁用");
}

TaskScheduler *_Scheduler;
// 全局任务调度器
TaskScheduler *Task::Scheduler()
{
    if (!_Scheduler)
    {
        _Scheduler = new TaskScheduler("Sys");
    }
    return _Scheduler;
}

Task *Task::Get(int taskid)
{
    return (*Task::Scheduler())[taskid];
}

Task &Task::Current()
{
    return  *Task::Scheduler()->Current;

}

bool Task::CheckTime(UInt64 end, bool isSleep)
{
    bool ret;

    if (this->Deepth < this->MaxDeepth)
    {
        uint mscur = Sys.Ms();
        //		if(!this->Enable)
        //		{
        //			ret=false;
        //		}
        //		else 
        if (end >= this->NextTime)
        {
            if (isSleep)
            {
                if (this->Event || (this->Period >= 0))
                {
                    ret = true;
                }
                else
                {
                    ret = false;
                }
            }
            else
            {
                ret = true;
            }
        }
        else
        {
            ret = false;
        }
    }
    else
    {
        ret = false;
    }
    return ret;
}

void Task::Init()
{
    this->ID = 0; // 编号
    this->Name = nullptr; // 名称

    this->Callback = nullptr; // 回调
    this->Param = nullptr; // 参数

    this->Period = 0; // 周期ms 5
    this->NextTime = 0; // 下一次执行时间ms 3

    this->Times = 0; // 执行次数 8
    this->SleepTime = 0; // 当前睡眠时间us 9
    this->Cost = 0; // 平均执行时间us 10
    this->CostMs = 0; // 平均执行时间ms 11
    this->MaxCost = 0; // 最大执行时间us 12

    this->Enable = true;
    this->Event = false;
    this->Deepth = 0;
    this->MaxDeepth = 1;
}

//bool Task::operator == (Task &tsk)
//{
//    if (tsk.ID == this->ID)
//    {
//        return true;
//    }
//    return false;
//}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


template <typename T1, typename T2> T1 sum(T1 x, T2 y)
{
    return x + y;
}

void ShowTime(void *param); //显示时间
uint mgid = 0; // 总编号

TaskScheduler::TaskScheduler(cstring name)
{
    this->Name = name;

    mgid = 1;

    this->Running = false;
    this->Current = NULL;
    this->Count = 0;
}

// 创建任务，返回任务编号。dueTime首次调度时间ms，-1表示事件型任务，period调度间隔ms，-1表示仅处理一次
uint TaskScheduler::Add(Action func, void *param, int dueTime, int period, cstring name)
{
    Task *task = new Task();
    task->ID = mgid++;
    task->Callback = func;
    task->Param = param;
    task->Period = period;
    task->NextTime = Sys.Ms()+dueTime;
    task->Name = name;
    if (dueTime < 0)
    {
        task->Enable = false;
    }

    this->Count++;
    //_Tasks.Add(task);
    _TasksOld.Add(task);
    #if DEBUG
        debug_printf("%s::添加%2d %-11s", Name, task->ID, task->Name);
        if (dueTime >= 1000)
        {
            debug_printf("First = %3lds", dueTime / 1000);
        }
        else
        {
            debug_printf("First = %3ldms", dueTime);
        }

        // 输出长整型%ld，无符号长整型%llu
        if (period >= 1000)
        {
            debug_printf(" Period = %3lds\r\n", period / 1000);
        }
        else
        {
            debug_printf(" Period = %3ldms\r\n", period);
        }
    #endif 
    return task->ID;
}

void TaskScheduler::Remove(uint taskid)
{
    for (int i = 0; i < this->Count; i++)
    {
        Task *task = _TasksOld[i];
        if (task->ID == taskid)
        {
            _TasksOld.RemoveAt(i);
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
    #ifdef DEBUG
        if (!this->Running)
        {
            //this->Add(&TaskScheduler::ShowStatus,this,10000,30000,"ShowStatus");
        }
    #endif 

    if (Running)
    {
        return ;
    }

    #ifdef DEBUG
        Sys.AddTask(&TaskScheduler::ShowStatus, this, 1 *1000, 30 *1000, "任务状态");
    #else 
        //Sys.AddTask(ShowTime, this, 2 *1000, 20 *1000, "时间显示");
    #endif 

    debug_printf("%s::准备就绪 开始循环处理%d个任务！\r\n\r\n", Name, Count);

    Running = true;
    while (Running)
    {
        bool bb = false;
        Execute(0xFFFFFFFF, bb);
    }
    debug_printf("%s停止调度，共有%d个任务！\r\n", Name, Count);
}

void TaskScheduler::Stop()
{
    debug_printf("%s停止！\r\n", Name);
    Running = false;
}

// 执行一次循环。指定最大可用时间
void TaskScheduler::Execute(uint msMax, bool &cancel)
{
    UInt64 now = Sys.Ms();
    TimeCost tmcost;
    int mscurMax = now + msMax;

    Int64 v7;
    v7 =  - 1LL;
    #if 0	
        for (int i = 0; i < this->Count; i++)
        {
            Task *taskcur = this->_TasksOld[i];
            if (taskcur && taskcur->Callback && taskcur->Enable)
            {
                if (taskcur->CheckTime(mscurMax, msMax !=  - 1))
                {
                    if (taskcur->Execute(now))
                        this->Times++;
                    if (!msMax)
                        return ;
                    uint msEnd = Sys.Ms();
                    if (mscurMax < msEnd)
                        return ;
                }
                if (taskcur->Callback && taskcur->Enable)
                {
                    //				if(taskcur->Event)
                    //					 v7 = 0LL;
                    //				else if( (unsigned int)(*(_QWORD *)(taskcur + 24) >> 32) < (v7)+ (unsigned int)((unsigned int)*(_QWORD *)(taskcur + 24) < (unsigned int)v7) )
                    //				{
                    //					v7 = *(_QWORD *)(taskcur + 24);
                    //				}

                }
            }
        }
        this->Cost = tmcost.Elapsed();

    #else 
        now = Sys.Ms(); // 当前时间。减去系统启动时间，避免修改系统时间后导致调度停摆	

        UInt64 min = UInt64_Max; // 最小时间，这个时间就会有任务到来
        UInt64 end = Sys.Ms()+msMax;

        for (int i = 0; i < this->Count; i++)
        {
            Task *task = _TasksOld[i];
            if (task && task->Enable && task->NextTime <= now)
            {
                // 不能通过累加的方式计算下一次时间，因为可能系统时间被调整
                task->NextTime = now + task->Period;
                if (task->NextTime < min)
                {
                    min = task->NextTime;
                }
                bool cancel = false;
                this->Current = task;
                if (task->CheckTime(Sys.Ms(), cancel))
                    ;
                {
                    task->Execute(Sys.Ms());
                }
                this->Current = NULL;
            }
        }
        // 如果有最小时间，睡一会吧
        now = Sys.Ms(); // 当前时间
        if (min != UInt64_Max && min > now)
        {
            min -= now;
            Time.Sleep(min);
        }
    #endif 
}

Task *TaskScheduler::operator[](int taskid)
{
    for (int i = 0; i < this->Count; i++)
    {
        Task *task = _TasksOld[i];
        if (task && task->ID == taskid)
        {
            return task;
        }
    }
    return NULL;
}

// 查找任务 返回使用此函数的首个任务
Task *TaskScheduler::FindTask(Action func){}
// 跳过最近一次睡眠，马上开始下一轮循环
void TaskScheduler::SkipSleep(){}
// 使用外部缓冲区初始化任务列表，避免频繁的堆分配
void TaskScheduler::Set(Task *tasks, int count){}
uint TaskScheduler::ExecuteForWait(uint msMax, bool &cancel){}
//显示时间
void ShowTime(void *param)
{
    UInt64 curms = Time.Milliseconds;
    debug_printf("Time: %02lld:%02lld:%02lld.%03lld\n", curms / 3600000, curms / 60000 % 60, curms / 1000 % 60, curms % 1000);
}

// 显示状态
void TaskScheduler::ShowStatus()
{
    static UInt64 runCounts = 0;
    float RunTimes = 0;
    float RunTimesAvg = 0;
    Task *tsk;
    byte buf[1];

    runCounts++;
    UInt64 curms = Sys.Ms();

    //统计运行时间
    RunTimes = 0;
    RunTimesAvg = 0;
    for (int j = 0; j < this->Count; j++)
    {
        tsk = this->_TasksOld[j];
        RunTimes += tsk->Cost *tsk->Times;
        RunTimesAvg += tsk->Cost;
    }
    RunTimesAvg /= this->Count;
    //SRAM   0X20000000-0X3FFFFFFF 共512MB
    //SCODE  0X00000000-0X1FFFFFFF 共512MB
    debug_printf("\r\nTask::%s [%llu]负载 %0.2f%% ", "ShowStatus", runCounts, RunTimes / 10 / curms);
    if (RunTimesAvg >= 1000)
    {
        debug_printf("平均 %3.1fms ", RunTimesAvg / this->Count / 1000);
    }
    else
    {
        debug_printf("平均 %3.0fus ", RunTimesAvg / this->Count);
    }
    debug_printf("当前 1970-01-01 23 00:00");
    debug_printf("启动 %02lld:%02lld:%02lld.%03lld 堆 %u/%u\r\n", curms / 3600000, curms / 60000 % 60, curms / 1000 % 60, curms % 1000, &(buf[0]) - 0X20000000, 1024);

    for (int i = 0; i < this->Count; i++)
    {
        Task *task = this->_TasksOld[i];
        if (task)
        {
            task->ShowStatus();
        }
    }
}

// 查找任务 返回使用此函数的首个任务的ID
uint TaskScheduler::FindID(Action func)
{

}
