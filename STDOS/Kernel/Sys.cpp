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

	this->Config = &g_Config;
	this->OnInit();

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
	this->Started = false;
}

// ��ʼ��ϵͳ
void TSys::Init()
{
	Time.Init();
}

void TSys::ShowInfo()const
{
	StdPrintf("%s::%s Code:%04X %s \r\n", "stdos", "std", 12, "222");
	StdPrintf("Build:%s %s\r\n", "armku", "yyyy-MM-dd HH:mm:ss");

	this->OnShowInfo();
	StdPrintf("ChipID:");
	Buffer buff((void*)this->ID,12);
	buff.ShowHex();
	StdPrintf("\r\n");	
	
	StdPrintf("Support: http://www.armku.com\r\n");
}

// ϵͳ������ĺ�����
uint64_t TSys::Ms()const
{
	return Time.Current();
}

// ϵͳ���Ե�ǰʱ�䣬��
uint32_t TSys::Seconds()const
{
	return Time.Seconds;
}

// ���뼶�ӳ�
void TSys::Sleep(int ms)const
{	
	if (!this->Started)
	{
		//����ϵͳû����ʱ��ʱʹ��
		for (int i = 0; i < ms; i++)
		{
			this->Delay(500);
			this->Delay(500);
		}
		return;
	}

	if (ms > 1000)
		debug_printf("Sys::Sleep ��ƴ���˯��%dms̫��������1000ms����ʹ�ö��߳�Thread��", ms);
	if (ms)
	{
		bool cancel = false;
		int executems = Task::Scheduler()->ExecuteForWait(ms, cancel);
		if (executems >= ms)
			return;
		ms -= executems;
	}
	if (ms)
		Time.Sleep(ms, nullptr);
}

// ΢�뼶�ӳ�
void TSys::Delay(int us)const
{
	if (us > 1000000)
		debug_printf("Sys::Sleep ��ƴ���˯��%dus̫��������1000ms����ʹ�ö��߳�Thread��", us);
	if (us && us >= 1000)
	{
		bool cancle = false;
		int executeus = Task::Scheduler()->ExecuteForWait(us, cancle) * 1000;
		if (executeus >= us)
			return;
		us -= executeus;
	}
	if (us)
	{
		Time.Delay(us);
	}
}

// �ӳ��첽����
void TSys::Reboot(int msDelay)const
{
	if (msDelay <= 0)
		this->Reset();
	Sys.AddTask((void (TSys::*)())&TSys::Reset, (TSys *)this, msDelay, -1, "Reset");
}

// �������񣬷��������š�dueTime�״ε���ʱ��ms��period���ȼ��ms��-1��ʾ������һ��
uint32_t TSys::AddTask(Action func, void *param, int dueTime, int period, cstring name)const
{
	return Task::Scheduler()->Add(func, param, dueTime, period, name);
}

void TSys::RemoveTask(uint32_t &taskid)const
{
	if (taskid)
	{
		Task::Scheduler()->Remove(taskid);
	}
}

// ��������Ŀ���״̬��ͬʱ����ָ���������һ�ε��ȵ�ʱ�䣬0��ʾ���ϵ���
bool TSys::SetTask(uint32_t taskid, bool enable, int msNextTime)const
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
bool TSys::SetTaskPeriod(uint32_t taskid, int period)const
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
	//this->=debug_printf;
	this->Started = true;
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

	Buffer bs(sprint_buf, n);
	SerialPort::GetMessagePort()->Write(bs);

	return n;
}
