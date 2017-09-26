#include "Task.h"
#include "TTime.h"

Task::Task()
{
	this->Init();
}
Task::~Task()
{
	if(this->ID)
	this->Host->Remove(this->ID);
}
// ִ�����񡣷����Ƿ�����ִ�С�
bool Task::Execute(UInt64 now)
{
	if(this->Deepth<this->MaxDeepth)
	{
		this->Deepth++;		
		if(this->Event)
			this->Enable=false;
		else
			this->NextTime=this->Period+now;
		
		TimeCost costms;		
		this->SleepTime=0;
		
		this->Callback(this->Param);
		
		
		this->Times++;
		int costMsCurrent=costms.Elapsed()-this->SleepTime;
		if(this->MaxCost<costMsCurrent)
			this->MaxCost=costMsCurrent;
		this->Cost= (5*this->Cost+3*costMsCurrent)>>3;
		this->CostMs=this->Cost/1000;
		
		if ( costMsCurrent > 500000 )
			debug_printf("Task::Execute ����:%s %d [%d] ִ��ʱ����� %dus ˯�� %dus\r\n", this->Name, this->ID, this->Times,costMsCurrent, this->SleepTime);
		if(!this->Event&&this->Period<0)
			Task::Scheduler()->Remove(this->ID);
		this->Deepth--;
		return true;
	}
	else
	{
		return false;
	}		
}
// ��������Ŀ���״̬��ͬʱ����ָ���������һ�ε��ȵ�ʱ�䣬0��ʾ���ϵ���
void Task::Set(bool enable, int msNextTime)
{
	if ( msNextTime >= 0 )
	{
		this->NextTime=msNextTime+Sys.Ms()*1000;
	}
	this->Enable=enable;
	if(enable)
	{
		Task::Scheduler()->SkipSleep();
	}
}
// ��ʾ״̬
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
        debug_printf(" ƽ��[%4.1f%%] ", cpuPercent);
    }
    else
    {
        debug_printf(" ƽ��[%0.2f%%] ", cpuPercent);
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
        debug_printf(" ��� %3ds ", CpuTime / 1000000);
    }
    else if (CpuTime >= 1000)
    {
        debug_printf(" ��� %3dms", CpuTime / 1000);
    }
    else
    {
        debug_printf(" ��� %3dus", CpuTime);
    }
    if (this->Period >= 1000000)
    {
        debug_printf(" ���� %3lds ", Period / 1000000);
    }
    else if (this->Period >= 1000)
    {
        debug_printf(" ���� %3ldms", Period / 1000);
    }
    else
    {
        debug_printf(" ���� %3ldus", Period);
    }
    debug_printf(" %s\r\n", this->Enable ? " " : "����");
}
TaskScheduler *_Scheduler;
// ȫ�����������
TaskScheduler* Task::Scheduler()
{
	if (!_Scheduler)
    {
        _Scheduler = new TaskScheduler("Sys");
    }
	return _Scheduler;
}
Task* Task::Get(int taskid)
{
	return (*Task::Scheduler())[taskid];
}

