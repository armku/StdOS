/*
系统能自动识别常见芯片类型、主频、Flash大小、Ram大小
Sys.ID 是12字节芯片唯一标识、也就是ChipID，同一批芯片仅前面几个字节不同
毫秒级睡眠期间，系统将会安排执行其他耗时较短的任务。如果没有可用任务，系统将会进入低功耗模式，以节省能耗

 */
#include "TTime.h"
#include "Task.h"
#include <string.h>
#include "Array.h"
#include "TInterrupt.h"
#include "SerialPort.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif
TSys Sys; //系统参数
extern "C"
{
    extern uint __heap_base;
    extern uint __heap_limit;
    extern uint __initial_sp;
}

String *CPUName;
//外部注册函数
// 任务
// 任务类
TaskScheduler *_Scheduler;
void TimeSleep(uint us)
{
    // 在这段时间里面，去处理一下别的任务
    if (_Scheduler && (us >= 1000))
    {
        // 记录当前正在执行任务
        Task *task = _Scheduler->Current;

        UInt64 start = Time.Current()*1000;
        // 1ms一般不够调度新任务，留给硬件等待
        UInt64 end = start + us - 1000;
        // 如果休眠时间足够长，允许多次调度其它任务
        int cost = 0;
        while (true)
        {
            UInt64 start2 = Time.Current()*1000;
            bool bb = false;
            _Scheduler->Execute(us, bb);

            UInt64 now = Time.Current()*1000;
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

        cost = (int)(Time.Current()*1000 - start);
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


int SmartOS_printf(const char *format, ...);


void ExitCritical(void)
{
//  enable_irq();
}
void EnterCritical(void)
{
//  disable_irq();
}
//uint _REV(uint a1)
//{
//  return rev(a1);
//}

int REV16(ushort a1)
{
  return (ushort)REV16(a1);
}

// 构造函数
TSys::TSys()
{
	#ifdef STM32F0
		this->CystalClock=8;
		this->Clock = 48;
	#elif defined STM32F1
		this->CystalClock=8;
		this->Clock = 72;
	#elif defined STM32F4
		this->CystalClock=8;
		this->Clock = 168;
	#endif
    this->MessagePort = COM1;
	
	Interrupt.Init();
}
// 初始化系统时钟
void TSys::InitClock()
{
	#ifdef STM32F0
        void *p = (void*)0x1FFFF7AC;
    #elif defined STM32F1 
        void *p = (void*)0x1FFFF7E8;
    #elif defined STM32F4 
        void *p = (void*)0x1fff7a10;
    #endif 
    memcpy(ID, p, ArrayLength(ID));

    this->CPUID = SCB->CPUID;
    uint MCUID = DBGMCU->IDCODE; // MCU编码。低字设备版本，高字子版本
    this->RevID = MCUID >> 16;
    this->DevID = MCUID &0x0FFF;

    #ifdef STM32F0
        FlashSize = *(__IO ushort*)(0x1FFFF7CC); // 容量
    #elif defined STM32F1 
        FlashSize = *(__IO ushort*)(0x1FFFF7E0); // 容量
    #elif defined STM32F4 
        FlashSize = *(__IO ushort*)(0X1FFF7a22); // 容量
    #endif 
    switch (this->DevID)
    {
		#ifdef STM32F0
			case 0X0448:
				CPUName = new String("STM32F072VB");
                this->RAMSize = 64;
                break;
        #elif defined STM32F1
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
         #elif defined STM32F4
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
// 初始化系统
void TSys::Init()
{    		
	this->InitClock();
	Time.Init();
}		
void TSys::ShowInfo() const
{
	this->OnShowInfo();
}	
uint TSys::HeapBase() const	// 堆起始地址，前面是静态分配内存
{
//	return &_heap_base;
}	
uint TSys::StackTop() const	// 栈顶，后面是初始化不清零区域
{
	//return (*((_WORD *)this + 31) << 10) + 536870656;
}	
void TSys::SetStackTop(uint addr)
{
//	return _set_MSP(addr);
}	

// 系统启动后的毫秒数
UInt64 TSys::Ms()const  
{
    return Time.Current();
}	

// 系统绝对当前时间，秒
uint TSys::Seconds()const  
{
    return Time.Seconds;
	
	 //return Time + dword_6A0;
}

// 毫秒级延迟
void TSys::Sleep(int ms)const  
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
// 微秒级延迟
void TSys::Delay(int us)const  
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
bool TSys::CheckMemory() const
{
	return true;
}	

// 延迟异步重启
void TSys::Reboot(int msDelay)const
{
    this->Reset();
}
// 系统跟踪
void TSys::InitTrace(void* port) const
{
	//*(_DWORD *)trace = a2;
}	
void TSys::Trace(int times) const
{

}	


// 重启系统
void TSys::Reset() const
{
	NVIC_SystemReset();
}
void TSys::OnInit()
{
}	
void TSys::OnShowInfo()const
{
    uint Rx = 0;
    uint Px = 0;

    uint HeapSize = 0;
    uint StackSize = 0;


    HeapSize = ((uint) &__heap_limit - (uint) &__heap_base);
    StackSize = ((uint) &__initial_sp - (uint) &__heap_limit);

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


    printf("STD_Embedded_Team::STD0801 Code:Demo Ver:0.0.6113 Build:%s\r\n",__DATE__);
    printf("STDOS::%s %dMHz Flash:%dk RAM:%dk\r\n", CPUName->GetBuffer(), this->Clock, this->FlashSize, this->RAMSize);
    printf("DevID:0X%04X RevID:0X%04X\r\n", this->DevID, this->RevID);
    printf("CPUID:0X%X ARM:ARMv7-M Cortex-M3: R%dp%d\r\n", this->CPUID, Rx, Px);
    printf("Heap :(0X%X, 0X%X) = 0X%X (%dk)\r\n", (uint) &__heap_base, (uint) &__heap_limit, HeapSize, HeapSize / 1024);
    printf("Stack:(0X%X, 0X%X) = 0X%X (%dk)\r\n", (uint) &__heap_limit, (uint) &__initial_sp, StackSize, StackSize / 1024);
    printf("ChipType:0x42455633 3\r\n");
    printf("ChipID:%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\r\n", ID[0], ID[1], ID[2], ID[3], ID[4], ID[5], ID[6], ID[7], ID[8], ID[9], ID[10], ID[11]);
    printf("Time : ");
    DateTime dt(Time.Seconds+Time.BaseSeconds);
    dt.Show(true);

    printf("Support: http://www.armku.com\r\n");
}
	
void TSys::OnStart()
{
	
}	

// 创建任务，返回任务编号。dueTime首次调度时间ms，period调度间隔ms，-1表示仅处理一次
uint TSys::AddTask(Action func, void *param, int dueTime, int period, cstring name)const
{
    // 屏蔽中断，否则可能有线程冲突
    SmartIRQ irq;

    if (!_Scheduler)
    {
        _Scheduler = new TaskScheduler("Sys");
    }

    return _Scheduler->Add(func, param, dueTime, period, name);

}
void TSys::RemoveTask(uint &taskid)const
{
    #if 0
        assert_ptr(_Scheduler);
    #endif 
    _Scheduler->Remove(taskid);
}
// 设置任务的开关状态，同时运行指定任务最近一次调度的时间，0表示马上调度
bool TSys::SetTask(uint taskid, bool enable, int msNextTime)const
{
    //    _Scheduler->SetTask(taskid, enable, msNextTime);
    return true;
}

// 改变任务周期
bool TSys::SetTaskPeriod(uint taskid, int period) const
{
	return false;
}
// 开始系统大循环
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
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
uint _REV(uint value)
{
	return value;
}
ushort _REV16(ushort value)
{
	return value;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
SmartIRQ::SmartIRQ(bool enable)
{
    TInterrupt::GlobalDisable();
}

SmartIRQ::~SmartIRQ()
{
    TInterrupt::GlobalEnable();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
extern SerialPort *_printf_sp;
extern "C"
{
    /* 重载fputc可以让用户程序使用printf函数 */
    int fputc(int ch, FILE *f)
    {
		static bool isInFPutc; //正在串口输出
        //        if (!Sys.Inited)
        //            return ch;

        if (Sys.MessagePort == COM_NONE)
            return ch;

        if (isInFPutc)
            return ch;
        isInFPutc = true;

        // 检查并打开串口
        //if ((port->CR1 &USART_CR1_UE) != USART_CR1_UE && _printf_sp == NULL)
        if (_printf_sp == NULL)
        {
            _printf_sp = new SerialPort(COM(Sys.MessagePort));
            _printf_sp->Open();
        }
        _printf_sp->SendData((byte)ch);
        isInFPutc = false;
        return ch;
    }
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if DEBUG
    // 函数栈。
    // 进入函数时压栈函数名，离开时弹出。便于异常时获取主线程调用列表
    TraceStack::TraceStack(cstring name){}
    TraceStack::~TraceStack(){}

    void TraceStack::Show(){}

#endif
