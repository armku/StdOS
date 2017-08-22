#include "Task.h"
#include "TTime.h"

Task::Task()
{
	this->Init();
}
//Task::Task(const Task& task)
//{
//Task *v1; // r4@1

//  v1 = this;
//  if ( *((_DWORD *)this + 1) )
//    TaskScheduler::Remove(*(_QWORD *)this, *(_QWORD *)this >> 32);
//  return v1;	
//}
Task::~Task()
{
	
}
// 执行任务。返回是否正常执行。
bool Task::Execute(UInt64 now)
{
	return false;
	
//	Task *v3; // r4@1
//  unsigned __int64 v4; // kr08_8@1
//  _BYTE *v5; // r6@1
//  signed int result; // r0@2
//  int v7; // r9@7
//  int v8; // r5@7
//  char v9; // [sp+8h] [bp-30h]@7
//  char v10; // [sp+18h] [bp-20h]@1

//  v3 = this;
//  v4 = a3;
//  TraceStack::TraceStack((TraceStack *)&v10, *((const char **)this + 2));
//  v5 = (char *)v3 + 54;
//  if ( *((_BYTE *)v3 + 54) < (signed int)*((_BYTE *)v3 + 55) )
//  {
//    ++*v5;
//    if ( *((_BYTE *)v3 + 53) )
//      *((_BYTE *)v3 + 52) = 0;
//    else
//      *((_QWORD *)v3 + 3) = *((_DWORD *)v3 + 5) + v4;
//    TimeCost::TimeCost((TimeCost *)&v9);
//    *((_DWORD *)v3 + 9) = 0;
//    v7 = *(_DWORD *)(*(_DWORD *)v3 + 48);
//    *(_DWORD *)(*(_DWORD *)v3 + 48) = v3;
//    ((void (__fastcall *)(_DWORD))*(_QWORD *)((char *)v3 + 12))(*(_QWORD *)((char *)v3 + 12) >> 32);
//    *(_DWORD *)(*(_DWORD *)v3 + 48) = v7;
//    ++*((_DWORD *)v3 + 8);
//    v8 = TimeCost::Elapsed((TimeCost *)&v9) - *((_DWORD *)v3 + 9);
//    if ( *((_DWORD *)v3 + 12) < v8 )
//      *((_DWORD *)v3 + 12) = v8;
//    *((_DWORD *)v3 + 10) = (5 * *((_DWORD *)v3 + 10) + 3 * v8) >> 3;
//    *((_DWORD *)v3 + 11) = *((_DWORD *)v3 + 10) / 1000;
//    if ( v8 > 500000 )
//      SmartOS_printf(
//        "Task::Execute 浠诲姟 %d [%d] 鎵ц鏃堕棿杩囬暱 %dus 鐫＄湢 %dus\r\n",
//        *((_DWORD *)v3 + 1),
//        *((_DWORD *)v3 + 8),
//        v8,
//        *((_DWORD *)v3 + 9));
//    if ( !*((_BYTE *)v3 + 53) && *((_DWORD *)v3 + 5) < 0 )
//      TaskScheduler::Remove(*(_QWORD *)v3, *(_QWORD *)v3 >> 32);
//    --*v5;
//    TraceStack::~TraceStack((TraceStack *)&v10);
//    result = 1;
//  }
//  else
//  {
//    TraceStack::~TraceStack((TraceStack *)&v10);
//    result = 0;
//  }
//  return result;
}
// 设置任务的开关状态，同时运行指定任务最近一次调度的时间，0表示马上调度
void Task::Set(bool enable, int msNextTime)
{
//	__int64 v2; // kr08_8@1
//  int v3; // r4@1
//  __int64 v4; // r0@2
//  TaskScheduler *v5; // r0@4

//  v2 = this;
//  v3 = a2;
//  *(_BYTE *)(this + 52) = BYTE4(this);
//  if ( a2 >= 0 )
//  {
//    LODWORD(v4) = TSys::Ms((TSys *)&Sys);
//    this = v4 + v3;
//    *(_QWORD *)(v2 + 24) = this;
//  }
//  if ( HIDWORD(v2) )
//  {
//    v5 = (TaskScheduler *)Task::Scheduler((Task *)this);
//    LODWORD(this) = TaskScheduler::SkipSleep(v5);
//  }
//  return this;
}
// 显示状态
void Task::ShowStatus()
{
    debug_printf("Task::%-12s %2d", Name, ID);
    if (Times >= 1000000)
    {
        debug_printf("[%d]", Times);
    }
    else if (Times >= 100000)
    {
        debug_printf("[%d] ", Times);
    }
    else if (Times >= 10000)
    {
        debug_printf("[%d]  ", Times);
    }
    else if (Times >= 1000)
    {
        debug_printf("[%d]   ", Times);
    }
    else if (Times >= 100)
    {
        debug_printf("[%d]    ", Times);
    }
    else if (Times >= 10)
    {
        debug_printf("[%d]     ", Times);
    }
    else
    {
        debug_printf("[%d]      ", Times);
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
        debug_printf("%3ums", Cost / 1000);
    }
    else
    {
        debug_printf("%3uus", Cost);
    }
    if (CpuTime >= 1000000)
    {
        debug_printf(" 最大 %3ds ", CpuTime / 1000000);
    }
    else if (CpuTime >= 1000)
    {
        debug_printf(" 最大 %3dms", CpuTime / 1000);
    }
    else
    {
        debug_printf(" 最大 %3dus", CpuTime);
    }
    if (this->Period >= 1000000)
    {
        debug_printf(" 周期 %3lds ", Period / 1000000);
    }
    else if (this->Period >= 1000)
    {
        debug_printf(" 周期 %3ldms", Period / 1000);
    }
    else
    {
        debug_printf(" 周期 %3ldus", Period);
    }
    debug_printf(" %s\r\n", this->Enable ? " " : "禁用");
	
	
//	Task *v1; // r4@1

//  v1 = this;
//  SmartOS_printf(
//    (const char *)sub_810,
//    (_DWORD)(*(_QWORD *)((char *)v1 + 4) >> 32),
//    (unsigned int)*(_QWORD *)((char *)v1 + 4),
//    *((_DWORD *)v1 + 8),
//    *((_DWORD *)this + 10));
//  if ( *((_DWORD *)v1 + 10) < 1000 )
//    SmartOS_printf((const char *)&dword_830);
//  SmartOS_printf((const char *)dword_834, *((_DWORD *)v1 + 12));
//  if ( *((_DWORD *)v1 + 12) < 1000 )
//    SmartOS_printf((const char *)&dword_830);
//  SmartOS_printf((const char *)&sub_844);
//  if ( *((_DWORD *)v1 + 5) < 1000 )
//    SmartOS_printf("%dms", *((_DWORD *)v1 + 5));
//  else
//    SmartOS_printf((const char *)&dword_850, *((_DWORD *)v1 + 5) / 1000);
//  if ( !*((_BYTE *)v1 + 52) )
//    SmartOS_printf((const char *)&sub_85C);
//  return SmartOS_printf((const char *)&dword_864);
}
// 全局任务调度器
TaskScheduler* Task::Scheduler()
{
	return NULL;
//	 if ( !(ZGV_ZZN4Task9SchedulerEvE3_sc & 1) && _cxa_guard_acquire(&ZGV_ZZN4Task9SchedulerEvE3_sc) )
//  {
//    TaskScheduler::TaskScheduler((int)&Task::Scheduler(void)::_sc, (int)"Task");
//    _aeabi_atexit();
//    _cxa_guard_release(&ZGV_ZZN4Task9SchedulerEvE3_sc);
//  }
//  return &Task::Scheduler(void)::_sc;
}
Task* Task::Get(int taskid)
{
	return NULL;
	
//	Task *v2; // r4@1
//  IList *v3; // r0@1

//  v2 = this;
//  v3 = (IList *)Task::Scheduler(this);
//  return TaskScheduler::operator[](v3, (int)v2);
}
Task t;
Task& Task::Current()
{	
	return t;
	
	//return *(_DWORD *)(Task::Scheduler(this) + 48);
}
bool Task::CheckTime(UInt64 end, bool isSleep)
{
	return false;
//	Task *v4; // r4@1
//  unsigned __int64 v5; // kr00_8@1
//  signed int result; // r0@2
//  unsigned int v7; // r1@4
//  unsigned int v8; // r5@4

//  v4 = this;
//  v5 = a3;
//  if ( *((_BYTE *)this + 54) < (signed int)*((_BYTE *)this + 55) )
//  {
//    v8 = TSys::Ms((TSys *)&Sys);
//    if ( *((_QWORD *)v4 + 3)
//      && v7 < (unsigned int)((*((_QWORD *)v4 + 3) >> 32) + (v8 < (unsigned int)*((_QWORD *)v4 + 3))) )
//    {
//      result = 0;
//    }
//    else if ( HIDWORD(v5) >= (unsigned int)(((*((_DWORD *)v4 + 11) + __PAIR__(v7, v8)) >> 32)
//                                          + ((unsigned int)v5 < *((_DWORD *)v4 + 11) + v8)) )
//    {
//      if ( a4 )
//      {
//        if ( *((_BYTE *)v4 + 53) || *((_DWORD *)v4 + 8) > 0 )
//          result = 1;
//        else
//          result = (__PAIR__(v7, v8) + 500) >> 32 < HIDWORD(v5) + (unsigned int)(v8 + 500 < (unsigned int)v5);
//      }
//      else
//      {
//        result = 1;
//      }
//    }
//    else
//    {
//      result = 0;
//    }
//  }
//  else
//  {
//    result = 0;
//  }
//  return result;
}
void Task::Init()
{
//	*(_DWORD *)result = 0;
//  *(_DWORD *)(result + 4) = 0;
//  *(_DWORD *)(result + 8) = 0;
//  *(_DWORD *)(result + 12) = 0;
//  *(_DWORD *)(result + 16) = 0;
//  *(_DWORD *)(result + 20) = 0;
//  *(_DWORD *)(result + 24) = 0;
//  *(_DWORD *)(result + 28) = 0;
//  *(_DWORD *)(result + 32) = 0;
//  *(_DWORD *)(result + 36) = 0;
//  *(_DWORD *)(result + 40) = 0;
//  *(_DWORD *)(result + 44) = 0;
//  *(_DWORD *)(result + 48) = 0;
//  *(_BYTE *)(result + 52) = 1;
//  *(_BYTE *)(result + 53) = 0;
//  *(_BYTE *)(result + 54) = 0;
//  *(_BYTE *)(result + 55) = 1;
//  return result;
}
Task::Task(TaskScheduler *scheduler)
{
    Host = scheduler;

    Times = 0;
    CpuTime = 0;
    SleepTime = 0;
    Cost = 0;
    Enable = true;
}
bool Task::operator == (Task &tsk)
{
    if (tsk.ID == this->ID)
    {
        return true;
    }
    return false;
}
































