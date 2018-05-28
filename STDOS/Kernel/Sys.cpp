/*
ϵͳ���Զ�ʶ�𳣼�оƬ���͡���Ƶ��Flash��С��Ram��С
Sys.ID ��12�ֽ�оƬΨһ��ʶ��Ҳ����ChipID��ͬһ��оƬ��ǰ�漸���ֽڲ�ͬ
���뼶˯���ڼ䣬ϵͳ���ᰲ��ִ��������ʱ�϶̵��������û�п�������ϵͳ�������͹���ģʽ���Խ�ʡ�ܺ�
 */
#include "TTime.h"
#include "Task.h"
#include <string.h>
#include "Sys.h"
#include "stdarg.h"
#include "Platform\stm32.h"
#include "Device\DeviceConfigHelper.h"

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

	this->Config = &g_Config;
	this->OnInit();

	this->Name = "stdos";
	this->Company = "armku";
	this->Code = 0x0201;
	this->Ver = 0x00;
	this->DevID = 0x00;
	this->RevID = 0x00;
	this->CPUID = 0x00;

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
	Buffer buff((void*)this->ID, 12);
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
#include "BspPlatform\BspPlatform.h"
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
	DeviceConfigHelper::com1send(bs);
	
	return n;
}
extern "C"
{
	extern uint32_t __heap_base;
	extern uint32_t __heap_limit;
	extern uint32_t __initial_sp;
}
static char *CPUName;

void TSys::OnInit()
{
#if defined STM32F0
	this->Clock = 72000000;
	this->CystalClock = HSE_VALUE;
	Buffer::Copy(this->ID, (void*)0x1FFFF7AC, ArrayLength(this->ID));

	this->CPUID = SCB->CPUID;
	uint32_t MCUID = DBGMCU->IDCODE; // MCU���롣�����豸�汾�������Ӱ汾
	this->RevID = MCUID >> 16;
	this->DevID = MCUID & 0x0FFF;

	this->FlashSize = *(__IO uint16_t*)(0x1FFFF7CC); // ����
	switch (this->DevID)
	{
	case 0X0440:
		CPUName = "STM32F030C8";
		this->RAMSize = 8;
		break;
	case 0X0448:
		CPUName = "STM32F072VB";
		this->RAMSize = 64;
		break;
	default:
		CPUName = "δ֪";
		this->RAMSize = 0;
		break;
	}
#elif defined STM32F1
	this->Clock = 72000000;
	this->CystalClock = HSE_VALUE;
	Buffer::Copy(this->ID, (void*)0x1FFFF7E8, ArrayLength(this->ID));

	this->CPUID = SCB->CPUID;
	uint32_t MCUID = DBGMCU->IDCODE; // MCU���롣�����豸�汾�������Ӱ汾
	this->RevID = MCUID >> 16;
	this->DevID = MCUID & 0x0FFF;

	this->FlashSize = *(__IO uint16_t*)(0x1FFFF7E0); // ����
	switch (this->DevID)
	{
	case 0X0307:
		CPUName = "STM32F103RD";
		this->RAMSize = 64;
		break;
	case 0x0410:
		CPUName = "STM32F103C8";
		this->RAMSize = 20;
		break;
	case 0X0414:
		CPUName = "STM32F103ZE";
		this->RAMSize = 64;
		break;
	case 0X0418:
		CPUName = "STM32F105VC";
		this->RAMSize = 64;
		break;
	case 0X0430:
		CPUName = "STM32F103VG";
		this->RAMSize = 768;
		break;
	default:
		CPUName = "δ֪";
		this->RAMSize = 0;
		break;
	}
#elif defined STM32F4
	this->Clock = 168000000;
	this->CystalClock = HSE_VALUE;
	Buffer::Copy(this->ID, (void*)0x1fff7a10, ArrayLength(this->ID));

	this->CPUID = SCB->CPUID;
	uint32_t MCUID = DBGMCU->IDCODE; // MCU���롣�����豸�汾�������Ӱ汾
	this->RevID = MCUID >> 16;
	this->DevID = MCUID & 0x0FFF;

	this->FlashSize = *(__IO uint16_t*)(0X1FFF7a22); // ����
	switch (this->DevID)
	{
	case 0X0413:
		CPUName = "STM32F407ZG";
		this->RAMSize = 192;
		break;
	default:
		CPUName = "δ֪";
		this->RAMSize = 0;
		break;
	}
#endif
}