Task& Task::Current()
{	
	return *Task::Scheduler()->Current;
	
}
bool Task::CheckTime(UInt64 end, bool isSleep)
{
	bool ret;
	if(this->Deepth<this->MaxDeepth)
	{
		uint mscur=Sys.Ms();
		if(this->Enable)
		{
			ret=false;
		}
		else if(end>=this->NextTime)
		{
			if(isSleep)
			{
				if(this->Event||(this->Period>=0))
				{
					ret=true;
				}
				else
				{
					ret=false;
				}
			}
			else
			{
				ret=true;
			}
		}
		else
		{
			ret=false;
		}
	}
	else
	{
		ret=false;
	}
	return ret;
}
void Task::Init()
{
	this->ID	= 0;			// ���
	this->Name = nullptr;		// ����

	this->Callback = nullptr;	// �ص�
	this->Param = nullptr;		// ����

	this->Period = 0;		// ����ms 5
	this->NextTime = 0;	// ��һ��ִ��ʱ��ms 3
	
	this->Times = 0;		// ִ�д��� 8
	this->SleepTime = 0;	// ��ǰ˯��ʱ��us 9
	this->Cost = 0;		// ƽ��ִ��ʱ��us 10
	this->CostMs = 0;		// ƽ��ִ��ʱ��ms 11
	this->MaxCost = 0;	// ���ִ��ʱ��us 12

	this->Enable = true;		
	this->Event	= false;		
	this->Deepth = 0;	
	this->MaxDeepth=1;
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
void ShowTime(void * param);//��ʾʱ��
uint mgid=0; // �ܱ��

TaskScheduler::TaskScheduler(cstring name)
{
    this->Name = name;

    mgid = 1;

    this->Running = false;
    this->Current = NULL;
    this->Count = 0;
}

// �������񣬷��������š�dueTime�״ε���ʱ��ms��-1��ʾ�¼�������period���ȼ��ms��-1��ʾ������һ��
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

    Task *task = new Task();
    task->ID = mgid++;
    task->Callback = func;
    task->Param = param;
    task->Period = period;
    task->NextTime = Sys.Ms()*1000 + dueTime;
    task->Name = name;
    if (dueTime < 0)
    {
        task->Enable = false;
    }

    this->Count++;
    _Tasks.Add(task);
    _TasksOld.Add(task);
    #if DEBUG
        debug_printf("%s::���%2d %-11s", Name, task->ID, task->Name);
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

        // ���������%ld���޷��ų�����%llu
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

void TaskScheduler::Remove(uint taskid)
{
    for(int i=0;i<this->Count;i++)
    {
        Task *task = _TasksOld[i];
        if (task->ID == taskid)
        {
            _TasksOld.RemoveAt(i);
            debug_printf("%s::ɾ������%d 0x%08x\r\n", Name, task->ID, (unsigned int)task->Callback);
            // ��������ID������delete��ʱ���ٴ�ɾ��
            task->ID = 0;
            delete task;
            break;
        }
    }
}

void TaskScheduler::Start()
{
	#ifdef DEBUG
	if(!this->Running)
	{
		//this->Add(&TaskScheduler::ShowStatus,this,10000,30000,"ShowStatus");
	}
	#endif
	
	
	
	
    if (Running)
    {
        return ;
    }

    #ifdef DEBUG
		//Sys.AddTask(&TaskScheduler::ShowStatus, this, 1 *1000, 30 *1000, "����״̬");
    #else 
        //Sys.AddTask(ShowTime, this, 2 *1000, 20 *1000, "ʱ����ʾ");
    #endif 

    debug_printf("%s::׼������ ��ʼѭ������%d������\r\n\r\n", Name, Count);

    Running = true;
    while (Running)
    {
		bool bb=false;
        Execute(0xFFFFFFFF,bb);
    }
    debug_printf("%sֹͣ���ȣ�����%d������\r\n", Name, Count);
}

void TaskScheduler::Stop()
{
    debug_printf("%sֹͣ��\r\n", Name);
    Running = false;
}

// ִ��һ��ѭ����ָ��������ʱ��
void TaskScheduler::Execute(uint msMax, bool& cancel)
{
	UInt64 mscur=Sys.Ms();
	#if 0
	TimeCost tmcost;
	int mscurMax = mscur + msMax;
	for(int i=0;i<this->Count;i++)
	{
		Task* taskcur=this->_TasksOld[i];
		if ( taskcur && taskcur->Enable && taskcur->Period)
		{
			if ( taskcur->CheckTime(mscurMax, msMax != -1) )
			{
				taskcur->Execute(mscur);
				taskcur->Times++;
			}
		}
		
	}
	#endif
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
    UInt64 now;
    now = Sys.Ms()*1000; // ��ǰʱ�䡣��ȥϵͳ����ʱ�䣬�����޸�ϵͳʱ����µ���ͣ��	

    UInt64 min = UInt64_Max; // ��Сʱ�䣬���ʱ��ͻ���������
    UInt64 end = Sys.Ms()*1000 + msMax;

    // ��Ҫ������ǰ����ִ������ĵ���
    //Task* _cur = Current;

	for(int i=0;i<this->Count;i++)
    {
        Task *task = _TasksOld[i];
        //if(task && task != _cur && task->Enable && task->NextTime <= now)
        if (task && task->Enable && task->NextTime <= now)
        {
            // ����ͨ���ۼӵķ�ʽ������һ��ʱ�䣬��Ϊ����ϵͳʱ�䱻����
            task->NextTime = now + task->Period;
            if (task->NextTime < min)
            {
                min = task->NextTime;
            }

            UInt64 now2 = Sys.Ms()*1000;
            task->SleepTime = 0;

            this->Current = task;
            task->Execute(Sys.Ms());
            this->Current = NULL;

            // �ۼ�����ִ�д�����ʱ��
            task->Times++;
            int cost = (int)(Sys.Ms()*1000 - now2);
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
                debug_printf("Task::Execute ����:%s %d [%d] ִ��ʱ����� %dus ˯�� %dus\r\n", task->Name, task->ID, task->Times, cost, task->SleepTime);
            }
            // ���ֻ��һ������������������
            if (task->Period < 0)
            {
                Remove(task->ID);
            }
        }

        // ����Ѿ�����������ʱ�䣬���˳�
        if (!msMax || (Sys.Ms()*1000) > end)
        {
            return ;
        }
    }
    // �������Сʱ�䣬˯һ���
    now = Sys.Ms()*1000; // ��ǰʱ��
    if (min != UInt64_Max && min > now)
    {
        min -= now;
        //debug_printf("TaskScheduler::Execute �ȴ���һ��������� %uus\r\n", (uint)min);           
        //// ���ֻ����˯��1�룬����Sys.Delay������ƴ���ͬʱҲ�����Ի�
        //if(min > 1000000) min = 1000000;
        //Sys.Delay(min);
        Time.Delay(min);
    }
}