////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


template <typename T1, typename T2> T1 sum(T1 x, T2 y)
{
    return x + y;
}
void ShowTime(void * param);//显示时间
void ShowStatus(void *param); // 显示状态
uint mgid; // 总编号


/*Task::~Task()
{
if(ID) _Scheduler->Remove(ID);
}*/


TaskScheduler::TaskScheduler(cstring name)
{
    this->Name = name;

    mgid = 1;

    this->Running = false;
    this->Current = NULL;
    this->Count = 0;
	
	
	
//	int v2; // r6@1
//  _DWORD *v3; // r0@1
//  int v4; // r4@1
//  __int64 v5; // r0@1

//  v2 = a2;
//  v3 = (_DWORD *)IList::IList();
//  *v3 = &off_BD8;
//  v4 = (int)(v3 + 9);
//  *(_DWORD *)(v4 + 4) = v2;
//  *(_BYTE *)(v4 + 16) = 0;
//  *(_DWORD *)(v4 + 12) = 0;
//  *(_DWORD *)(v4 + 8) = 0;
//  *(_BYTE *)(v4 + 18) = 0;
//  *(_BYTE *)(v4 + 19) = 8;
//  *(_DWORD *)(v4 + 20) = 0;
//  *(_DWORD *)(v4 + 24) = 0;
//  *(_DWORD *)(v4 + 28) = 0;
//  *(_DWORD *)(v4 + 32) = 0;
//  LODWORD(v5) = TSys::Ms((TSys *)&Sys);
//  *(_QWORD *)(v4 + 36) = v5;
//  *(_BYTE *)v4 = 0;
//  *(_DWORD *)(v4 + 44) = 0;
//  *(_DWORD *)(v4 + 48) = 0;
//  return v4 - 36;
}
//TaskScheduler::~TaskScheduler()
//{
////  return List<Task *>::~List(this);
//}

