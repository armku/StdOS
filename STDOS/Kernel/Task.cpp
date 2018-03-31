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

// ִ�����񡣷����Ƿ�����ִ�С�
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
		int cms=costms.Elapsed();
        int costMsCurrent = costms.Elapsed() - this->SleepTime;
		if(costMsCurrent<0)
			costMsCurrent=30;//�쳣����Ϊ3us		
        if (this->MaxCost < costMsCurrent)
            this->MaxCost = costMsCurrent;
        this->Cost = (5 *this->Cost + 3 * costMsCurrent) / 8;
        this->CostMs = this->Cost / 1000;

        if (costMsCurrent > 500000)
            debug_printf("Task::Execute ����:%s %d [%d] ִ��ʱ����� %dus ˯�� %dus\r\n", this->Name, this->ID, this->Times, costMsCurrent, this->SleepTime);
        #if 0 //��ʱȡ������ɾ������
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

// ��������Ŀ���״̬��ͬʱ����ָ���������һ�ε��ȵ�ʱ�䣬0��ʾ���ϵ���
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

// ��ʾ״̬
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
        debug_printf(" ƽ��[%4.1f%%] ", cpuPercent);
    }
    else
    {
        debug_printf(" ƽ��[%0.2f%%] ", cpuPercent);
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
        debug_printf(" ��� %3ds ", this->MaxCost / 1000000);
    }
    else if (this->MaxCost >= 1000)
    {
        debug_printf(" ��� %3dms", this->MaxCost / 1000);
    }
    else
    {
        debug_printf(" ��� %3dus", this->MaxCost);
    }
    if (this->Period >= 1000)
    {
        debug_printf(" ���� %3lds", this->Period / 1000);
    }
    else
    {
        debug_printf(" ���� %3ldms", this->Period);
    }
    debug_printf(" %s\r\n", this->Enable ? " " : "����");
}

TaskScheduler *_Scheduler;
// ȫ�����������
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
        uint32_t mscur = Sys.Ms();
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
    this->ID = 0; // ���
    this->Name = nullptr; // ����

    this->Callback = nullptr; // �ص�
    this->Param = nullptr; // ����

    this->Period = 0; // ����ms 5
    this->NextTime = 0; // ��һ��ִ��ʱ��ms 3

    this->Times = 0; // ִ�д��� 8
    this->SleepTime = 0; // ��ǰ˯��ʱ��us 9
    this->Cost = 0; // ƽ��ִ��ʱ��us 10
    this->CostMs = 0; // ƽ��ִ��ʱ��ms 11
    this->MaxCost = 0; // ���ִ��ʱ��us 12

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
void ShowTime(void *param); //��ʾʱ��
uint32_t mgid = 0; // �ܱ��

TaskScheduler::TaskScheduler(cstring name)
{
    this->Name = name;

    mgid = 1;
	this->Deepth = 0;
	this->MaxDeepth = 8;

    this->Running = false;
    this->Current = NULL;
    this->Count = 0;
}

// �������񣬷��������š�dueTime�״ε���ʱ��ms��-1��ʾ�¼�������period���ȼ��ms��-1��ʾ������һ��
uint32_t TaskScheduler::Add(Action func, void *param, int dueTime, int period, cstring name)
{
    Task *task = new Task();
    task->ID = mgid++;
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
    //_Tasks.Add(task);
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

        // ���������%ld���޷��ų�����%llu
        if (period >= 1000)
        {
            debug_printf(" Period=  %3lds\r\n", period / 1000);
        }
        else
        {
            debug_printf(" Period= %3ldms\r\n", period);
        }
    #endif 
    return task->ID;
}

