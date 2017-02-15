/*
系统能自动识别常见芯片类型、主频、Flash大小、Ram大小
Sys.ID 是12字节芯片唯一标识、也就是ChipID，同一批芯片仅前面几个字节不同
毫秒级睡眠期间，系统将会安排执行其他耗时较短的任务。如果没有可用任务，系统将会进入低功耗模式，以节省能耗

 */
#include "stdio.h"
#include "Sys.h"
#include "TTime.h"
#include "stm32f10x.h"
#include "TaskScheduler.h"
#include "SmartIrq.h"
#include <string.h>
#include "Array.h"
#include "DateTime.h"

//外部注册函数
// 任务
#include "Task.h"
// 任务类
TaskScheduler *_Scheduler;

// 创建任务，返回任务编号。priority优先级，dueTime首次调度时间us，period调度间隔us，-1表示仅处理一次
uint TSys::AddTask(Action func, void *param, ulong dueTime, long period, const char *name)
{
    // 屏蔽中断，否则可能有线程冲突
    SmartIRQ irq;

    if (!_Scheduler)
    {
        _Scheduler = new TaskScheduler("Sys");
    }

    return _Scheduler->Add(func, param, dueTime*1000, period*1000, name);
}

void TSys::RemoveTask(uint taskid)
{
    #if 0
        assert_ptr(_Scheduler);
    #endif 
    _Scheduler->Remove(taskid);
}

void TSys::SetTask(uint taskid, bool enable)
{
    Task *task = (*_Scheduler)[taskid];
    if (task)
    {
        task->Enable = enable;
    }
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
    // 在这段时间里面，去处理一下别的任务
    if (_Scheduler && (us >= 1000))
    {
        // 记录当前正在执行任务
        Task *task = _Scheduler->Current;

        ulong start = Time.Current();
        // 1ms一般不够调度新任务，留给硬件等待
        ulong end = start + us - 1000;
        // 如果休眠时间足够长，允许多次调度其它任务
        int cost = 0;
        while (true)
        {
            ulong start2 = Time.Current();

            _Scheduler->Execute(us);

            ulong now = Time.Current();
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

//毫秒级睡眠，常用于业务层杂宁等待一定时间
void TSys::Sleep(uint ms)
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

//微妙级延迟，常用于高精度外设信号控制
void TSys::Delay(uint us)
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

TSys::TSys(uint clock, COM_Def messagePort)
{
    this->Clock = clock;
    this->MessagePort = messagePort;
}

void TSys::Show(bool newLine)const{

}
//系统启动以来的毫秒数，无符号长整型8字节
UInt64 TSys::Ms()
{
    return Time.Ms();
}

//系统绝对UTC时间，整型4字节，Unix格式，1970年以来的总秒数。
uint TSys::Seconds()
{
    return Time.Seconds();
}

//异步热重启系统。延迟一定毫秒数执行。
void TSys::Reboot(uint msDelay){}
//显示系统信息
void TSys::ShowInfo()
{
    printf("STD_Embedded_Team::STD0801 Code:Demo Ver:0.0.6113 Build:%s\n",__DATE__);
    printf("STDOS::%s 72MHz Flash:%dk RAM:%dk\n", this->CPUName->GetBuffer(), this->FlashSize, this->RamSize);
    printf("DevID:0X%04X RevID:0X%04X\n", this->DevID, this->RevID);
    printf("CPUID:0X%X ARM:ARMv7-M Cortex-M3: R1p2\n", this->CPUID);
    printf("Heap :(0x20000720, 0x20010000) = 0xf8e0 (62k)\n");
    printf("Stack:(0x20001720, 0x20010000) = 0xe8e0 (58k)\n");
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
UInt64 TicksPerms=0; 

//初始化
//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void TSys::Init()
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //选择外部时钟  HCLK/8
    fac_us = SystemCoreClock / 8000000 * 8; //为系统时钟的1/8 //非OS下,代表每个us需要的systick时钟数   
	TicksPerms=SystemCoreClock / delay_ostickspersec;
    SysTick_Config(TicksPerms); //tick is 1ms	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //关闭jtag，保留swd	


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

    this->_Index = 0;

    #ifdef STM32F0XX
        FlashSize = *(__IO ushort*)(0x1FFFF7CC); // 容量
    #else 
        FlashSize = *(__IO ushort*)(0x1FFFF7E0); // 容量
    #endif 
    this->Initjs();
    this->Inited = 1;
}

//计算ram、型号等
void TSys::Initjs()
{    
    switch (this->DevID)
    {
        case 0x0410:
            this->CPUName=new String("STM32F103C8");
			this->RamSize=20;
            break;
		case 0X0414:
            this->CPUName=new String("STM32F103ZE");
			this->RamSize=64;
            break;
		case 0x418:
			this->CPUName=new String("STM32F105VC");
			this->RamSize=64;
            break;
		default:
			this->CPUName=new String("未知");
			break;
    }
}

#if 0

    extern uint __heap_base;
    extern uint __heap_limit;

    void *operator new(uint size)
    {
        debug_printf(" new size: %d ", size);
        void *p = malloc(size);
        if (!p)
            debug_printf("malloc failed! size=%d ", size);
        else
        {
            debug_printf("0x%08x ", p);
            // 如果堆只剩下64字节，则报告失败，要求用户扩大堆空间以免不测
            uint end = (uint) &__heap_limit;
            if ((uint)p + size + 0x40 >= end)
                debug_printf(" + %d near HeapEnd=0x%08x", size, end);
        }
        assert_param(p);
        return p;
    }

    void *operator new[](uint size)
    {
        debug_printf(" new size[]: %d ", size);
        void *p = malloc(size);
        if (!p)
            debug_printf("malloc failed! size=%d ", size);
        else
        {
            debug_printf("0x%08x ", p);
            // 如果堆只剩下64字节，则报告失败，要求用户扩大堆空间以免不测
            uint end = (uint) &__heap_limit;
            if ((uint)p + size + 0x40 >= end)
                debug_printf(" + %d near HeapEnd=0x%08x", size, end);
        }
        assert_param(p);
        return p;
    }

    void operator delete (void *p)
    {
        debug_printf(" delete 0x%08x ", p);
        if (p)
            free(p);
    }

    void operator delete [](void *p)
    {
        debug_printf(" delete[] 0x%08x ", p);
        if (p)
            free(p);
    }
#endif