// 创建任务，返回任务编号。dueTime首次调度时间ms，-1表示事件型任务，period调度间隔ms，-1表示仅处理一次
uint TaskScheduler::Add(Action func, void* param, int dueTime, int period, cstring name)
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
    task->NextTime = Time.Current()*1000 + dueTime;
    task->Name = name;
    if (dueTime < 0)
    {
        task->Enable = false;
    }

    this->Count++;
    _TasksOld.Add(task);
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
		
		
		//	TaskScheduler *v6; // r6@1
//  void (__cdecl *v7)(void *); // r7@1
//  void *v8; // r9@1
//  int v9; // r5@1
//  int v10; // r4@1
//  int i; // r8@1
//  int v12; // r0@2
//  Task *v13; // r0@9
//  __int64 v14; // r0@12

//  v6 = this;
//  v7 = a2;
//  v8 = a3;
//  v9 = a4;
//  v10 = 0;
//  for ( i = 0; !v10 && IList::Count(v6) > i; ++i )
//  {
//    v12 = List<Task *>::operator[](v6, i);
//    if ( !*(_DWORD *)(*(_DWORD *)v12 + 4) )
//      v10 = *(_DWORD *)v12;
//  }
//  if ( v10 )
//  {
//    Task::Init(v10);
//  }
//  else
//  {
//    v13 = (Task *)operator new(0x38u);
//    v10 = Task::Task(v13);
//    List<Task *>::Add(v6, (void *)v10);
//  }
//  *(_DWORD *)v10 = v6;
//  *(_DWORD *)(v10 + 4) = TaskScheduler::Add(void (*)(void *),void *,int,int,char const*)::_gid++;
//  *(_DWORD *)(v10 + 8) = a6;
//  *(_DWORD *)(v10 + 12) = v7;
//  *(_DWORD *)(v10 + 16) = v8;
//  *(_DWORD *)(v10 + 20) = a5;
//  if ( v9 >= 0 )
//  {
//    LODWORD(v14) = TSys::Ms((TSys *)&Sys);
//    *(_QWORD *)(v10 + 24) = v14 + v9;
//  }
//  else
//  {
//    *(_DWORD *)(v10 + 24) = 0;
//    *(_DWORD *)(v10 + 28) = 0;
//    *(_BYTE *)(v10 + 52) = 0;
//    *(_BYTE *)(v10 + 53) = 1;
//  }
//  ++*((_DWORD *)v6 + 11);
//  SmartOS_printf(
//    "%s::Add%d %s First=%dms Period=%dms 0x%p\r\n",
//    *((_DWORD *)v6 + 10),
//    *(_DWORD *)(v10 + 4),
//    a6,
//    v9,
//    a5,
//    v7);
//  return *(_DWORD *)(v10 + 4);
}

