#include "Kernel\Sys.h"

#include "Interrupt.h"
#include "TTime.h"

#include "Task.h"
#include <string.h>
#include "Platform\stm32.h"

TSys Sys; //ϵͳ����
TTime Time; //ϵͳʱ�䣬�������û�ֱ��ʹ��
// ϵͳ����
const SystemConfig g_Config = {
//SystemConfig g_Config = {
	// ����ϵͳ v3.2.x
	(0x03020000 | __BUILD_DATE__),
	"SmartOS_CPU",

	// Ӳ��
	(0x0 | __BUILD_DATE__),

	// Ӧ�����
	(0x0 | __BUILD_DATE__),
	(0x0 | __BUILD_DATE__),
	__BUILD_USER__,
	__BUILD_SDATE__,
};

#if defined(BOOT) || defined(APP)

struct HandlerRemap StrBoot __attribute__((at(0x2000fff0)));

#endif


















































// ���캯��
INROOT TSys::TSys()
{
#ifdef STM32F0
	this->CystalClock = 8;
	this->Clock = 48;
#elif defined STM32F1
	this->Clock = 72;
#elif defined STM32F4
	this->CystalClock = 8;
	this->Clock = 168;
#endif 	
	this->OsVer.Parse(STDOS_VERSION);
	this->AppVer.Parse(AppVersion);
	this->FlashSize = 0x01;
	this->OnInit();
	this->Started = false;
}

void TSys::Init()
{
	Sys.GlobalDisable();
	Time.Init();
}

void TSys::ShowInfo()const
{
	this->OnShowInfo();
	debug_printf("ChipID:");
	Buffer((void*)this->ID, 12).ShowHex(true);
}
// ϵͳ������ĺ�����
INROOT UInt64 TSys::Ms() const { return Time.Current(); }
// ϵͳ���Ե�ǰʱ�䣬��
uint TSys::Seconds()const
{
	return Time.Seconds;
}
INROOT void TSys::Sleep(int ms) const
{
	// ����ʹ���̼߳�˯��
	if(OnSleep)
		OnSleep(ms);
	else
	{
#if DEBUG
		if(ms > 1000) debug_printf("Sys::Sleep ��ƴ���˯��%dms̫����", ms);
#endif

		// �����ʱ�����棬ȥ����һ�±������
		if(Sys.Started && ms != 0)
		{
			bool cancel	= false;
			int ct	= Task::Scheduler()->ExecuteForWait(ms, cancel);

			if(ct >= ms) return;

			ms	-= ct;
		}
		//if(ms) Time.Sleep(ms);
	}
}

INROOT void TSys::Delay(int us) const
{
	// ����ӳ�΢����̫����ʹ���̼߳�˯��
	if(OnSleep && us >= 2000)
		OnSleep((us + 500) / 1000);
	else
	{
#if DEBUG
		if(us > 1000000) debug_printf("Sys::Sleep ��ƴ���˯��%dus̫����", us);
#endif

		// �����ʱ�����棬ȥ����һ�±������
		if(Sys.Started && us != 0 && us >= 1000)
		{
			bool cancel	= false;
			int ct	= Task::Scheduler()->ExecuteForWait(us / 1000, cancel);
			ct	*= 1000;

			if(ct >= us) return;

			us -= ct;
		}
		//if(us) Time.Delay(us);
	}
}
// ���뼶�ӳ�
void TSys::Sleep112233(int ms)const
{
	if (!this->Started)
	{
		//����ϵͳû����ʱ��ʱʹ��
		for (int i = 0; i < ms; i++)
		{
			delayMicroseconds(500);
			delayMicroseconds(500);
		}
		return;
	}
	if (ms > 1000)
		debug_printf("Sys::Sleep ��ƴ���˯��%dms̫��������1000ms����ʹ�ö��߳�Thread��\n", ms);
	if (ms)
	{
		bool cancel = false;

		int executems = Task::Scheduler()->ExecuteForWait(ms, cancel);
		if (executems >= ms)
			return;
		ms -= executems;
	}
//	if (ms)
//		Time.Sleep(ms, NULL);
}

// �ӳ��첽����
void TSys::Reboot(int msDelay)const
{
	if (msDelay <= 0)
		this->Reset();
	Sys.AddTask((void (TSys::*)())&TSys::Reset, (TSys *)this, msDelay, -1, "Reset");
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
		Task* tsk = Task::Get(taskid);
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
				tsk->NextTime = millis() + period;
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
	this->Started = true;
	Sys.GlobalEnable();	
	Task::Scheduler()->Start();
}
/****************ϵͳ����****************/

//#if DEBUG
	#include "Device\Port.h"
	static OutputPort* _trace = nullptr;
//#endif
void TSys::InitTrace(void* port) const
{
//#if DEBUG
	_trace	= (OutputPort*)port;
//#endif
}

void TSys::Trace(int times) const
{
//#if DEBUG
	if(_trace)
	{
		while(times--) *_trace = !*_trace;
	}
//#endif
}

#include "BspPlatform\BspPlatform.h"
/////////////////////////////////////////////////////////////////////////////
extern "C"
{
	extern uint __heap_base;
	extern uint __heap_limit;
	extern uint __initial_sp;
}





extern "C"
{
	//�ر������ж�
	void INTX_DISABLE(void)
	{
		__ASM volatile("cpsid i");
	}
	//���������ж�
	void INTX_ENABLE(void)
	{
		__ASM volatile("cpsie i");
	}
	//����ջ����ַ
	//addr:ջ����ַ
	__asm void MSR_MSP(u32 addr)
	{
		MSR MSP, r0 			//set Main Stack value
			BX r14
	}
}
// ��ȫ���ж�
void TSys::GlobalEnable()
{
	INTX_ENABLE();
}

// �ر�ȫ���ж�
void TSys::GlobalDisable()
{
	INTX_DISABLE();
}
void assert_failed(byte *file, uint line, char *errstr)
{
	debug_printf("%s(%d):    %s\n", file, line, errstr);
}

#if 0
/*����STM32 MCU������*/
typedef enum {
	STM32F0,
	STM32F1,
	STM32F2,
	STM32F3,
	STM32F4,
	STM32F7,
	STM32L0,
	STM32L1,
	STM32L4,
	STM32H7,
}MCUTypedef;

uint idAddr[] = { 0x1FFFF7AC,  /*STM32F0ΨһID��ʼ��ַ*/
0x1FFFF7E8,  /*STM32F1ΨһID��ʼ��ַ*/
0x1FFF7A10,  /*STM32F2ΨһID��ʼ��ַ*/
0x1FFFF7AC,  /*STM32F3ΨһID��ʼ��ַ*/
0x1FFF7A10,  /*STM32F4ΨһID��ʼ��ַ*/
0x1FF0F420,  /*STM32F7ΨһID��ʼ��ַ*/
0x1FF80050,  /*STM32L0ΨһID��ʼ��ַ*/
0x1FF80050,  /*STM32L1ΨһID��ʼ��ַ*/
0x1FFF7590,  /*STM32L4ΨһID��ʼ��ַ*/
0x1FF0F420 }; /*STM32H7ΨһID��ʼ��ַ*/

			  /*��ȡMCU��ΨһID*/
void GetSTM32MCUID(uint *id, MCUTypedef type)
{
	if (id != NULL)
	{
		id[0] = *(uint*)(idAddr[type]);
		id[1] = *(uint*)(idAddr[type] + 4);
		id[2] = *(uint*)(idAddr[type] + 8);
	}
}
#endif
