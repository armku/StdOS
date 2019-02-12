/*
sys.ID ��12�ֽ�оƬΨһ��ʶ��Ҳ����ChipID��ͬһ��оƬ��ǰ�漸���ֽڲ�ͬ
���뼶˯���ڼ䣬ϵͳ���ᰲ��ִ��������ʱ�϶̵�����
 */
#include "TTime.h"
#include "Task.h"
#include <string.h>
#include "Sys.h"
#include "../Bsp/ST/stm32.h"
#include "../Bsp/Porting.h"
#include "../component/lib/Buffer.h"
#include "arduino.h"

Sys_T sys; //ϵͳ����

// ���캯��
Sys_T::Sys_T()
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

void Sys_T::Init()
{
	sys.GlobalDisable();
	Time.Init();
}

void Sys_T::ShowInfo()const
{
	this->OnShowInfo();
	printf("ChipID:");
	Buffer((void*)this->ID, 12).ShowHex(true);
}

// ϵͳ������ĺ�����
uint64_t Sys_T::Ms()const
{
	return Time.Current();
}

// ϵͳ���Ե�ǰʱ�䣬��
uint32_t Sys_T::Seconds()const
{
	return Time.Seconds;
}

// ���뼶�ӳ�
void Sys_T::Sleep(uint32_t dwMs)const
{
	if (!sys.Started)
	{
		//����ϵͳû����ʱ��ʱʹ��
		delay_ms(dwMs);
		return;
	}
	if (dwMs > 1000)
		debug_printf("sys::Sleep ��ƴ���˯��%dms̫��������1000ms����ʹ�ö��߳�Thread��\n", dwMs);
	if (dwMs)
	{
		bool cancel = false;

		int executems = Task::Scheduler()->ExecuteForWait(dwMs, cancel);
		if (executems >= dwMs)
			return;
		dwMs -= executems;
	}
	if (dwMs)
		Time.Sleep(dwMs, nullptr);
}

// ΢�뼶�ӳ�
void Sys_T::delayMicroseconds(uint32_t usec)const
{
	if (usec > 1000000)
		debug_printf("sys::Sleep ��ƴ���˯��%dus̫��������1000ms����ʹ�ö��߳�Thread��\n", usec);
	if (usec && usec >= 1000)
	{
		bool cancle = false;
		int executeus = Task::Scheduler()->ExecuteForWait(usec / 1000, cancle) * 1000;
		if (executeus >= usec)
			return;
		usec -= executeus;
	}
	if (usec)
	{
		delayMicroseconds(usec);
	}
}

// �ӳ��첽����
void Sys_T::Reboot(int msDelay)const
{
	if (msDelay <= 0)
		this->Reset();
	sys.AddTask((void (Sys_T::*)())&Sys_T::Reset, (Sys_T *)this, msDelay, -1, "Reset");
}

// �������񣬷��������š�dueTime�״ε���ʱ��ms��period���ȼ��ms��-1��ʾ������һ��
uint32_t Sys_T::AddTask(Action func, void *param, int dueTime, int period, const char* name)const
{
	return Task::Scheduler()->Add(func, param, dueTime, period, name);
}

void Sys_T::RemoveTask(uint32_t &taskid)const
{
	if (taskid)
	{
		Task::Scheduler()->Remove(taskid);
	}
}

// ��������Ŀ���״̬��ͬʱ����ָ���������һ�ε��ȵ�ʱ�䣬0��ʾ���ϵ���
bool Sys_T::SetTask(uint32_t taskid, bool enable, int msNextTime)const
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
bool Sys_T::SetTaskPeriod(uint32_t taskid, int period)const
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
				tsk->NextTime = sys.Ms() + period;
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
void Sys_T::Start()
{
	this->Started = true;
	sys.GlobalEnable();	
	Task::Scheduler()->Start();
}

#include "../Bsp/Porting.h"
/////////////////////////////////////////////////////////////////////////////
extern "C"
{
	extern uint32_t __heap_base;
	extern uint32_t __heap_limit;
	extern uint32_t __initial_sp;
}

void Sys_T::OnInit()
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

void Sys_T::OnShowInfo()const
{	
	debug_printf("StdOSVer:");this->OsVer.Show();
	debug_printf("  AppVer:");this->AppVer.Show();
	debug_printf("CystalClock:%dMHz SysClock:%dMHz Flash:%dk\n", this->CystalClock / 1000 / 1000, this->Clock / 1000 / 1000, this->FlashSize);
	debug_printf("HeapSize:0X%08X(%dk) StackSize:0X%08X(%dk)\n", this->HeapSize, this->HeapSize / 1024, this->StackSize, this->StackSize / 1024);
}

// ����ϵͳ
void Sys_T::Reset()const
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
	__asm void MSR_MSP(uint32_t addr)
	{
		MSR MSP, r0 			//set Main Stack value
			BX r14
	}
	// ���뼶�ӳ� ����ϵͳû����ʱ��ʱʹ��
	void delay_ms(int ms)
	{
		for (int i = 0; i < ms; i++)
		{
			delayMicroseconds(500);
			delayMicroseconds(500);
		}
	}	
	// ΢�뼶�ӳ�
	void delayMicroseconds(unsigned int us)
	{
		if (us)
		{
			Time.Delay(us);
		}
	}
}
// ��ȫ���ж�
void Sys_T::GlobalEnable()
{
	INTX_ENABLE();
}

// �ر�ȫ���ж�
void Sys_T::GlobalDisable()
{
	INTX_DISABLE();
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