//设置任务执行、就绪状态
//void TaskScheduler::SetTask(uint taskid, bool onoff, long delaytime)
//{
//    if (taskid > this->Count)
//    {
//        //超范围，返回
//        return ;
//    }
//    this->_Tasks[taskid]->Enable = onoff;
//    if (onoff)
//    {
//        this->_Tasks[taskid]->NextTime = Time.Current() + delaytime * 1000;
//    }
//}

void TaskScheduler::Remove(uint taskid)
{
    int i =  - 1;

    while (_TasksOld.MoveNext(i))
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
	
	
//	int v2; // r7@1
//  unsigned int v3; // r5@1
//  int i; // r6@3
//  int v5; // r0@4
//  int v6; // r4@4

//  v2 = result;
//  v3 = a2;
//  if ( a2 )
//  {
//    for ( i = 0; ; ++i )
//    {
//      result = IList::Count((IList *)v2);
//      if ( result <= i )
//        break;
//      v5 = List<Task *>::operator[](v2, i);
//      v6 = *(_DWORD *)v5;
//      if ( *(_DWORD *)(*(_DWORD *)v5 + 4) == v3 )
//      {
//        SmartOS_printf("%s::Remove%d %s 0x%p\r\n", *(_DWORD *)(v2 + 40), *(_QWORD *)(v6 + 4), *(_DWORD *)(v6 + 12));
//        result = 0;
//        *(_DWORD *)(v6 + 4) = 0;
//        return result;
//      }
//    }
//  }
//  return result;
}