void TaskScheduler::Remove(uint32_t taskid)
{
    for (int i = 0; i < this->Count; i++)
    {
        Task *task = this->_Tasks[i];
        if (task->ID == taskid)
        {
            this->_Tasks.RemoveAt(i);
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
        if (!this->Running)
        {
            this->Add(&TaskScheduler::ShowStatus,this,1000*30,1000*30,"ShowStatus");
        }
    #endif 

    if (Running)
    {
        return ;
    }

    #ifdef DEBUG
        //Sys.AddTask(&TaskScheduler::ShowStatus, this, 30 *1000, 30 *1000, "����״̬");
    #else 
        //Sys.AddTask(ShowTime, this, 2 *1000, 20 *1000, "ʱ����ʾ");
    #endif 

    debug_printf("%s::׼������ ��ʼѭ������%d������\r\n\r\n", Name, Count);

    Running = true;
    while (Running)
    {
        bool bb = false;
        Execute(0xFFFFFFFF, bb);
    }
    debug_printf("%sֹͣ���ȣ�����%d������\r\n", Name, Count);
}

void TaskScheduler::Stop()
{
    debug_printf("%sֹͣ��\r\n", Name);
    Running = false;
}

// ִ��һ��ѭ����ָ��������ʱ��
void TaskScheduler::Execute(uint32_t msMax, bool &cancel)
{
    uint64_t mscur = Sys.Ms();
    TimeCost tmcost;

    uint64_t min = UInt64_Max; // ��Сʱ�䣬���ʱ��ͻ���������

    uint64_t mscurMax = mscur + msMax;
    for (int i = 0; i < this->Count; i++)
    {
        if (cancel)
            return ;
        Task *taskcur = this->_Tasks[i];
        if (taskcur && taskcur->Callback && taskcur->Enable && taskcur->NextTime <= mscur)
        {            
			if(taskcur->CheckTime(mscurMax, false))
			{
				if(taskcur->Execute(mscur))
				{
					this->Times++;
				}
				if(!msMax)
					return;
				auto msend = Sys.Ms();
			}
			// ����ͨ���ۼӵķ�ʽ������һ��ʱ�䣬��Ϊ����ϵͳʱ�䱻����
			taskcur->NextTime = mscur + taskcur->Period;
			if (taskcur->NextTime < min)
			{
				min = taskcur->NextTime;
			}
			bool cancel = false;
			this->Current = taskcur;
			if (taskcur->CheckTime(Sys.Ms(), cancel))
				;
			{
				taskcur->Execute(Sys.Ms());
			}
			this->Current = NULL;			
        }
    }

    this->Cost = tmcost.Elapsed();

    // �������Сʱ�䣬˯һ���
    mscur = Sys.Ms(); // ��ǰʱ��
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
        Task *task = this->_Tasks[i];
        if (task && task->ID == taskid)
        {
            return task;
        }
    }
    return NULL;
}

// �������� ����ʹ�ô˺������׸�����
Task *TaskScheduler::FindTask(Action func)
{
    return NULL;
}

// �������һ��˯�ߣ����Ͽ�ʼ��һ��ѭ��
void TaskScheduler::SkipSleep(){}
// ʹ���ⲿ��������ʼ�������б�����Ƶ���Ķѷ���
void TaskScheduler::Set(Task *tasks, int count){}
uint32_t TaskScheduler::ExecuteForWait(uint32_t msMax, bool &cancel)
{
    uint32_t ret = 0;
    if (this->Deepth < MaxDeepth)
    {
        ++this->Deepth;
				
		Task* tskcur=this->Current;
        int msBegin = Sys.Ms();
        int msEndMax = msBegin + msMax;
        int msRemain = msMax;
        TimeCost tmcost;
        while (msRemain > 0 && !cancel)
        {
            this->Execute(msRemain, cancel);
            msRemain = msEndMax - Sys.Ms();
        }
		auto sleepus = tmcost.Elapsed();
        this->TotalSleep += sleepus/1000;
        int msUsed = Sys.Ms() - msBegin;
		tskcur->SleepTime+=sleepus;
		        
        --this->Deepth;
        ret = msUsed;
    }
    else
    {
        ret = false;
    }
    return ret;
}

//��ʾʱ��
void ShowTime(void *param)
{
    uint64_t curms = Time.Milliseconds;
    debug_printf("Time: %02lld:%02lld:%02lld.%03lld\n", curms / 3600000, curms / 60000 % 60, curms / 1000 % 60, curms % 1000);
}

// ��ʾ״̬
void TaskScheduler::ShowStatus()
{

    static uint64_t runCounts = 0;
    float RunTimes = 0;
    float RunTimesAvg = 0;
    Task *tsk;
    uint8_t buf[1];

    runCounts++;
    uint64_t curms = Sys.Ms();
    //debug_printf("\r\n\r\n %lld \r\n\r\n",curms);
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
    debug_printf("\r\nTask::%s [%llu]���� %0.2f%% ", "ShowStatus", runCounts, RunTimes / 10 / curms);
    if (RunTimesAvg >= 1000)
    {
        debug_printf("ƽ�� %3.1fms ", RunTimesAvg / this->Count / 1000);
    }
    else
    {
        debug_printf("ƽ�� %3.0fus ", RunTimesAvg / this->Count);
    }
    debug_printf("��ǰ 1970-01-01 23 00:00");
    debug_printf("���� %02lld:%02lld:%02lld.%03lld �� %u/%u\r\n", curms / 3600000, curms / 60000 % 60, curms / 1000 % 60, curms % 1000, &(buf[0]) - 0X20000000, 1024);
    //debug_printf("\r\n\r\n %lld--%lld \r\n\r\n",Sys.Ms(),Sys.Ms()-curms);
    for (int i = 0; i < this->Count; i++)
    {
        Task *task = this->_Tasks[i];
        if (task)
        {
            task->ShowStatus();
        }
    }
    //debug_printf("\r\n\r\n %lld--%lld \r\n\r\n",Sys.Ms(),Sys.Ms()-curms);
}

// �������� ����ʹ�ô˺������׸������ID
uint32_t TaskScheduler::FindID(Action func)
{
    return 0;
}
