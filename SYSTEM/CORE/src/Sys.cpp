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

#define delay_ostickspersec 1000			//时钟频率
static byte fac_us = 0; //us延时倍乘数
//static uint16_t fac_ms = 0;							//ms延时倍乘数,在ucos下,代表每个节拍的ms数

#ifdef __cplusplus
    extern "C"
    {
    #endif 
    void delay_us(uint nus);
    #ifdef __cplusplus
    }
#endif 

TSys Sys; //系统参数
TTime Time; //系统时间，不建议用户直接使用
TaskScheduler *_Scheduler; //任务调度


TSys::TSys(uint clock, COM_Def messagePort)
{
    this->Clock = clock;
    this->MessagePort = messagePort;
}

void TSys::Show(bool newLine)const{

}

//初始化
//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void TSys::Init()
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //选择外部时钟  HCLK/8
    fac_us = SystemCoreClock / 8000000 * 8; //为系统时钟的1/8
    //fac_ms = (uint16_t)fac_us * 1000;					//非OS下,代表每个ms需要的systick时钟数
    SysTick_Config(SystemCoreClock / delay_ostickspersec); //tick is 1ms	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //关闭jtag，保留swd	
    this->FlashSize = *(uint16_t*)(0X1FFFF7E0); // 容量

    for (int i = 0; i < 12; i++)
    {
        this->ID[i] = *(byte*)(0X1FFFF7E8 + i);
    }
    this->FlashSize = *(ushort*)(0X1FFFF7E0);
}

uint TSys::AddTask(Action func, void *param, uint dueTime, int period, const char *name)
{
    // 屏蔽中断，否则可能有线程冲突
    SmartIRQ irq;
    if (!_Scheduler)
    {
        _Scheduler = new TaskScheduler("系统");
    }

    return _Scheduler->Add(func, param, dueTime, period);
}

//间隔1ms调用一次
void TSys::TimeTick()
{
    Time.mCurrent++;
}

//启动系统任务调度，该函数内部为死循环。*在此之间，添加的所有任务函数将得不到调度，所有睡眠方法无效！
void TSys::Start()
{
    _Scheduler->Start();
}

//运行  
void TSys::Routin(){

}

//设置任务参数
void TSys::SetTask(uint taskid, bool onoff, int delayms){

}

//显示系统信息
void TSys::ShowInfo()
{
    printf("STD_Embedded_Team::STD0801 Code:0801 Ver:0.0.6113 Build:2016-01-01\n");
    printf("STDOS::STM32F103C8 72MHz Flash:%dk RAM:20k\n", this->FlashSize);
    printf("DevID:0x0414 RevID:0x1309\n");
    printf("CPUID:0x412fc231 ARM:ARMv7-M Cortex-M3: R1p2\n");
    printf("Heap :(0x20000720, 0x20010000) = 0xf8e0 (62k)\n");
    printf("Stack:(0x20001720, 0x20010000) = 0xe8e0 (58k)\n");
    printf("ChipType:0x42455633 3\n");
    printf("ChipID:%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\n", ID[0], ID[1], ID[2], ID[3], ID[4], ID[5], ID[6], ID[7], ID[8], ID[9], ID[10], ID[11]);
    printf("Time : 2016-12-28 10:56:32\n");
    printf("Support: http://www.armku.com\n");
}

//系统启动以来的毫秒数，无符号长整型8字节
UInt64 TSys::Ms()
{
    return Time.Current();
}

//系统绝对UTC时间，整型4字节，Unix格式，1970年以来的总秒数。
uint TSys::Seconds()
{
    return Time.Seconds();
}

//微妙级延迟，常用于高精度外设信号控制
void TSys::Delay(uint us)
{
    delay_us(us);
}

//毫秒级睡眠，常用于业务层杂宁等待一定时间
void TSys::Sleep(uint ms)
{
    while (ms--)
    {
        this->Delay(1000);
    }
}

//异步热重启系统。延迟一定毫秒数执行。
void TSys::Reboot(uint msDelay){}
//删除任务
void TSys::Remove(uint taskid){

}

#ifdef __cplusplus
    extern "C"
    {
    #endif 

    //systick中断服务函数,使用ucos时用到
    void SysTick_Handler(void)
    {
        Sys.TimeTick();
    }

    //延时nus
    //nus为要延时的us数.
    void delay_us(uint nus)
    {
        uint ticks;
        uint told, tnow, tcnt = 0;
        uint reload = SysTick->LOAD; //LOAD的值
        ticks = nus * fac_us; //需要的节拍数
        tcnt = 0;
        told = SysTick->VAL; //刚进入时的计数器值
        while (1)
        {
            tnow = SysTick->VAL;
            if (tnow != told)
            {
                if (tnow < told)
                    tcnt += told - tnow;
                //这里注意一下SYSTICK是一个递减的计数器就可以了.
                else
                    tcnt += reload - tnow + told;
                told = tnow;
                if (tcnt >= ticks)
                    break;
                //时间超过/等于要延迟的时间,则退出.
            }
        };
    }
    //以下为汇编函数
    void WFI_SET(void); //执行WFI指令
    void INTX_DISABLE(void); //关闭所有中断
    void INTX_ENABLE(void); //开启所有中断
    void MSR_MSP(uint addr); //设置堆栈地址
    //THUMB指令不支持汇编内联
    //采用如下方法实现执行汇编指令WFI  
    void WFI_SET(void)
    {
        __ASM volatile("wfi");
    }

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
    __asm void MSR_MSP(uint addr)
    {
        MSR MSP, r0  //set Main Stack value
        BX r14
    }
    #ifdef __cplusplus
    }
#endif