void TaskScheduler::Start()
{
    if (Running)
    {
        return ;
    }

    #ifdef DEBUG
        //Add(ShowStatus, this, 1 *1000, 30 *1000, "任务状态");
    #else 
        Add(ShowTime, this, 2 *1000, 20 *1000, "时间显示");
    #endif 

    debug_printf("%s::准备就绪 开始循环处理%d个任务！\r\n\r\n", Name, Count);

    Running = true;
    while (Running)
    {
		bool bb=false;
        Execute(0xFFFFFFFF,bb);
    }
    debug_printf("%s停止调度，共有%d个任务！\r\n", Name, Count);
	
	
	
//	TaskScheduler *v1; // r4@1
//  int result; // r0@1
//  int v3; // r4@3
//  int i; // r4@3
//  int v5; // r4@6
//  bool v6[4]; // [sp+10h] [bp-18h]@3

//  v1 = this;
//  result = *((_BYTE *)this + 52);
//  if ( !result )
//  {
//    TaskScheduler::Add(
//      v1,
//      (void (__cdecl *)(void *))TaskScheduler::ShowStatus,
//      (void *)v1,
//      10000,
//      30000,
//      (const char *)dword_404);
//    v3 = (int)v1 + 40;
//    SmartOS_printf((const char *)&unk_414, *(_QWORD *)v3);
//    *(_DWORD *)v6 = 0;
//    *(_BYTE *)(v3 + 12) = 1;
//    ++*(_BYTE *)(v3 + 14);
//    for ( i = v3 - 40; *(_BYTE *)(i + 52); TaskScheduler::Execute((TaskScheduler *)i, 0xFFFFFFFF, v6) )
//      ;
//    v5 = i + 40;
//    --*(_BYTE *)(v5 + 14);
//    *(_BYTE *)(v5 + 12) = 0;
//    result = SmartOS_printf((const char *)dword_44C, *(_DWORD *)v5, *(_DWORD *)(v5 + 4));
//  }
//  return result;
}

void TaskScheduler::Stop()
{
    debug_printf("%s停止！\r\n", Name);
    Running = false;
	
//	TaskScheduler *v1; // r4@1
//  int result; // r0@1

//  v1 = this;
//  SmartOS_printf((const char *)sub_370, *((_DWORD *)this + 10));
//  result = 0;
//  *((_BYTE *)v1 + 52) = 0;
//  return result;
}

