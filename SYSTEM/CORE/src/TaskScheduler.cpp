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


// �������񣬷��������š�dueTime�״ε���ʱ��us��period���ȼ��us��-1��ʾ������һ��
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
    // ���������%ld���޷��ų�����%llu
    //debug_printf("%s::�������%d 0x%08x FirstTime=%llums Period=%ldms\r\n", Name,task->ID, func, dueTime, period);
	debug_printf("%s::���%2d %-10s FirstTime = %8llums Period = %8ldms\r\n", Name,task->ID, task->Name, dueTime, period);

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
    if (Running)
    {
        return ;
    }

    Add(ShowTime, NULL, 2*1000*1000, 30*1000*1000,"ʱ����ʾ");
    //Add(ShowStatus, this, 3*1000*1000, 30*1000*1000,"������ʾ");

    debug_printf("%s::׼������ ��ʼѭ������%d������\r\n\r\n", Name, Count);

    Running = true;
    while (Running)
    {
        Execute(0xFFFFFFFF);
    }
    debug_printf("%sֹͣ���ȣ�����%d������\r\n", Name, Count);
}

void TaskScheduler::Stop()
{
    debug_printf("%sֹͣ��\r\n", Name);
    Running = false;
}

// ִ��һ��ѭ����ָ��������ʱ��
void TaskScheduler::Execute(uint usMax)
{	
	ulong now;	
	now= Time.Current() - Sys.StartTime; // ��ǰʱ�䡣��ȥϵͳ����ʱ�䣬�����޸�ϵͳʱ����µ���ͣ��	
			
    ulong min = UInt64_Max; // ��Сʱ�䣬���ʱ��ͻ���������
    ulong end = Time.Current() + usMax;

    // ��Ҫ������ǰ����ִ������ĵ���
    //Task* _cur = Current;

    int i =   -1;
    while (_Tasks.MoveNext(i))
    {		
        Task *task = _Tasks[i];
        //if(task && task != _cur && task->Enable && task->NextTime <= now)
        if (task && task->Enable && task->NextTime <= now)
        {
            // ����ͨ���ۼӵķ�ʽ������һ��ʱ�䣬��Ϊ����ϵͳʱ�䱻����
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

            // �ۼ�����ִ�д�����ʱ��
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
                debug_printf("Task::Execute ���� %d [%d] ִ��ʱ����� %dus ˯�� %dus\r\n", task->ID, task->Times, cost, task->SleepTime);
            }
            // ���ֻ��һ������������������
            if (task->Period < 0)
            {
                Remove(task->ID);
            }
        }

        // ����Ѿ�����������ʱ�䣬���˳�
        if (!usMax || Time.Current() > end)
        {
            return ;
        }		
    }

    // �������Сʱ�䣬˯һ���
    now = Time.Current(); // ��ǰʱ��
    if (min != UInt64_Max && min > now)
    {
        min -= now;
        //debug_printf("TaskScheduler::Execute �ȴ���һ��������� %uus\r\n", (uint)min);           
        //// ���ֻ����˯��1�룬����Sys.Delay������ƴ���ͬʱҲ�����Ի�
        //if(min > 1000000) min = 1000000;
        //Sys.Delay(min);
        Time.Sleep(min);
    }
}
//��ʾʱ��
void TaskScheduler::ShowTime(void * param)
{
	UInt64 curms=Time.Ms();
	debug_printf("Time: %02lld:%02lld:%02lld\n",curms/3600000,curms/60000%60,curms/1000%60);
}
// ��ʾ״̬
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
