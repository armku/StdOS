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


// �������񣬷��������š�dueTime�״ε���ʱ��ms��period���ȼ��ms��-1��ʾ������һ��
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

//��������ִ�С�����״̬
void TaskScheduler::SetTask(uint taskid, bool onoff, long delaytime)
{
    if (taskid > this->Count)
    {
        //����Χ������
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

    #ifdef DEBUG
        Add(ShowStatus, this, 1 *1000, 30 *1000, "����״̬");
    #else 
        Add(ShowTime, this, 2 *1000, 20 *1000, "ʱ����ʾ");
    #endif 

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
    UInt64 now;
    now = Time.Current(); // ��ǰʱ�䡣��ȥϵͳ����ʱ�䣬�����޸�ϵͳʱ����µ���ͣ��	

    UInt64 min = UInt64_Max; // ��Сʱ�䣬���ʱ��ͻ���������
    UInt64 end = Time.Current() + usMax;

    // ��Ҫ������ǰ����ִ������ĵ���
    //Task* _cur = Current;

    int i =  - 1;
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

            UInt64 now2 = Time.Current();
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
                debug_printf("Task::Execute ����:%s %d [%d] ִ��ʱ����� %dus ˯�� %dus\r\n", task->Name, task->ID, task->Times, cost, task->SleepTime);
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
void TaskScheduler::ShowTime(void *param)
{
    UInt64 curms = Time.Ms();
    debug_printf("Time: %02lld:%02lld:%02lld.%03lld\n", curms / 3600000, curms / 60000 % 60, curms / 1000 % 60, curms % 1000);
}

// ��ʾ״̬
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

    //ͳ������ʱ��
    RunTimes = 0;
    RunTimesAvg = 0;
    for (int j = 0; j < ts->Count; j++)
    {
        tsk = ts->_Tasks[j];
        RunTimes += tsk->Cost *tsk->Times;
        RunTimesAvg += tsk->Cost;
    }
    RunTimesAvg /= ts->Count;
    //SRAM   0X20000000-0X3FFFFFFF ��512MB
    //SCODE  0X00000000-0X1FFFFFFF ��512MB
	debug_printf("\r\n");
    debug_printf("Task::%s [%llu]", "ShowStatus", runCounts);
    debug_printf("���� %0.2f%% ", RunTimes / 10 / curms);
    if (RunTimesAvg >= 1000)
    {
        debug_printf("ƽ�� %3.1fms ", RunTimesAvg / ts->Count / 1000);
    }
    else
    {
        debug_printf("ƽ�� %3.0fus ", RunTimesAvg / ts->Count);
    }
    debug_printf("��ǰ 1970-01-01 23 00:00");
    debug_printf("���� ");
    debug_printf("%02lld:%02lld:%02lld.%03lld ", curms / 3600000, curms / 60000 % 60, curms / 1000 % 60, curms % 1000);
    debug_printf("�� %u/%u", &(buf[0]) - 0X20000000, 1024);
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