Task *TaskScheduler::operator[](int taskid)
{
    for(int i=0;i<this->Count;i++)
    {
        Task *task = _TasksOld[i];
        if (task && task->ID == taskid)
        {
            return task;
        }
    }
    return NULL;
}

// �������� ����ʹ�ô˺������׸�����
Task* TaskScheduler::FindTask(Action func)
{
}
// �������һ��˯�ߣ����Ͽ�ʼ��һ��ѭ��
void TaskScheduler::SkipSleep()
{
}
// ʹ���ⲿ��������ʼ�������б�����Ƶ���Ķѷ���
void TaskScheduler::Set(Task* tasks, int count)
{
}
uint TaskScheduler::ExecuteForWait(uint msMax, bool& cancel)
{
}
//��ʾʱ��
void ShowTime(void *param)
{
    UInt64 curms = Time.Milliseconds;
    debug_printf("Time: %02lld:%02lld:%02lld.%03lld\n", curms / 3600000, curms / 60000 % 60, curms / 1000 % 60, curms % 1000);
}

// ��ʾ״̬
void TaskScheduler::ShowStatus()
{
    static UInt64 runCounts = 0;
    float RunTimes = 0;
    float RunTimesAvg = 0;
    Task *tsk;
    byte buf[1];

    runCounts++;
    UInt64 curms = Sys.Ms();

    //ͳ������ʱ��
    RunTimes = 0;
    RunTimesAvg = 0;
    for (int j = 0; j < this->Count; j++)
    {
        tsk = this->_Tasks[j];
        RunTimes += tsk->Cost *tsk->Times;
        RunTimesAvg += tsk->Cost;
    }
    RunTimesAvg /= this->Count;
    //SRAM   0X20000000-0X3FFFFFFF ��512MB
    //SCODE  0X00000000-0X1FFFFFFF ��512MB
	debug_printf("\r\n");
    debug_printf("Task::%s [%llu]", "ShowStatus", runCounts);
    debug_printf("���� %0.2f%% ", RunTimes / 10 / curms);
    if (RunTimesAvg >= 1000)
    {
        debug_printf("ƽ�� %3.1fms ", RunTimesAvg / this->Count / 1000);
    }
    else
    {
        debug_printf("ƽ�� %3.0fus ", RunTimesAvg / this->Count);
    }
    debug_printf("��ǰ 1970-01-01 23 00:00");
    debug_printf("���� ");
    debug_printf("%02lld:%02lld:%02lld.%03lld ", curms / 3600000, curms / 60000 % 60, curms / 1000 % 60, curms % 1000);
    debug_printf("�� %u/%u", &(buf[0]) - 0X20000000, 1024);
    debug_printf("\r\n");

    Task *task = this->Current;
    task->ShowStatus();
    for(int i=0;i<this->Count;i++)
    {
        Task *task = this->_Tasks[i];
        if (task)
        {
            task->ShowStatus();
        }
    }	
}	

// �������� ����ʹ�ô˺������׸������ID
uint TaskScheduler::FindID(Action func)
{
}
