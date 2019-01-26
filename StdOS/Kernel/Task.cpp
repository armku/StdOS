#include <stddef.h>
#include "Task.h"
#include "TTime.h"

TaskScheduler *_Scheduler;

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
bool Task::Execute(uint64_t now)
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

        this->Callback(this->Param);

        this->Times++;
		auto cms=costms.Elapsed();
        auto costMsCurrent = costms.Elapsed() - this->SleepTime;
		if(costMsCurrent<0)
			costMsCurrent=30;//异常设置为3us		
        if (this->MaxCost < costMsCurrent)
            this->MaxCost = costMsCurrent;
        this->Cost = (5 *this->Cost + 3 * costMsCurrent) / 8;
        this->CostMs = this->Cost / 1000;

        if (costMsCurrent > 500000)
            debug_printf("Task::Execute 任务:%s %d [%d] 执行时间过长 %dus 睡眠 %dus\n", this->Name, this->ID, this->Times, costMsCurrent, this->SleepTime);
        #if 0 //临时取消任务删除功能
            if (!this->Event && this->Period < 0)
                Task::Scheduler()->Remove(this->ID);
        #endif 
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
        this->NextTime = msNextTime + Sys.Ms();
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
        debug_printf("%3.2fms", this->Cost / 1000.0);
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
        debug_printf(" 最大 %3.2fms", this->MaxCost / 1000.0);
    }
    else
    {
        debug_printf(" 最大 %3dus", this->MaxCost);
    }
    if (this->Period >= 1000)
    {
        debug_printf(" 周期 %3lds", this->Period / 1000);
    }
    else
    {
        debug_printf(" 周期 %3ldms", this->Period);
    }
    debug_printf(" %s\n", this->Enable ? " " : "禁用");
}

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

