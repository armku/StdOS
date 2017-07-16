/*
系统能自动识别常见芯片类型、主频、Flash大小、Ram大小
Sys.ID 是12字节芯片唯一标识、也就是ChipID，同一批芯片仅前面几个字节不同
毫秒级睡眠期间，系统将会安排执行其他耗时较短的任务。如果没有可用任务，系统将会进入低功耗模式，以节省能耗

 */
#include "stdio.h"
#include "Sys.h"
#include "TTime.h"
#include "Task.h"
#include "Sys.h"
#include <string.h>
#include "Array.h"
#include "DateTime.h"
#include "TInterrupt.h"
#ifdef STM32F1
	#include "stm32f10x.h"
#endif
#ifdef STM32F4
	#include "stm32f4xx.h"
#endif

#if 0
extern "C"
{
    extern uint __heap_base;
    extern uint __heap_limit;
    extern uint __initial_sp;
}
#endif
String *CPUName;
void assert_failed2(cstring msg, cstring file, unsigned int line)
{
}
//外部注册函数
// 任务
#include "Task.h"
// 任务类
TaskScheduler *_Scheduler;

// 创建任务，返回任务编号。dueTime首次调度时间ms，period调度间隔ms，-1表示仅处理一次
uint TSys::AddTask(Action func, void* param, int dueTime, int period, cstring name) const
{
    // 屏蔽中断，否则可能有线程冲突
    SmartIRQ irq;

    if (!_Scheduler)
    {
        _Scheduler = new TaskScheduler("Sys");
    }

    return _Scheduler->Add(func, param, dueTime, period, name);
}

void TSys::RemoveTask(uint& taskid) const
{
    #if 0
        assert_ptr(_Scheduler);
    #endif 
    _Scheduler->Remove(taskid);
}
// 设置任务的开关状态，同时运行指定任务最近一次调度的时间，0表示马上调度
bool TSys::SetTask(uint taskid, bool enable, int msNextTime) const
{
//    _Scheduler->SetTask(taskid, enable, msNextTime);
	return true;
}

//启动系统任务调度，该函数内部为死循环。*在此之间，添加的所有任务函数将得不到调度，所有睡眠方法无效！
void TSys::Start()
{
    if (!_Scheduler)
    {
        _Scheduler = new TaskScheduler("Sys");
    }

    #if 0		
        if (OnStart)
        {
            OnStart();
        }
        else
    #endif 
    {
        _Scheduler->Start();
    }
}



