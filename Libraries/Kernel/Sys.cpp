/*
Sys.ID 是12字节芯片唯一标识、也就是ChipID，同一批芯片仅前面几个字节不同
毫秒级睡眠期间，系统将会安排执行其他耗时较短的任务。
 */
#include "TTime.h"
#include "Task.h"
#include <string.h>
#include "Sys.h"
#include "Platform\stm32.h"
#include "Device\Configuration.h"
#include "Core\Buffer.h"

SystemConfig g_Config;//系统配置

TSys Sys; //系统参数

// 构造函数
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

	this->Name = "stdos";
	this->Company = "armku";
	this->Code = 0x0201;
	this->Ver = 0x00;
	this->DevID = 0x00;
	this->RevID = 0x00;
	this->CPUID = 0x00;

	this->FlashSize = 0x01;
	this->RAMSize = 0x01;
	this->OnInit();
	this->Started = false;
}

// 初始化系统
void TSys::Init()
{
	Time.Init();
}

void TSys::ShowInfo()const
{
	this->OnShowInfo();
	StdPrintf("ChipID:");
	Buffer((void*)this->ID, 12).ShowHex(true);

	StdPrintf("Support: http://www.armku.com\n");
}

// 系统启动后的毫秒数
uint64_t TSys::Ms()const
{
	return Time.Current();
}

// 系统绝对当前时间，秒
uint32_t TSys::Seconds()const
{
	return Time.Seconds;
}

// 毫秒级延迟
void TSys::Sleep(int ms)const
{
	if (!this->Started)
	{
		//用于系统没启动时延时使用
		for (int i = 0; i < ms; i++)
		{
			this->Delay(500);
			this->Delay(500);
		}
		return;
	}
	if (ms > 1000)
		debug_printf("Sys::Sleep 设计错误，睡眠%dms太长，超过1000ms建议使用多线程Thread！\n", ms);
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

// 微秒级延迟
void TSys::Delay(int us)const
{
	if (us > 1000000)
		debug_printf("Sys::Sleep 设计错误，睡眠%dus太长，超过1000ms建议使用多线程Thread！\n", us);
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
		Time.Delay(us);
	}
}

// 延迟异步重启
void TSys::Reboot(int msDelay)const
{
	if (msDelay <= 0)
		this->Reset();
	Sys.AddTask((void (TSys::*)())&TSys::Reset, (TSys *)this, msDelay, -1, "Reset");
}

// 创建任务，返回任务编号。dueTime首次调度时间ms，period调度间隔ms，-1表示仅处理一次
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

// 设置任务的开关状态，同时运行指定任务最近一次调度的时间，0表示马上调度
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

// 改变任务周期
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

// 开始系统大循环
void TSys::Start()
{
	//this->=debug_printf;
	this->Started = true;
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
static char *CPUName;

void TSys::OnInit()
{
#if defined STM32F0
	this->Clock = 72000000;
	this->CystalClock = HSE_VALUE;
	memcpy(this->ID, (void*)0x1FFFF7AC, ArrayLength(this->ID));

	this->CPUID = SCB->CPUID;
	uint32_t MCUID = DBGMCU->IDCODE; // MCU编码。低字设备版本，高字子版本
	this->RevID = MCUID >> 16;
	this->DevID = MCUID & 0x0FFF;

	this->FlashSize = *(__IO uint16_t*)(0x1FFFF7CC); // 容量
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
		CPUName = "Unknown";
		this->RAMSize = 0;
		break;
	}
#elif defined STM32F1
	this->Clock = 72000000;
	this->CystalClock = HSE_VALUE;
	memcpy(this->ID, (void*)0x1FFFF7E8, ArrayLength(this->ID));

	this->CPUID = SCB->CPUID;
	uint32_t MCUID = DBGMCU->IDCODE; // MCU编码。低字设备版本，高字子版本
	this->RevID = MCUID >> 16;
	this->DevID = MCUID & 0x0FFF;

	this->FlashSize = *(__IO uint16_t*)(0x1FFFF7E0); // 容量
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
		CPUName = "Unknown";
		this->RAMSize = 0;
		break;
	}
