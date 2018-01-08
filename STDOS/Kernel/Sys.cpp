/*
ϵͳ���Զ�ʶ�𳣼�оƬ���͡���Ƶ��Flash��С��Ram��С
Sys.ID ��12�ֽ�оƬΨһ��ʶ��Ҳ����ChipID��ͬһ��оƬ��ǰ�漸���ֽڲ�ͬ
���뼶˯���ڼ䣬ϵͳ���ᰲ��ִ��������ʱ�϶̵��������û�п�������ϵͳ�������͹���ģʽ���Խ�ʡ�ܺ�

 */
#include "TTime.h"
#include "Task.h"
#include <string.h>
#include "TInterrupt.h"
#include "SerialPort.h"
#include "Sys.h"
#include "stdarg.h"

SystemConfig g_Config;//ϵͳ����

TSys Sys; //ϵͳ����

//�ⲿע�ắ��
// ����
// ������
extern TaskScheduler *_Scheduler;
void TimeSleep(uint us)
{
    // �����ʱ�����棬ȥ����һ�±������
    if (_Scheduler && (us >= 1000))
    {
        // ��¼��ǰ����ִ������
        Task *task = _Scheduler->Current;

        UInt64 start = Time.Current() *1000;
        // 1msһ�㲻����������������Ӳ���ȴ�
        UInt64 end = start + us - 1000;
        // �������ʱ���㹻���������ε�����������
        int cost = 0;
        while (true)
        {
            UInt64 start2 = Time.Current() *1000;
            bool bb = false;
            _Scheduler->Execute(us, bb);

            UInt64 now = Time.Current() *1000;
            cost += (int)(now - start2);

            // us=0 ��ʾ�ͷ�һ��CPU
            if (!us)
            {
                return ;
            }

            if (now >= end)
            {
                break;
            }
        }

        if (task)
        {
            _Scheduler->Current = task;
            task->SleepTime += cost;
        }

        cost = (int)(Time.Current() *1000-start);
        if (cost > 0)
        {
            return ;
        }

        us -= cost;
    }
    if (us)
    {
        Time.Delay(us);
    }
}

// ���캯��
TSys::TSys()
{
    #ifdef STM32F0
        this->CystalClock = 8;
        this->Clock = 48;
    #elif defined STM32F1
        this->CystalClock = 8;
        this->Clock = 72;
    #elif defined STM32F4
        this->CystalClock = 8;
        this->Clock = 168;
    #endif 
    this->MessagePort = COM1;

    this->Config=&g_Config;
    this->OnInit();
	
//	*((_DWORD *)pthis + 19) = 0;
//	*((_WORD *)pthis + 12) = 6450;
//	*((_DWORD *)pthis + 7) = 50469170;
//	*((_DWORD *)pthis + 8) = 6450;
//	*((_DWORD *)pthis + 9) = 6450;
//	*((_DWORD *)pthis + 3) = "SmartOS_CPU";
//	*((_DWORD *)pthis + 4) = "X3_nnhy";
//	*((_DWORD *)pthis + 5) = &unk_6E8;
//	TInterrupt::Init((TInterrupt *)&Interrupt);
//	*((_DWORD *)pthis + 16) = 1;
//	*((_DWORD *)pthis + 17) = 1;
//	*((_BYTE *)pthis + 80) = 0;
	
    this->MessagePort = COM1;
    this->Name = "stdos";
    this->Company = "armku";
    this->Code = 0x0201;
    this->Ver = 0x00;
    this->DevID = 0x00;
    this->RevID = 0x00;
    this->CPUID = 0x00;

    Interrupt.Init();
    this->FlashSize = 0x01;
    this->RAMSize = 0x01;
    this->OnSleep = NULL;
}

// ��ʼ��ϵͳ
void TSys::Init()
{
    this->InitClock();
    Time.Init();
}

void TSys::ShowInfo()const
{
    StdPrintf("%s::%s Code:%04X %s \r\n", "stdos", "std", 12, "222");
    StdPrintf("Build:%s %s\r\n", "armku", "yyyy-MM-dd HH:mm:ss");

    this->OnShowInfo();	
    StdPrintf("ChipID:");
	ByteArray baid(this->ID,12);
	baid.Show();StdPrintf("\r\n");
    StdPrintf("Time : ");
    DateTime dt(Time.Seconds + Time.BaseSeconds);
    dt.Show(true);

    StdPrintf("Support: http://www.armku.com\r\n");
}