// 执行一次循环。指定最大可用时间
void TaskScheduler::Execute(uint msMax, bool& cancel)
{
    UInt64 now;
    now = Time.Current()*1000; // 当前时间。减去系统启动时间，避免修改系统时间后导致调度停摆	

    UInt64 min = UInt64_Max; // 最小时间，这个时间就会有任务到来
    UInt64 end = Time.Current()*1000 + msMax;

    // 需要跳过当前正在执行任务的调度
    //Task* _cur = Current;

    int i =  - 1;
    while (_TasksOld.MoveNext(i))
    {
        Task *task = _TasksOld[i];
        //if(task && task != _cur && task->Enable && task->NextTime <= now)
        if (task && task->Enable && task->NextTime <= now)
        {
            // 不能通过累加的方式计算下一次时间，因为可能系统时间被调整
            task->NextTime = now + task->Period;
            if (task->NextTime < min)
            {
                min = task->NextTime;
            }

            UInt64 now2 = Time.Current()*1000;
            task->SleepTime = 0;

            this->Current = task;
            task->Callback(task->Param);
            this->Current = NULL;

            // 累加任务执行次数和时间
            task->Times++;
            int cost = (int)(Time.Current()*1000 - now2);
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
        if (!msMax || (Time.Current()*1000) > end)
        {
            return ;
        }
    }
    // 如果有最小时间，睡一会吧
    now = Time.Current()*1000; // 当前时间
    if (min != UInt64_Max && min > now)
    {
        min -= now;
        //debug_printf("TaskScheduler::Execute 等待下一次任务调度 %uus\r\n", (uint)min);           
        //// 最大只允许睡眠1秒，避免Sys.Delay出现设计错误，同时也更人性化
        //if(min > 1000000) min = 1000000;
        //Sys.Delay(min);
        Time.Delay(min);
    }
	
	
	
	
//	TaskScheduler *v3; // r10@1
//  unsigned __int64 v4; // r0@1
//  unsigned __int64 v5; // r6@1
//  unsigned __int64 v6; // r8@1
//  signed __int64 v7; // r4@1
//  int v9; // r11@5
//  unsigned int v10; // r0@13
//  int v11; // r1@13
//  unsigned int v12; // r1@25
//  __int64 v13; // r6@25
//  int v14; // r4@28
//  __int64 v15; // r0@31 OVERLAPPED
//  int v16; // r2@31
//  int i; // [sp+4h] [bp-4Ch]@1
//  char v18; // [sp+8h] [bp-48h]@1
//  char v19; // [sp+18h] [bp-38h]@1
//  TaskScheduler *v20; // [sp+20h] [bp-30h]@1
//  unsigned int v21; // [sp+24h] [bp-2Ch]@1
//  bool *v22; // [sp+28h] [bp-28h]@1

//  v20 = this;
//  v21 = a2;
//  v22 = a3;
//  v3 = this;
//  TraceStack::TraceStack((TraceStack *)&v19, "Task::Execute");
//  LODWORD(v4) = TSys::Ms((TSys *)&Sys);
//  v5 = v4;
//  v6 = v4 + v21;
//  v7 = -1LL;
//  TimeCost::TimeCost((TimeCost *)&v18);
//  for ( i = 0; IList::Count(v3) > i; ++i )
//  {
//    if ( *v22 )
//      return TraceStack::~TraceStack((TraceStack *)&v19);
//    v9 = *(_DWORD *)List<Task *>::operator[](v3, i);
//    if ( v9 && *(_DWORD *)(v9 + 4) && *(_BYTE *)(v9 + 52) )
//    {
//      if ( Task::CheckTime((Task *)v9, v6, v21 != -1) )
//      {
//        if ( Task::Execute((Task *)v9, v5) )
//          ++*((_DWORD *)v3 + 14);
//        if ( !v21 )
//          return TraceStack::~TraceStack((TraceStack *)&v19);
//        v10 = TSys::Ms((TSys *)&Sys);
//        if ( HIDWORD(v6) < v11 + (unsigned int)((unsigned int)v6 < v10) )
//          return TraceStack::~TraceStack((TraceStack *)&v19);
//      }
//      if ( *(_DWORD *)(v9 + 4) && *(_BYTE *)(v9 + 52) )
//      {
//        if ( *(_BYTE *)(v9 + 53) )
//        {
//          v7 = 0LL;
//        }
//        else if ( (unsigned int)(*(_QWORD *)(v9 + 24) >> 32) < HIDWORD(v7)
//                                                             + (unsigned int)((unsigned int)*(_QWORD *)(v9 + 24) < (unsigned int)v7) )
//        {
//          v7 = *(_QWORD *)(v9 + 24);
//        }
//      }
//    }
//  }
//  *((_DWORD *)v3 + 15) += TimeCost::Elapsed((TimeCost *)&v18);
//  if ( HIDWORD(v6) < HIDWORD(v7) + (unsigned int)((unsigned int)v6 < (unsigned int)v7) )
//    v7 = v6;
//  LODWORD(v13) = TSys::Ms((TSys *)&Sys);
//  HIDWORD(v13) = v12;
//  if ( !*((_BYTE *)v3 + 36) && ~v7 && v12 < HIDWORD(v7) + (unsigned int)((unsigned int)v13 < (unsigned int)v7) )
//  {
//    v14 = v7 - v13;
//    *((_BYTE *)v3 + 53) = 1;
//    if ( *((_DWORD *)v3 + 20) )
//      (*((void (__fastcall **)(_DWORD))v3 + 20))(v14);
//    else
//      TTime::Sleep((TTime *)&Time, v14, (bool *)v3 + 53);
//    *((_BYTE *)v3 + 53) = 0;
//    LODWORD(v15) = TSys::Ms((TSys *)&Sys);
//    v16 = (unsigned __int64)(*((_QWORD *)v3 + 8) + v15 - v13) >> 32;
//    HIDWORD(v15) = *((_DWORD *)v3 + 16) + v15 - v13;
//    *((_QWORD *)v3 + 8) = *(__int64 *)((char *)&v15 + 4);
//  }
//  *((_BYTE *)v3 + 36) = 0;
//  return TraceStack::~TraceStack((TraceStack *)&v19);
}

//显示时间
void ShowTime(void *param)
{
    UInt64 curms = Time.Milliseconds;
    debug_printf("Time: %02lld:%02lld:%02lld.%03lld\n", curms / 3600000, curms / 60000 % 60, curms / 1000 % 60, curms % 1000);
}

// 显示状态
void ShowStatus(void *param)
{
	#if 0
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
	#endif
}

Task *TaskScheduler::operator[](int taskid)
{
    int i =  - 1;

    while (_TasksOld.MoveNext(i))
    {
        Task *task = _TasksOld[i];
        if (task && task->ID == taskid)
        {
            return task;
        }
    }
    return NULL;
	
	
//	IList *v2; // r7@1
//  int v3; // r5@1
//  int i; // r4@1
//  int v5; // r0@2

//  v2 = this;
//  v3 = a2;
//  for ( i = 0; IList::Count(v2) > i; ++i )
//  {
//    v5 = List<Task *>::operator[](v2, i);
//    if ( *(_DWORD *)(*(_DWORD *)v5 + 4) == v3 )
//      return *(_DWORD *)v5;
//  }
//  return 0;
}

// 查找任务 返回使用此函数的首个任务
Task* TaskScheduler::FindTask(Action func)
{
//	TaskScheduler *v2; // r8@1
//  void (__cdecl *v3)(void *); // r4@1
//  int result; // r0@2
//  int v5; // r6@4
//  int i; // r5@4
//  int v7; // r0@5

//  v2 = this;
//  v3 = a2;
//  if ( a2 )
//  {
//    v5 = 0;
//    for ( i = 0; IList::Count(v2) > i; ++i )
//    {
//      v7 = List<Task *>::operator[](v2, i);
//      if ( *(void (__cdecl **)(void *))(*(_DWORD *)v7 + 12) == v3 )
//      {
//        v5 = *(_DWORD *)v7;
//        break;
//      }
//    }
//    result = v5;
//  }
//  else
//  {
//    result = 0;
//  }
//  return result;
}
// 跳过最近一次睡眠，马上开始下一轮循环
void TaskScheduler::SkipSleep()
{
//	TaskScheduler *v1; // r4@1
//  int result; // r0@1

//  v1 = this;
//  *((_BYTE *)this + 36) = 1;
//  *((_BYTE *)this + 53) = 0;
//  result = *((_DWORD *)this + 21);
//  if ( result )
//    result = (*((int (**)(void))v1 + 21))();
//  return result;
}
// 使用外部缓冲区初始化任务列表，避免频繁的堆分配
void TaskScheduler::Set(Task* tasks, int count)
{
//	IList *v3; // r7@1
//  Task *v4; // r5@1
//  int v5; // r6@1
//  int i; // r4@1

//  v3 = (IList *)result;
//  v4 = a2;
//  v5 = a3;
//  for ( i = 0; i < v5; ++i )
//  {
//    *((_DWORD *)v4 + 14 * i + 1) = 0;
//    result = List<Task *>::Add(v3, (char *)v4 + 56 * i);
//  }
//  return result;
}
uint TaskScheduler::ExecuteForWait(uint msMax, bool& cancel)
{
//	TaskScheduler *v3; // r5@1
//  unsigned int v4; // r6@1
//  char *v5; // r9@1
//  int result; // r0@2
//  int v7; // r10@4
//  int v8; // r7@4
//  signed int v9; // r4@4
//  int v10; // r0@5
//  int v11; // r11@8
//  int v12; // [sp+0h] [bp-48h]@4
//  unsigned int v13; // [sp+10h] [bp-38h]@4
//  TaskScheduler *v14; // [sp+1Ch] [bp-2Ch]@1
//  unsigned int v15; // [sp+20h] [bp-28h]@1
//  bool *v16; // [sp+24h] [bp-24h]@1

//  v14 = this;
//  v15 = a2;
//  v16 = a3;
//  v3 = this;
//  v4 = a2;
//  v5 = (char *)this + 54;
//  if ( *((_BYTE *)this + 54) < (signed int)*((_BYTE *)this + 55) )
//  {
//    ++*v5;
//    v7 = *((_DWORD *)this + 12);
//    v8 = TSys::Ms((TSys *)&Sys);
//    v13 = v8 + v4;
//    v9 = v4;
//    TimeCost::TimeCost((TimeCost *)&v12);
//    while ( v9 > 0 && !*v16 )
//    {
//      TaskScheduler::Execute(v3, v9, v16);
//      v10 = TSys::Ms((TSys *)&Sys);
//      v9 = v13 - v10;
//    }
//    *((_DWORD *)v3 + 12) = v7;
//    v11 = TSys::Ms((TSys *)&Sys) - v8;
//    if ( v7 )
//      *(_DWORD *)(v7 + 36) += TimeCost::Elapsed((TimeCost *)&v12);
//    --*v5;
//    result = v11;
//  }
//  else
//  {
//    result = 0;
//  }
//  return result;
}

// 显示状态
void TaskScheduler::ShowStatus()
{
//	TaskScheduler *v1; // r4@1
//  __int64 v2; // r0@1
//  unsigned int v3; // r5@1 OVERLAPPED
//  unsigned int v4; // r6@1 OVERLAPPED
//  signed int v5; // r7@1
//  signed int v6; // r11@1
//  unsigned __int64 v7; // ST08_8@1
//  int v8; // ST04_4@1
//  unsigned __int64 v9; // r0@1
//  signed int v10; // ST14_4@1
//  int v11; // r12@1
//  int v12; // r0@2
//  TimeSpan *v13; // r0@4
//  Heap *v14; // ST10_4@4
//  int v15; // r9@4
//  int v16; // r0@4
//  signed int v17; // r8@4
//  int i; // r9@6
//  int v19; // r10@7
//  int result; // r0@11
//  char v21; // [sp+18h] [bp-38h]@4
//  char v22; // [sp+20h] [bp-30h]@4

//  v1 = this;
//  LODWORD(v2) = TSys::Ms((TSys *)&Sys);
//  v4 = HIDWORD(v2);
//  v3 = v2;
//  v5 = *((_DWORD *)v1 + 14);
//  v6 = *((_DWORD *)v1 + 15);
//  v7 = v2 - *((_QWORD *)v1 + 9);
//  v8 = (unsigned __int64)(10000LL * *((_QWORD *)v1 + 8)) >> 32;
//  v9 = 10000LL * *((_QWORD *)v1 + 8);
//  v10 = 10000 - v9 / v7;
//  *((_DWORD *)v1 + 14) = 0;
//  *((_DWORD *)v1 + 15) = 0;
//  *((_DWORD *)v1 + 16) = 0;
//  *((_DWORD *)v1 + 17) = 0;
//  *((_QWORD *)v1 + 9) = *(_QWORD *)&v3;
//  SmartOS_printf("Task::ShowStatus [%d]", v5, v9 % v7, v11, v8, v7);
//  SmartOS_printf((const char *)dword_14C, v10 / 100, v10 % 100);
//  if ( v5 )
//    v12 = v6 / v5;
//  else
//    v12 = 0;
//  SmartOS_printf((const char *)dword_15C, v12);
//  DateTime::Now((DateTime *)&v22);
//  DateTime::Show((DateTime *)&v22, 0);
//  SmartOS_printf((const char *)dword_174);
//  v13 = (TimeSpan *)TimeSpan::TimeSpan((TimeSpan *)&v21, __PAIR__(v4, v3));
//  TimeSpan::Show(v13, 0);
//  v14 = (Heap *)Heap::Current;
//  v15 = Heap::Count((Heap *)Heap::Current);
//  v16 = Heap::Used(v14);
//  SmartOS_printf((const char *)&unk_184, v16, v15);
//  v17 = v6 / 1000;
//  if ( v6 / 1000 > 10 )
//    v17 = 10;
//  for ( i = 0; ; ++i )
//  {
//    result = IList::Count(v1);
//    if ( result <= i )
//      break;
//    v19 = *(_DWORD *)List<Task *>::operator[](v1, i);
//    if ( v19 )
//    {
//      if ( *(_DWORD *)(v19 + 4) )
//      {
//        Task::ShowStatus((Task *)v19);
//        TSys::Sleep((TSys *)&Sys, v17);
//      }
//    }
//  }
//  return result;
}	

// 查找任务 返回使用此函数的首个任务的ID
uint TaskScheduler::FindID(Action func)
{
//	TaskScheduler *v2; // r8@1
//  void (__cdecl *v3)(void *); // r4@1
//  int result; // r0@2
//  int v5; // r6@4
//  int i; // r5@4
//  int v7; // r0@5

//  v2 = this;
//  v3 = a2;
//  if ( a2 )
//  {
//    v5 = 0;
//    for ( i = 0; IList::Count(v2) > i; ++i )
//    {
//      v7 = List<Task *>::operator[](v2, i);
//      if ( *(void (__cdecl **)(void *))(*(_DWORD *)v7 + 12) == v3 )
//      {
//        v5 = *(_DWORD *)(*(_DWORD *)v7 + 4);
//        break;
//      }
//    }
//    result = v5;
//  }
//  else
//  {
//    result = 0;
//  }
//  return result;
}

