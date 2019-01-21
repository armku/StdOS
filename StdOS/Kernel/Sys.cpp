/*
Sys.ID ��12�ֽ�оƬΨһ��ʶ��Ҳ����ChipID��ͬһ��оƬ��ǰ�漸���ֽڲ�ͬ
���뼶˯���ڼ䣬ϵͳ���ᰲ��ִ��������ʱ�϶̵�����
 */
#include "TTime.h"
#include "Task.h"
#include <string.h>
#include "Sys.h"
#include "Platform\stm32.h"
#include "OnChip\Configuration.h"
#include "../component/lib/Buffer.h"

TSys Sys; //ϵͳ����

// ���캯��
TSys::TSys()
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
	StdPrintf("ChipID:");
	Buffer((void*)this->ID, 12).ShowHex(true);
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
	if (!Sys.Started)
	{
		//����ϵͳû����ʱ��ʱʹ��
		delay_ms(ms);
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
	if (ms)
		Time.Sleep(ms, nullptr);
}

// ΢�뼶�ӳ�
void TSys::Delay(int us)const
{
	if (us > 1000000)
		debug_printf("Sys::Sleep ��ƴ���˯��%dus̫��������1000ms����ʹ�ö��߳�Thread��\n", us);
	if (us && us >= 1000)
	{
		bool cancle = false;
		int executeus = Task::Scheduler()->ExecuteForWait(us / 1000, cancle) * 1000;
		if (executeus >= us)
			return;
		us -= executeus;
	}
	if (us)
	{
		delay_us(us);
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
	this->Started = true;
	Sys.GlobalEnable();	
	Task::Scheduler()->Start();
}

#include "BspPlatform\BspPlatform.h"
/////////////////////////////////////////////////////////////////////////////
extern "C"
{
	extern uint32_t __heap_base;
	extern uint32_t __heap_limit;
	extern uint32_t __initial_sp;
}

void TSys::OnInit()
{
#if defined STM32F0
	this->Clock = 72000000;
	this->CystalClock = HSE_VALUE;
	memcpy(this->ID, (void*)0x1FFFF7AC, ArrayLength(this->ID));
		
	this->FlashSize = *(__IO uint16_t*)(0x1FFFF7CC); // ����	
#elif defined STM32F1
	this->Clock = 72000000;
	this->CystalClock = HSE_VALUE;
	memcpy(this->ID, (void*)0x1FFFF7E8, ArrayLength(this->ID));

	uint32_t MCUID = DBGMCU->IDCODE; // MCU���롣�����豸�汾�������Ӱ汾
	
	this->FlashSize = *(__IO uint16_t*)(0x1FFFF7E0); // ����
#elif defined STM32F4
	this->Clock = 168000000;
	this->CystalClock = HSE_VALUE;
	memcpy(this->ID, (void*)0x1fff7a10, ArrayLength(this->ID));
	
	this->FlashSize = *(__IO uint16_t*)(0X1FFF7a22); // ����	
#endif
	HeapSize = ((uint32_t)&__heap_limit - (uint32_t)&__heap_base);
	StackSize = ((uint32_t)&__initial_sp - (uint32_t)&__heap_limit);
}

void TSys::OnShowInfo()const
{	
	debug_printf("StdOSVer:");this->OsVer.Show();
	debug_printf("  AppVer:");this->AppVer.Show();
	debug_printf("CystalClock:%dMHz SysClock:%dMHz Flash:%dk\n", this->CystalClock / 1000 / 1000, this->Clock / 1000 / 1000, this->FlashSize);
	debug_printf("HeapSize:0X%08X(%dk) StackSize:0X%08X(%dk)\n", this->HeapSize, this->HeapSize / 1024, this->StackSize, this->StackSize / 1024);
}

// ����ϵͳ
void TSys::Reset()const
{
	NVIC_SystemReset();
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
	// ���뼶�ӳ� ����ϵͳû����ʱ��ʱʹ��
	void delay_ms(int ms)
	{
		for (int i = 0; i < ms; i++)
		{
			delay_us(500);
			delay_us(500);
		}
	}
	// ΢�뼶�ӳ�
	void delay_us(int us)
	{
		if (us)
		{
			Time.Delay(us);
		}
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
void assert_failed(uint8_t *file, uint32_t line, char *errstr)
{
	StdPrintf("%s(%d):    %s\n", file, line, errstr);
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

uint32_t idAddr[] = { 0x1FFFF7AC,  /*STM32F0ΨһID��ʼ��ַ*/
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
void GetSTM32MCUID(uint32_t *id, MCUTypedef type)
{
	if (id != NULL)
	{
		id[0] = *(uint32_t*)(idAddr[type]);
		id[1] = *(uint32_t*)(idAddr[type] + 4);
		id[2] = *(uint32_t*)(idAddr[type] + 8);
	}
}
#endif