// ϵͳ������ĺ�����
UInt64 TSys::Ms()const
{
    return Time.Current();
}

// ϵͳ���Ե�ǰʱ�䣬��
uint TSys::Seconds()const
{
    return Time.Seconds;

    //return Time + dword_6A0;
}

// ���뼶�ӳ�
void TSys::Sleep(int ms)const
{
    if (this->OnSleep)
    {
        this->OnSleep(ms);
    }
    else
    {
        if (ms > 1000)
            debug_printf("Sys::Sleep ��ƴ���˯��%dms̫��������1000ms����ʹ�ö��߳�Thread��", ms);
        if (ms)
        {
            bool cancel = false;
            int executems = Task::Scheduler()->ExecuteForWait(ms, cancel);
            if (executems >= ms)
                return ;
            ms -= executems;
        }
        if (ms)
            Time.Sleep(ms, nullptr);
    }
}

// ΢�뼶�ӳ�
void TSys::Delay(int us)const
{
    if (this->OnSleep && us >= 2000)
    {
        this->OnSleep((us + 500) / 1000);
    }
    else
    {
        if (us > 1000000)
            debug_printf("Sys::Sleep ��ƴ���˯��%dus̫��������1000ms����ʹ�ö��߳�Thread��", us);
        if (us && us >= 1000)
        {
            bool cancle = false;
            int executeus = Task::Scheduler()->ExecuteForWait(us, cancle) *1000;
            if (executeus >= us)
                return ;
            us -= executeus;
        }
        if (us)
		{
			if(us > 800)
				Time.Delay(us);
			else
				Time.DelayUs(us);
		}
    }
    return ;
}

// �ӳ��첽����
void TSys::Reboot(int msDelay)const
{
    if (msDelay <= 0)
        this->Reset();
	Sys.AddTask((void (TSys::*)())&TSys::Reset,(TSys *)this,msDelay,-1,"Reset");
}

// ϵͳ����
void TSys::InitTrace(void *port)const
{
    //*(_DWORD *)trace = a2;
}

void TSys::Trace(int times)const{

}

// �������񣬷��������š�dueTime�״ε���ʱ��ms��period���ȼ��ms��-1��ʾ������һ��
uint TSys::AddTask(Action func, void *param, int dueTime, int period, cstring name)const
{
    return Task::Scheduler()->Add(func, param, dueTime, period, name);
}

void TSys::RemoveTask(uint &taskid)const
{
    if (taskid)
    {
        Task::Scheduler()->Remove(taskid);
    }
}

// ��������Ŀ���״̬��ͬʱ����ָ���������һ�ε��ȵ�ʱ�䣬0��ʾ���ϵ���
bool TSys::SetTask(uint taskid, bool enable, int msNextTime)const
{
    bool ret;

    if (taskid)
    {
        auto tsk = Task::Get(taskid);
        if (tsk)
        {
            tsk->Set(enable, msNextTime);
            ret = true;
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

// �ı���������
bool TSys::SetTaskPeriod(uint taskid, int period)const
{
    bool ret;
    Task *tsk;

    if (taskid)
    {
        tsk = Task::Get(taskid);
        if (tsk)
        {
            if (period)
            {
                tsk->Period = period;
                tsk->NextTime = Sys.Ms() + period;
            }
            else
            {
                tsk->Enable = false;
            }
            ret = true;
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

// ��ʼϵͳ��ѭ��
void TSys::Start()
{
    this->OnStart();
    //this->=debug_printf;
    Task::Scheduler()->Start();
}
#include <stdio.h>
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int StdPrintf(const char *format, ...)
{
	static char sprint_buf[1024];
	va_list args;

	int n;
	va_start(args, format);
	n = vsprintf(sprint_buf, format, args);
	va_end(args);
	
	Buffer bs(sprint_buf,n);
	SerialPort::GetMessagePort()->Write(bs);
	
	return n;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if DEBUG
    // ����ջ��
    // ���뺯��ʱѹջ���������뿪ʱ�����������쳣ʱ��ȡ���̵߳����б�
    TraceStack::TraceStack(cstring name){}
    TraceStack::~TraceStack(){}

    void TraceStack::Show(){}

#endif
