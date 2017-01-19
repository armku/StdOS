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


    // �������񣬷��������š�dueTime�״ε���ʱ��us��period���ȼ��us��-1��ʾ������һ��
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
            // ���������%ld���޷��ų�����%llu
            //debug_printf("%s�������%d 0x%08x FirstTime=%lluus Period=%ldus\r\n", Name, task->ID, func, dueTime, period);
            
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
                debug_printf("%s::ɾ������%d 0x%08x\r\n", Name, task->ID, task->Callback);
                // ��������ID������delete��ʱ���ٴ�ɾ��
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
    #if 0
        ulong now = Time.Current() - Sys.StartTime; // ��ǰʱ�䡣��ȥϵͳ����ʱ�䣬�����޸�ϵͳʱ����µ���ͣ��
        ulong min = UInt64_Max; // ��Сʱ�䣬���ʱ��ͻ���������
        ulong end = Time.Current() + usMax;

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
                    min = task->NextTime;

                ulong now2 = Time.Current();
                task->SleepTime = 0;

                Current = task;
                task->Callback(task->Param);
                Current = NULL;

                // �ۼ�����ִ�д�����ʱ��
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
                        debug_printf("Task::Execute ���� %d [%d] ִ��ʱ����� %dus ˯�� %dus\r\n", task->ID, task->Times, cost, task->SleepTime);
                
                // ���ֻ��һ������������������
                if (task->Period < 0)
                    Remove(task->ID);
            }

            // ����Ѿ�����������ʱ�䣬���˳�
            if (!usMax || Time.Current() > end)
                return ;
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
    #endif 
}

// ��ʾ״̬
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
    //�ⲿע�ắ��
    // ����
    #include "Task.h"
    // ������
    TaskScheduler *_Scheduler;

    // �������񣬷��������š�priority���ȼ���dueTime�״ε���ʱ��us��period���ȼ��us��-1��ʾ������һ��
    uint TSys::AddTask(Action func, void *param, ulong dueTime, long period)
    {
        // �����жϣ�����������̳߳�ͻ
        SmartIRQ irq;

        if (!_Scheduler)
            _Scheduler = new TaskScheduler("ϵͳ");

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
            _Scheduler = new TaskScheduler("ϵͳ");
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
        // �����ʱ�����棬ȥ����һ�±������
        if (_Scheduler && (!us || us >= 1000))
        {
            // ��¼��ǰ����ִ������
            Task *task = _Scheduler->Current;

            ulong start = Time.Current();
            // 1msһ�㲻����������������Ӳ���ȴ�
            ulong end = start + us - 1000;
            // �������ʱ���㹻���������ε�����������
            int cost = 0;
            while (true)
            {
                ulong start2 = Time.Current();

                _Scheduler->Execute(us);

                ulong now = Time.Current();
                cost += (int)(now - start2);

                // us=0 ��ʾ�ͷ�һ��CPU
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
        // ����ʹ���̼߳�˯��
        if (OnSleep)
            OnSleep(ms);
        else
        {            
                if (ms > 1000)
                    debug_printf("Sys::Sleep ��ƴ���˯��%dms̫��������1000ms����ʹ�ö��߳�Thread��", ms);
            
            TimeSleep(ms *1000);
        }
    }

    void TSys::Delay(uint us)
    {
        // ����ӳ�΢����̫����ʹ���̼߳�˯��
        if (OnSleep && us >= 2000)
            OnSleep((us + 500) / 1000);
        else
        {
           
                if (us > 1000000)
                    debug_printf("Sys::Sleep ��ƴ���˯��%dus̫��������1000ms����ʹ�ö��߳�Thread��", us);
           

            TimeSleep(us);
        }
    }

#endif