void TSys::OnShowInfo()const
{
#if defined STM32F0
	uint32_t Rx = 0;
	uint32_t Px = 0;

	uint32_t HeapSize = 0;
	uint32_t StackSize = 0;


	HeapSize = ((uint32_t)&__heap_limit - (uint32_t)&__heap_base);
	StackSize = ((uint32_t)&__initial_sp - (uint32_t)&__heap_limit);

	switch (this->CPUID &SCB_CPUID_VARIANT_Msk)
	{
	case 0:
		if ((this->CPUID &SCB_CPUID_REVISION_Msk) == 1)
		{
			Rx = 1;
			Px = 0;
		}
		else
		{
			Rx = 0;
			Px = 0;
		}
		break;
	case 0X00100000:
		Rx = 1;
		if ((this->CPUID &SCB_CPUID_REVISION_Msk) == 1)
		{
			Px = 1;
		}
		break;
	case 0X00200000:
		Rx = 2;
		if ((this->CPUID &SCB_CPUID_REVISION_Msk) == 0)
		{
			Px = 0;
		}
		break;
	default:
		Px = 9;
		Rx = 9;
		break;
	}


	debug_printf("STDOS::");
	debug_printf("STM32");
	debug_printf("F103");
	debug_printf("%s %dMHz Flash:%dk RAM:%dk\r\n", CPUName, this->Clock, this->FlashSize, this->RAMSize);
	debug_printf("DevID:0x%04X RevID:0x%04X \r\n", this->DevID, this->RevID);
	debug_printf("CPUID:%p", this->CPUID);
	debug_printf(" ARMv7-M");
	debug_printf(" Cortex-M%d:", 3);
	debug_printf(" R%dp%d", Rx, Px);
	debug_printf("\r\n");
	debug_printf("Heap :(%p, %p) = 0x%x (%dk)\r\n", (uint32_t)&__heap_base, (uint32_t)&__heap_limit, HeapSize, HeapSize / 1024);
	debug_printf("Stack:(%p, %p) = 0x%x (%dk)\r\n", (uint32_t)&__heap_limit, (uint32_t)&__initial_sp, StackSize, StackSize / 1024);

	debug_printf("ChipType:0x42455633 3\r\n");
#elif defined STM32F1
	uint32_t Rx = 0;
	uint32_t Px = 0;

	uint32_t HeapSize = 0;
	uint32_t StackSize = 0;

	HeapSize = ((uint32_t)&__heap_limit - (uint32_t)&__heap_base);
	StackSize = ((uint32_t)&__initial_sp - (uint32_t)&__heap_limit);

	switch (this->CPUID &SCB_CPUID_VARIANT_Msk)
	{
	case 0:
		if ((this->CPUID &SCB_CPUID_REVISION_Msk) == 1)
		{
			Rx = 1;
			Px = 0;
		}
		else
		{
			Rx = 0;
			Px = 0;
		}
		break;
	case 0X00100000:
		Rx = 1;
		if ((this->CPUID &SCB_CPUID_REVISION_Msk) == 1)
		{
			Px = 1;
		}
		break;
	case 0X00200000:
		Rx = 2;
		if ((this->CPUID &SCB_CPUID_REVISION_Msk) == 0)
		{
			Px = 0;
		}
		break;
	default:
		Px = 9;
		Rx = 9;
		break;
	}

	debug_printf("STDOS::");
	debug_printf("STM32");
	debug_printf("F103");
	debug_printf("%s %dMHz Flash:%dk RAM:%dk\r\n", CPUName, this->Clock, this->FlashSize, this->RAMSize);
	debug_printf("DevID:0x%04X RevID:0x%04X \r\n", this->DevID, this->RevID);
	debug_printf("CPUID:%p", this->CPUID);
	debug_printf(" ARMv7-M");
	debug_printf(" Cortex-M%d:", 3);
	debug_printf(" R%dp%d", Rx, Px);
	debug_printf("\r\n");
	debug_printf("Heap :(%p, %p) = 0x%x (%dk)\r\n", (uint32_t)&__heap_base, (uint32_t)&__heap_limit, HeapSize, HeapSize / 1024);
	debug_printf("Stack:(%p, %p) = 0x%x (%dk)\r\n", (uint32_t)&__heap_limit, (uint32_t)&__initial_sp, StackSize, StackSize / 1024);

	debug_printf("ChipType:0x42455633 3\r\n");
#elif defined STM32F4
	uint32_t Rx = 0;
	uint32_t Px = 0;

	uint32_t HeapSize = 0;
	uint32_t StackSize = 0;


	HeapSize = ((uint32_t)&__heap_limit - (uint32_t)&__heap_base);
	StackSize = ((uint32_t)&__initial_sp - (uint32_t)&__heap_limit);

	switch (this->CPUID &SCB_CPUID_VARIANT_Msk)
	{
	case 0:
		if ((this->CPUID &SCB_CPUID_REVISION_Msk) == 1)
		{
			Rx = 1;
			Px = 0;
		}
		else
		{
			Rx = 0;
			Px = 0;
		}
		break;
	case 0X00100000:
		Rx = 1;
		if ((this->CPUID &SCB_CPUID_REVISION_Msk) == 1)
		{
			Px = 1;
		}
		break;
	case 0X00200000:
		Rx = 2;
		if ((this->CPUID &SCB_CPUID_REVISION_Msk) == 0)
		{
			Px = 0;
		}
		break;
	default:
		Px = 9;
		Rx = 9;
		break;
	}


	debug_printf("STDOS::");
	debug_printf("STM32");
	debug_printf("F103");
	debug_printf("%s %dMHz Flash:%dk RAM:%dk\r\n", CPUName, this->Clock, this->FlashSize, this->RAMSize);
	debug_printf("DevID:0x%04X RevID:0x%04X \r\n", this->DevID, this->RevID);
	debug_printf("CPUID:%p", this->CPUID);
	debug_printf(" ARMv7-M");
	debug_printf(" Cortex-M%d:", 3);
	debug_printf(" R%dp%d", Rx, Px);
	debug_printf("\r\n");
	debug_printf("Heap :(%p, %p) = 0x%x (%dk)\r\n", (uint32_t)&__heap_base, (uint32_t)&__heap_limit, HeapSize, HeapSize / 1024);
	debug_printf("Stack:(%p, %p) = 0x%x (%dk)\r\n", (uint32_t)&__heap_limit, (uint32_t)&__initial_sp, StackSize, StackSize / 1024);

	debug_printf("ChipType:0x42455633 3\r\n");
#endif
}

// ����ϵͳ
void TSys::Reset()const
{
	NVIC_SystemReset();
}
// ��ȫ���ж�
void GlobalEnable()
{
	__ASM volatile("cpsie i");
}

// �ر�ȫ���ж�
void GlobalDisable()
{
	__ASM volatile("cpsid i");
}