bool Task::CheckTime(uint64_t end, bool isSleep)
{
    bool ret;

    if (this->Deepth < this->MaxDeepth)
    {
        auto mscur = Sys.Ms();
		if(!this->Enable)
		{
			ret=false;
		}
		else if(this->NextTime && (mscur<this->NextTime))
		{
			ret = false;
		}
		else if(end >= mscur)
		{
			if(isSleep)
			{
				if(this->Event || this->Times > 0)
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
				ret = 1;
			}
		}
		else
		{
			ret = 0;
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

    this->Period = 0; // 周期ms
    this->NextTime = 0; // 下一次执行时间ms

    this->Times = 0; // 执行次数
    this->SleepTime = 0; // 当前睡眠时间us
    this->Cost = 0; // 平均执行时间us
    this->CostMs = 0; // 平均执行时间ms
    this->MaxCost = 0; // 最大执行时间us

    this->Enable = true;
    this->Event = false;
    this->Deepth = 0;
    this->MaxDeepth = 1;
}
////////////////////////////////////////////////////////////////////////
TaskScheduler::TaskScheduler(const char* name)
{
    this->Name = name;

	this->Deepth = 0;
	this->MaxDeepth = 8;

    this->Running = false;
    this->Current = NULL;
    this->Count = 0;
}

// 创建任务，返回任务编号。dueTime首次调度时间ms，-1表示事件型任务，period调度间隔ms，-1表示仅处理一次
uint32_t TaskScheduler::Add(Action func, void *param, int dueTime, int period, const char* name)
{
    auto task = new Task();
    task->ID =this->Count;
    task->Callback = func;
    task->Param = param;
    task->Period = period;
    task->NextTime = Sys.Ms() + dueTime;
    task->Name = name;
    if (dueTime < 0)
    {
        task->Enable = false;
    }

    this->Count++;
    this->_Tasks.Add(task);
    #if DEBUG
        debug_printf("Task::Add%d %-11s", task->ID, task->Name);
        if (dueTime >= 1000)
        {
            debug_printf("First= %3lds", dueTime / 1000);
        }
        else
        {
            debug_printf("First=%3ldms", dueTime);
        }

        // 输出长整型%ld，无符号长整型%llu
        if (period >= 1000)
        {
            debug_printf(" Period=  %3lds\n", period / 1000);
        }
        else
        {
            debug_printf(" Period= %3ldms\n", period);
        }
    #endif 
    return task->ID;
}

void TaskScheduler::Remove(uint32_t taskid)
{
    for (int i = 0; i < this->Count; i++)
    {
        auto task = this->_Tasks[i];
        if (task->ID == taskid)
        {
            this->_Tasks.RemoveAt(i);
            debug_printf("%s::删除任务%d 0x%08x\n", Name, task->ID, (unsigned int)task->Callback);
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
            //this->Add(&TaskScheduler::ShowStatus,this,1000*30,1000*30,"ShowStatus");
        }
    #endif 

    if (Running)
    {
        return ;
    }

    #ifdef DEBUG
        //Sys.AddTask(&TaskScheduler::ShowStatus, this, 30 *1000, 30 *1000, "任务状态");
    #else 
        //Sys.AddTask(ShowTime, this, 2 *1000, 20 *1000, "时间显示");
    #endif 

    debug_printf("%s::准备就绪 开始循环处理%d个任务！\n\n", Name, Count);

    Running = true;
    while (Running)
    {
        auto bb = false;
        Execute(0xFFFFFFFF, bb);
    }
    debug_printf("%s停止调度，共有%d个任务！\n", Name, Count);
}

void TaskScheduler::Stop()
{
    debug_printf("%s停止！\r\n", Name);
    Running = false;
}

// 执行一次循环。指定最大可用时间
void TaskScheduler::Execute(uint32_t msMax, bool &cancel)
{
    auto mscur = Sys.Ms();
    TimeCost tmcost;

    auto min = UInt64_Max; // 最小时间，这个时间就会有任务到来

    auto mscurMax = mscur + msMax;
    for (int i = 0; i < this->Count; i++)
    {
        if (cancel)
            return ;
		this->Current = this->_Tasks[i];
        if (this->Current && this->Current->Callback && this->Current->Enable && this->Current->NextTime <= mscur)
        {            
			if(this->Current->CheckTime(mscurMax, false))
			{
				if(this->Current->Execute(mscur))
				{
					this->Times++;
				}
				if(!msMax)
					return;
				auto msend = Sys.Ms();
			}
			// 不能通过累加的方式计算下一次时间，因为可能系统时间被调整
			this->Current->NextTime = mscur + this->Current->Period;
			if (this->Current->NextTime < min)
			{
				min = this->Current->NextTime;
			}
			auto cancel = false;
			this->Current = this->Current;
			if (this->Current->CheckTime(Sys.Ms(), cancel))
			{
				this->Current->Execute(Sys.Ms());
			}
			this->Current = NULL;			
        }
    }

    this->Cost = tmcost.Elapsed();

    // 如果有最小时间，睡一会吧
    mscur = Sys.Ms(); // 当前时间
    if (min != UInt64_Max && min > mscur)
    {
        min -= mscur;
        Time.Sleep(min);
    }
}

Task *TaskScheduler::operator[](int taskid)
{
    for (int i = 0; i < this->Count; i++)
    {
        auto task = this->_Tasks[i];
        if (task && task->ID == taskid)
        {
            return task;
        }
    }
    return NULL;
}

// 跳过最近一次睡眠，马上开始下一轮循环
void TaskScheduler::SkipSleep(){}
// 使用外部缓冲区初始化任务列表，避免频繁的堆分配
void TaskScheduler::Set(Task *tasks, int count){}
uint32_t TaskScheduler::ExecuteForWait(uint32_t msMax, bool &cancel)
{
    uint32_t ret = 0;
	auto tskcur = Task::Scheduler()->Current;
    if (this->Deepth < MaxDeepth)
    {
        this->Deepth++;
		auto tskcur=this->Current;
		tskcur->Deepth++;
        auto msBegin = Sys.Ms();
        auto msEndMax = msBegin + msMax;
        auto msRemain = msMax;
        TimeCost tmcost;
        while (msRemain > 0 && !cancel)
        {
			this->Execute(msRemain, cancel);
			auto msc = Sys.Ms();
			if (msEndMax > msc)			
				msRemain = msEndMax - msc;			
			else
				msRemain = 0;
        }
		auto sleepus = tmcost.Elapsed();
        this->TotalSleep += sleepus/1000;
        auto msUsed = Sys.Ms() - msBegin;
		tskcur->SleepTime+=sleepus;
		tskcur->Deepth--;
        this->Deepth--;
        ret = msUsed;
    }
    else
    {
        ret = false;
    }
	Task::Scheduler()->Current = tskcur;
    return ret;
}

// 显示状态
void TaskScheduler::ShowStatus()
{
    static uint64_t runCounts = 0;
    float RunTimes = 0;
    float RunTimesAvg = 0;
    Task *tsk;
    uint8_t buf[1];

    runCounts++;
    auto curms = Sys.Ms();
    //统计运行时间
    RunTimes = 0;
    RunTimesAvg = 0;
    for (int j = 0; j < this->Count; j++)
    {
        tsk = this->_Tasks[j];
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
    debug_printf("启动 %02lld:%02lld:%02lld.%03lld 堆 %u/%u\n", curms / 3600000, curms / 60000 % 60, curms / 1000 % 60, curms % 1000, &(buf[0]) - 0X20000000, 1024);
    for (int i = 0; i < this->Count; i++)
    {
        auto task = this->_Tasks[i];
        if (task)
        {
            task->ShowStatus();
        }
    }
}
