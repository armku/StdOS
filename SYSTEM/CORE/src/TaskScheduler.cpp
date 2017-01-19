#include "TaskScheduler.h"
#include "TTime.h"


TaskScheduler::TaskScheduler(string name)
{
    Name = name;

    _gid = 1;

    Running = false;
    Current = NULL;
    Count = 0;
}

TaskScheduler::~TaskScheduler()
{
    Current = NULL;
    #if 0
        _Tasks.DeleteAll().Clear();
    #endif 
}


    // 创建任务，返回任务编号。dueTime首次调度时间us，period调度间隔us，-1表示仅处理一次
    uint TaskScheduler::Add(Action func, void *param, ulong dueTime, long period)
    {
        Task *task = new Task(this);
        task->ID = _gid++;
        task->Callback = func;
        task->Param = param;
        task->Period = period;
        task->NextTime = Time.Current() + dueTime;

        Count++;
		#if 0
        _Tasks.Add(task);
		#endif       
            // 输出长整型%ld，无符号长整型%llu
            //debug_printf("%s添加任务%d 0x%08x FirstTime=%lluus Period=%ldus\r\n", Name, task->ID, func, dueTime, period);
            
        return task->ID;
    }

void TaskScheduler::Remove(uint taskid)
{
    int i =  - 1;
    #if 0
        while (_Tasks.MoveNext(i))
        {
            Task *task = _Tasks[i];
            if (task->ID == taskid)
            {
                _Tasks.RemoveAt(i);
                debug_printf("%s::删除任务%d 0x%08x\r\n", Name, task->ID, task->Callback);
                // 首先清零ID，避免delete的时候再次删除
                task->ID = 0;
                delete task;
                break;
            }
        }
    #endif 
}

void TaskScheduler::Start()
{
    if (Running)
        return ;
    
        //Add(ShowTime, NULL, 2000000, 2000000);
        //Add(ShowStatus, this, 10000000, 30000000);
   
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
    #if 0
        ulong now = Time.Current() - Sys.StartTime; // 当前时间。减去系统启动时间，避免修改系统时间后导致调度停摆
        ulong min = UInt64_Max; // 最小时间，这个时间就会有任务到来
        ulong end = Time.Current() + usMax;

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
                    min = task->NextTime;

                ulong now2 = Time.Current();
                task->SleepTime = 0;

                Current = task;
                task->Callback(task->Param);
                Current = NULL;

                // 累加任务执行次数和时间
                task->Times++;
                int cost = (int)(Time.Current() - now2);
                if (cost < 0)
                    cost =  - cost;
                //if(cost > 0)
                {
                    task->CpuTime += cost - task->SleepTime;
                    task->Cost = task->CpuTime / task->Times;
                }               
                    if (cost > 500000)
                        debug_printf("Task::Execute 任务 %d [%d] 执行时间过长 %dus 睡眠 %dus\r\n", task->ID, task->Times, cost, task->SleepTime);
                
                // 如果只是一次性任务，在这里清理
                if (task->Period < 0)
                    Remove(task->ID);
            }

            // 如果已经超出最大可用时间，则退出
            if (!usMax || Time.Current() > end)
                return ;
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
    #endif 
}

// 显示状态
void TaskScheduler::ShowStatus(void *param)
{
    TaskScheduler *ts = (TaskScheduler*)param;

    int i =  - 1;
    #if 0
        while (ts->_Tasks.MoveNext(i))
        {
            Task *task = ts->_Tasks[i];
            if (task)
                task->ShowStatus();
        }
    #endif 
}

Task *TaskScheduler::operator[](int taskid)
{
    int i =  - 1;
    #if 0
        while (_Tasks.MoveNext(i))
        {
            Task *task = _Tasks[i];
            if (task && task->ID == taskid)
                return task;
        }
    #endif 
    return NULL;
}

#if 0
    //外部注册函数
    // 任务
    #include "Task.h"
    // 任务类
    TaskScheduler *_Scheduler;

    // 创建任务，返回任务编号。priority优先级，dueTime首次调度时间us，period调度间隔us，-1表示仅处理一次
    uint TSys::AddTask(Action func, void *param, ulong dueTime, long period)
    {
        // 屏蔽中断，否则可能有线程冲突
        SmartIRQ irq;

        if (!_Scheduler)
            _Scheduler = new TaskScheduler("系统");

        return _Scheduler->Add(func, param, dueTime, period);
    }

    void TSys::RemoveTask(uint taskid)
    {
        assert_ptr(_Scheduler);

        _Scheduler->Remove(taskid);
    }

    void TSys::SetTask(uint taskid, bool enable)
    {
        Task *task = (*_Scheduler)[taskid];
        if (task)
            task->Enable = enable;
    }

    void TSys::Start()
    {
        if (!_Scheduler)
            _Scheduler = new TaskScheduler("系统");
            //AddTask(ShowTime, NULL, 2000000, 2000000);       
        if (OnStart)
            OnStart();
        else
            _Scheduler->Start();
    }

    void TSys::StartInternal()
    {
        _Scheduler->Start();
    }

    void TSys::Stop()
    {
        _Scheduler->Stop();
    }

    void TimeSleep(uint us)
    {
        // 在这段时间里面，去处理一下别的任务
        if (_Scheduler && (!us || us >= 1000))
        {
            // 记录当前正在执行任务
            Task *task = _Scheduler->Current;

            ulong start = Time.Current();
            // 1ms一般不够调度新任务，留给硬件等待
            ulong end = start + us - 1000;
            // 如果休眠时间足够长，允许多次调度其它任务
            int cost = 0;
            while (true)
            {
                ulong start2 = Time.Current();

                _Scheduler->Execute(us);

                ulong now = Time.Current();
                cost += (int)(now - start2);

                // us=0 表示释放一下CPU
                if (!us)
                    return ;

                if (now >= end)
                    break;
            }

            if (task)
            {
                _Scheduler->Current = task;
                task->SleepTime += cost;
            }

            cost = (int)(Time.Current() - start);
            if (cost > 0)
                return ;

            us -= cost;
        }
        if (us)
            Time.Sleep(us);
    }

    void TSys::Sleep(uint ms)
    {
        // 优先使用线程级睡眠
        if (OnSleep)
            OnSleep(ms);
        else
        {            
                if (ms > 1000)
                    debug_printf("Sys::Sleep 设计错误，睡眠%dms太长，超过1000ms建议使用多线程Thread！", ms);
            
            TimeSleep(ms *1000);
        }
    }

    void TSys::Delay(uint us)
    {
        // 如果延迟微秒数太大，则使用线程级睡眠
        if (OnSleep && us >= 2000)
            OnSleep((us + 500) / 1000);
        else
        {
           
                if (us > 1000000)
                    debug_printf("Sys::Sleep 设计错误，睡眠%dus太长，超过1000ms建议使用多线程Thread！", us);
           

            TimeSleep(us);
        }
    }

#endif