void TimeSleep(uint us)
{
    // 在这段时间里面，去处理一下别的任务
    if (_Scheduler && (us >= 1000))
    {
        // 记录当前正在执行任务
        Task *task = _Scheduler->Current;

        UInt64 start = Time.Current();
        // 1ms一般不够调度新任务，留给硬件等待
        UInt64 end = start + us - 1000;
        // 如果休眠时间足够长，允许多次调度其它任务
        int cost = 0;
        while (true)
        {
            UInt64 start2 = Time.Current();
			bool bb=false;
            _Scheduler->Execute(us,bb);

            UInt64 now = Time.Current();
            cost += (int)(now - start2);

            // us=0 表示释放一下CPU
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

        cost = (int)(Time.Current() - start);
        if (cost > 0)
        {
            return ;
        }

        us -= cost;
    }
    if (us)
    {
        Time.Sleep(us);
    }
}
void TSys::Sleep(int ms) const // 毫秒级延迟
{
    // 优先使用线程级睡眠
    #if 0
        if (OnSleep)
        {
            OnSleep(ms);
        }
        else
    #endif 
    {
        if (ms > 1000)
        {
            debug_printf("Sys::Sleep 设计错误，睡眠%dms太长，超过1000ms建议使用多线程Thread！", ms);
        }
        TimeSleep(ms *1000);
    }
}
void TSys::Delay(int us) const // 微秒级延迟
{
    // 如果延迟微秒数太大，则使用线程级睡眠
    #if 0
        if (OnSleep && us >= 2000)
        {
            OnSleep((us + 500) / 1000);
        }
        else
    #endif 
    {

        if (us > 1000000)
        {
            debug_printf("Sys::Sleep 设计错误，睡眠%dus太长，超过1000ms建议使用多线程Thread！", us);
        }
        TimeSleep(us);
    }
}

TSys::TSys()
{
	this->Clock = 72000000;
    this->MessagePort = COM1;
}


UInt64	 TSys::Ms() const		// 系统启动后的毫秒数
{
    return Time.Ms();
}
uint	TSys::Seconds() const	// 系统绝对当前时间，秒
{
    return Time.Seconds;
}

// 延迟异步重启
void TSys::Reboot(int msDelay ) const
{
	NVIC_SystemReset();
}
void TSys::ShowInfo() const
{
	uint Rx=0;
	uint Px=0;
#if 0	
    uint HeapSize = 0;
    uint StackSize = 0;
	

    HeapSize = ((uint) &__heap_limit - (uint) &__heap_base);
    StackSize = ((uint) &__initial_sp - (uint) &__heap_limit);
#endif
	switch(this->CPUID&SCB_CPUID_VARIANT_Msk)
	{
		case 0:
			if((this->CPUID&SCB_CPUID_REVISION_Msk)==1)
			{
				Rx=1;
				Px=0;
			}
			else
			{
				Rx=0;
				Px=0;
			}
			break;
		case 0X00100000:
			Rx=1;
			if((this->CPUID&SCB_CPUID_REVISION_Msk)==1)
			{
				Px=1;
			}
			break;
		case 0X00200000:
			Rx=2;
			if((this->CPUID&SCB_CPUID_REVISION_Msk)==0)
			{
				Px=0;
			}
			break;
		default:
			Px=9;
			Rx=9;
			break;
	}
	
	
    printf("STD_Embedded_Team::STD0801 Code:Demo Ver:0.0.6113 Build:%s\n", __DATE__);
    printf("STDOS::%s %dMHz Flash:%dk RAM:%dk\n", CPUName->GetBuffer(),this->Clock/1000000, this->FlashSize, this->RAMSize);
    printf("DevID:0X%04X RevID:0X%04X\n", this->DevID, this->RevID);
    printf("CPUID:0X%X ARM:ARMv7-M Cortex-M3: R%dp%d\n", this->CPUID,Rx,Px);
	#if 0
    printf("Heap :(0X%X, 0X%X) = 0X%X (%dk)\n", (uint) &__heap_base, (uint) &__heap_limit, HeapSize, HeapSize / 1024);
    printf("Stack:(0X%X, 0X%X) = 0X%X (%dk)\n", (uint) &__heap_limit, (uint) &__initial_sp, StackSize, StackSize / 1024);
	#endif
    printf("ChipType:0x42455633 3\n");
    printf("ChipID:%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\n", ID[0], ID[1], ID[2], ID[3], ID[4], ID[5], ID[6], ID[7], ID[8], ID[9], ID[10], ID[11]);
    printf("Time : ");
    DateTime dt;
    dt.Show();

    printf("Support: http://www.armku.com\n");
}

#define delay_ostickspersec 1000			//时钟频率
byte fac_us = 0; //每个us需要的systick时钟数 

TSys Sys; //系统参数
UInt64 TicksPerms = 0;

//初始化
//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void TSys::Init()
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //选择外部时钟  HCLK/8
    fac_us = SystemCoreClock / 8000000 * 8; //为系统时钟的1/8 //非OS下,代表每个us需要的systick时钟数   
    TicksPerms = SystemCoreClock / delay_ostickspersec;
    SysTick_Config(TicksPerms); //tick is 1ms	
    #ifdef STM32F1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //关闭jtag，保留swd	
	#endif

    NVIC_SetPriority(SysTick_IRQn, 0);

    #ifdef STM32F0XX
        void *p = (void*)0x1FFFF7AC;
    #else 
        void *p = (void*)0x1FFFF7E8;
    #endif 
    memcpy(ID, p, ArrayLength(ID));


    this->CPUID = SCB->CPUID;
    uint MCUID = DBGMCU->IDCODE; // MCU编码。低字设备版本，高字子版本
    this->RevID = MCUID >> 16;
    this->DevID = MCUID &0x0FFF;

    #ifdef STM32F0XX
        FlashSize = *(__IO ushort*)(0x1FFFF7CC); // 容量
    #else 
        FlashSize = *(__IO ushort*)(0x1FFFF7E0); // 容量
    #endif 
   switch (this->DevID)
    {
        case 0X0307:
			CPUName = new String("STM32F103RD");
            this->RAMSize = 64;
			break;
        case 0x0410:
            CPUName = new String("STM32F103C8");
            this->RAMSize = 20;
            break;
        case 0X0414:
            CPUName = new String("STM32F103ZE");
            this->RAMSize = 64;
            break;
        case 0X0418:
            CPUName = new String("STM32F105VC");
            this->RAMSize = 64;
            break;
        case 0X0430:
			CPUName = new String("STM32F103VG");
            this->RAMSize = 768;
			break;
		#ifdef STM32F4
		case 0X0413:
			CPUName = new String("STM32F407ZG");
            this->RAMSize = 192;
			break;
		#endif
		default:
            CPUName = new String("未知");
            this->RAMSize = 0;
			break;
    }
//    this->Inited = 1;
}

SmartIRQ::SmartIRQ(bool enable)
{    
	TInterrupt::GlobalDisable();
}

SmartIRQ::~SmartIRQ()
{
    TInterrupt::GlobalEnable();
}
