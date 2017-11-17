/*
系统能自动识别常见芯片类型、主频、Flash大小、Ram大小
Sys.ID 是12字节芯片唯一标识、也就是ChipID，同一批芯片仅前面几个字节不同
毫秒级睡眠期间，系统将会安排执行其他耗时较短的任务。如果没有可用任务，系统将会进入低功耗模式，以节省能耗

 */
#include "TTime.h"
#include "Task.h"
#include <string.h>
#include "TInterrupt.h"
#include "SerialPort.h"
#include "Sys.h"
#include "stdarg.h"

TSys Sys; //系统参数

//外部注册函数
// 任务
// 任务类
extern TaskScheduler *_Scheduler;
void TimeSleep(uint us)
{
    // 在这段时间里面，去处理一下别的任务
    if (_Scheduler && (us >= 1000))
    {
        // 记录当前正在执行任务
        Task *task = _Scheduler->Current;

        UInt64 start = Time.Current() *1000;
        // 1ms一般不够调度新任务，留给硬件等待
        UInt64 end = start + us - 1000;
        // 如果休眠时间足够长，允许多次调度其它任务
        int cost = 0;
        while (true)
        {
            UInt64 start2 = Time.Current() *1000;
            bool bb = false;
            _Scheduler->Execute(us, bb);

            UInt64 now = Time.Current() *1000;
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

        cost = (int)(Time.Current() *1000-start);
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
    this->MessagePort = COM1;

    //this->Config=g_config;
    this->OnInit();
    this->MessagePort = COM1;
    this->Name = "stdos";
    this->Company = "armku";
    this->Code = 0x0201;
    this->Ver = 0x00;
    this->ID[0] = 0;
    this->DevID = 0x00;
    this->RevID = 0x00;
    this->CPUID = 0x00;

    Interrupt.Init();
    this->FlashSize = 0x01;
    this->RAMSize = 0x01;
    this->OnSleep = NULL;
}

// 初始化系统
void TSys::Init()
{
    this->InitClock();
    Time.Init();
}

void TSys::ShowInfo()const
{
    printf("%s::%s Code:%04X %s \r\n", "stdos", "std", 12, "222");
    printf("Build:%s %s\r\n", "armku", "yyyy-MM-dd HH:mm:ss");

    this->OnShowInfo();
    printf("ChipID:%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\r\n", ID[0], ID[1], ID[2], ID[3], ID[4], ID[5], ID[6], ID[7], ID[8], ID[9], ID[10], ID[11]);
    printf("Time : ");
    DateTime dt(Time.Seconds + Time.BaseSeconds);
    dt.Show(true);

    printf("Support: http://www.armku.com\r\n");
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
    if (this->OnSleep)
    {
        this->OnSleep(ms);
    }
    else
    {
        if (ms > 1000)
            SmartOS_printf("Sys::Sleep 设计错误，睡眠%dms太长，超过1000ms建议使用多线程Thread！", ms);
        if (ms)
        {
            bool cancel = false;
            int executems = Task::Scheduler()->ExecuteForWait(ms, cancel);
            if (executems >= ms)
                return ;
            ms -= executems;
        }
        if (ms)
            Time.Sleep(ms, nullptr);
    }
    return ;
    //    TimeSleep(ms *1000);
}

// 微秒级延迟
void TSys::Delay(int us)const
{
    if (this->OnSleep && us >= 2000)
    {
        this->OnSleep((us + 500) / 1000);
    }
    else
    {
        if (us > 1000000)
            SmartOS_printf("Sys::Sleep 设计错误，睡眠%dus太长，超过1000ms建议使用多线程Thread！", us);
        if (us && us >= 1000)
        {
            bool cancle = false;
            int executeus = Task::Scheduler()->ExecuteForWait(us, cancle) *1000;
            if (executeus >= us)
                return ;
            us -= executeus;
        }
        if (us)
            Time.Delay(us);
    }
    return ;
    //    TimeSleep(us);
}

// 延迟异步重启
void TSys::Reboot(int msDelay)const
{
    if (msDelay <= 0)
        this->Reset();
    //this->AddTask(&this->Reset, this, pmsDelay,  - 1, "Reboot");
}

// 系统跟踪
void TSys::InitTrace(void *port)const
{
    //*(_DWORD *)trace = a2;
}

void TSys::Trace(int times)const{

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
    this->OnStart();
    //this->=SmartOS_printf;
    Task::Scheduler()->Start();
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int SmartOS_Log(const String *str);
extern "C"
{
    /* 重载fputc可以让用户程序使用printf函数 */
    int fputc(int ch, FILE *f)
    {
        char buf[1];
        buf[0] = ch;
        String str(buf, 1);
        SmartOS_Log(&str);
        return ch;
    }

    /*
     * 函数名：itoa
     * 描述  ：将整形数据转换成字符串
     * 输入  ：-radix =10 表示10进制，其他结果为0
     *         -value 要转换的整形数
     *         -buf 转换后的字符串
     *         -radix = 10
     * 输出  ：无
     * 返回  ：无
     * 调用  ：被USART2_printf()调用
     */
    static char *itoa(int value, char *string, int radix)
    {
        int i, d;
        int flag = 0;
        char *ptr = string;

        /* This implementation only works for decimal numbers. */
        if (radix != 10)
        {
            *ptr = 0;
            return string;
        }

        if (!value)
        {
            *ptr++ = 0x30;
            *ptr = 0;
            return string;
        }

        /* if this is a negative value insert the minus sign. */
        if (value < 0)
        {
            *ptr++ = '-';

            /* Make the value positive. */
            value *=  - 1;

        }

        for (i = 10000; i > 0; i /= 10)
        {
            d = value / i;

            if (d || flag)
            {
                *ptr++ = (char)(d + 0x30);
                value -= (d *i);
                flag = 1;
            }
        }

        /* Null terminate the string. */
        *ptr = 0;

        return string;

    } /* NCL_Itoa */
    /*
     * 函数名：USART2_printf
     * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
     * 输入  ：-USARTx 串口通道，这里只用到了串口2，即USART2
     *		     -Data   要发送到串口的内容的指针
     *			   -...    其他参数
     * 输出  ：无
     * 返回  ：无 
     * 调用  ：外部调用
     *         典型应用USART2_printf( USART2, "\r\n this is a demo \r\n" );
     *            		 USART2_printf( USART2, "\r\n %d \r\n", i );
     *            		 USART2_printf( USART2, "\r\n %s \r\n", j );
     */
    int SmartOS_printf1(const char *Data, ...)
    {
		static char buf[1024];
		const int len=1024;
        const char *s;
        int d;
        char buft[16];

        int pos = 0;


        va_list ap;
        va_start(ap, Data);

        while (*Data != 0)
        // 判断是否到达字符串结束符
        {
            if (*Data == 0x5c)
            //'\'
            {
                switch (*++Data)
                {
                    case 'r':
                        //回车符
                        buf[pos++] = 0x0d;
                        Data++;
                        break;
                    case 'n':
                        //换行符
                        buf[pos++] = 0x0a;
                        Data++;
                        break;
                    default:
                        Data++;
                        break;
                }
            }
            else if (*Data == '%')
            {
                //
                switch (*++Data)
                {
                    case 's':
                        //字符串
                        s = va_arg(ap, const char*);
                        for (;  *s; s++)
                        {
                            buf[pos++] =  *s;
                            if (pos >= len)
                                goto SmartOS_printfret;
                        }
                        Data++;
                        break;
                    case 'd':
                        //十进制
                        d = va_arg(ap, int);
                        itoa(d, buft, 10);
                        for (s = buft;  *s; s++)
                        {
                            buf[pos++] =  *s;
                        }
                        Data++;
                        break;
                    case 'u':
                        //十进制
                        d = va_arg(ap, int);
                        itoa(d, buft, 10);
                        for (s = buft;  *s; s++)
                        {
                            buf[pos++] =  *s;
                        }
                        Data++;
                        break;
                    default:
                        Data++;
                        break;
                }
            }
            else
                buf[pos++] =  *Data++;
            if (pos >= len)
                return pos;
        }
		SmartOS_printfret:		
		String str(buf, pos);
        SmartOS_Log(&str);
		return pos;
    }
    int SmartOS_printf(const char *format, ...)
    {
        static char sprint_buf[1024];
        va_list args;

        int n;
        va_start(args, format);
        n = vsprintf(sprint_buf, format, args);
        va_end(args);
        char buf[1];
        for (int i = 0; i < n; i++)
        {
            buf[0] = sprint_buf[i];
            String str(buf, 1);
            SmartOS_Log(&str);
        }
        return n;
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