#elif defined STM32F4
	this->Clock = 168000000;
	this->CystalClock = HSE_VALUE;
	memcpy(this->ID, (void*)0x1fff7a10, ArrayLength(this->ID));

	this->CPUID = SCB->CPUID;
	uint32_t MCUID = DBGMCU->IDCODE; // MCU编码。低字设备版本，高字子版本
	this->RevID = MCUID >> 16;
	this->DevID = MCUID & 0x0FFF;

	this->FlashSize = *(__IO uint16_t*)(0X1FFF7a22); // 容量
	switch (this->DevID)
	{
	case 0X0413:
		CPUName = "STM32F407ZG";
		this->RAMSize = 192;
		break;
	default:
		CPUName = "Unknown";
		this->RAMSize = 0;
		break;
	}
#endif
}

void TSys::OnShowInfo()const
{
	uint32_t Rx = 0;
	uint32_t Px = 0;

	uint32_t HeapSize = 0;
	uint32_t StackSize = 0;
#if defined STM32F0
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
#elif defined STM32F1
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
#elif defined STM32F4
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
#endif
	debug_printf("STDOS VER:%s\n", STDOS_VERSION);
	debug_printf("CPU:%s %dMHz Flash:%dk RAM:%dk\n", CPUName, this->Clock/1000/1000, this->FlashSize, this->RAMSize);
	debug_printf("DevID:0x%04X RevID:0x%04X \n", this->DevID, this->RevID);
	debug_printf("CPUID:%p", this->CPUID);
	debug_printf(" ARMv7-M");
#if defined STM32F0
	debug_printf(" Cortex-M%d:", 0);
#elif defined STM32F1
	debug_printf(" Cortex-M%d:", 3);
#elif defined STM32F4
	debug_printf(" Cortex-M%d:", 4);
#endif
	debug_printf(" R%dp%d", Rx, Px);
	debug_printf("\n");
	debug_printf("Heap :(%p, %p) = 0x%x (%dk)\n", (uint32_t)&__heap_base, (uint32_t)&__heap_limit, HeapSize, HeapSize / 1024);
	debug_printf("Stack:(%p, %p) = 0x%x (%dk)\n", (uint32_t)&__heap_limit, (uint32_t)&__initial_sp, StackSize, StackSize / 1024);
}

// 重启系统
void TSys::Reset()const
{
	NVIC_SystemReset();
}
// 打开全局中断
void TSys::GlobalEnable()
{
	__ASM volatile("cpsie i");
}

// 关闭全局中断
void TSys::GlobalDisable()
{
	__ASM volatile("cpsid i");
}
void assert_failed(uint8_t *file, uint32_t line, char *errstr)
{
	StdPrintf("%s(%d):    %s\n", file, line, errstr);
}
#if 0
/*定义STM32 MCU的类型*/
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


uint32_t idAddr[] = { 0x1FFFF7AC,  /*STM32F0唯一ID起始地址*/
0x1FFFF7E8,  /*STM32F1唯一ID起始地址*/
0x1FFF7A10,  /*STM32F2唯一ID起始地址*/
0x1FFFF7AC,  /*STM32F3唯一ID起始地址*/
0x1FFF7A10,  /*STM32F4唯一ID起始地址*/
0x1FF0F420,  /*STM32F7唯一ID起始地址*/
0x1FF80050,  /*STM32L0唯一ID起始地址*/
0x1FF80050,  /*STM32L1唯一ID起始地址*/
0x1FFF7590,  /*STM32L4唯一ID起始地址*/
0x1FF0F420 }; /*STM32H7唯一ID起始地址*/

			  /*获取MCU的唯一ID*/
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
