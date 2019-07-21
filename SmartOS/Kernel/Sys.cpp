#include "Kernel\Sys.h"

#include "Interrupt.h"
#include "TTime.h"

#include "Task.h"
#include <string.h>
#include "Platform\stm32.h"

TSys Sys; //系统参数
TTime Time; //系统时间，不建议用户直接使用
// 系统配置
const SystemConfig g_Config = {
//SystemConfig g_Config = {
	// 操作系统 v3.2.x
	(0x03020000 | __BUILD_DATE__),
	"SmartOS_CPU",

	// 硬件
	(0x0 | __BUILD_DATE__),

	// 应用软件
	(0x0 | __BUILD_DATE__),
	(0x0 | __BUILD_DATE__),
	__BUILD_USER__,
	__BUILD_SDATE__,
};

#if defined(BOOT) || defined(APP)

struct HandlerRemap StrBoot __attribute__((at(0x2000fff0)));

#endif


















































// 构造函数
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

// 系统绝对当前时间，秒
uint TSys::Seconds()const
{
	return Time.Seconds;
}

// 毫秒级延迟
void TSys::Sleep112233(int ms)const
{
	if (!this->Started)
	{
		//用于系统没启动时延时使用
		for (int i = 0; i < ms; i++)
		{
			delayMicroseconds(500);
			delayMicroseconds(500);
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
//	if (ms)
//		Time.Sleep(ms, NULL);
}

// 延迟异步重启
void TSys::Reboot(int msDelay)const
{
	if (msDelay <= 0)
		this->Reset();
	Sys.AddTask((void (TSys::*)())&TSys::Reset, (TSys *)this, msDelay, -1, "Reset");
}

// 创建任务，返回任务编号。dueTime首次调度时间ms，period调度间隔ms，-1表示仅处理一次
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

// 设置任务的开关状态，同时运行指定任务最近一次调度的时间，0表示马上调度
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

// 改变任务周期
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

// 开始系统大循环
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
	extern uint __heap_base;
	extern uint __heap_limit;
	extern uint __initial_sp;
}

void TSys::OnInit()
{
#if defined STM32F0
	this->Clock = 72000000;
	this->CystalClock = HSE_VALUE;
	memcpy(this->ID, (void*)0x1FFFF7AC, ArrayLength(this->ID));
		
	this->FlashSize = *(__IO ushort*)(0x1FFFF7CC); // 容量	
#elif defined STM32F1
	this->Clock = 72000000;
	this->CystalClock = HSE_VALUE;
	memcpy(this->ID, (void*)0x1FFFF7E8, ArrayLength(this->ID));

	uint MCUID = DBGMCU->IDCODE; // MCU编码。低字设备版本，高字子版本
	
	this->FlashSize = *(__IO ushort*)(0x1FFFF7E0); // 容量
#elif defined STM32F4
	this->Clock = 168000000;
	this->CystalClock = HSE_VALUE;
	memcpy(this->ID, (void*)0x1fff7a10, ArrayLength(this->ID));
	
	this->FlashSize = *(__IO ushort*)(0X1FFF7a22); // 容量	
#endif
	HeapSize = ((uint)&__heap_limit - (uint)&__heap_base);
	StackSize = ((uint)&__initial_sp - (uint)&__heap_limit);
}

void TSys::OnShowInfo()const
{	
	debug_printf("StdOSVer:");this->OsVer.Show();
	debug_printf("  AppVer:");this->AppVer.Show();
	debug_printf("CystalClock:%dMHz SysClock:%dMHz Flash:%dk\n", this->CystalClock / 1000 / 1000, this->Clock / 1000 / 1000, this->FlashSize);
	debug_printf("HeapSize:0X%08X(%dk) StackSize:0X%08X(%dk)\n", this->HeapSize, this->HeapSize / 1024, this->StackSize, this->StackSize / 1024);
}

// 重启系统
void TSys::Reset()const
{
	NVIC_SystemReset();
}
extern "C"
{
	//关闭所有中断
	void INTX_DISABLE(void)
	{
		__ASM volatile("cpsid i");
	}
	//开启所有中断
	void INTX_ENABLE(void)
	{
		__ASM volatile("cpsie i");
	}
	//设置栈顶地址
	//addr:栈顶地址
	__asm void MSR_MSP(u32 addr)
	{
		MSR MSP, r0 			//set Main Stack value
			BX r14
	}
}
// 打开全局中断
void TSys::GlobalEnable()
{
	INTX_ENABLE();
}

// 关闭全局中断
void TSys::GlobalDisable()
{
	INTX_DISABLE();
}
void assert_failed(byte *file, uint line, char *errstr)
{
	debug_printf("%s(%d):    %s\n", file, line, errstr);
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

uint idAddr[] = { 0x1FFFF7AC,  /*STM32F0唯一ID起始地址*/
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
