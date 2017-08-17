#include "TTime.h"
#include "Device\RTC.h"
#include "Device\Timer.h"

#ifdef STM32F0
    #include "stm32f0xx.h"
#elif defined STM32F1
    #include "stm32f10x.h"
#elif defined STM32F4
    #include "stm32f4xx.h"
#endif 

//extern byte fac_us; //每个us需要的systick时钟数 
TTime Time; //系统时间，不建议用户直接使用
//extern UInt64 TicksPerms;

void TTime::OnHandler(ushort num, void *param){

}

TTime::TTime()
{
    this->BaseSeconds = 0;
    this->Seconds = 0;
}

void RtcRefresh(void *param)
{
    HardRTC *rtc = (HardRTC*)param;
    rtc->LoadTime();
}

// 使用RTC，必须在Init前调用
void TTime::UseRTC()
{
    HardRTC *rtc = HardRTC::Instance();
    rtc->LowPower = false;
    rtc->External = false;

    rtc->Init();
    Sys.AddTask(RtcRefresh, rtc, 100, 100, "Rtc");
}

extern uint time6cnt;
Timer *timerTick;
Delegate < Timer & > abc;
void timTickrefesh(void *param)
{
    time6cnt++;
    Time.Milliseconds++;
}

static byte fac_us = 0; //us延时倍乘数			   
static ushort fac_ms = 0; //ms延时倍乘数,在ucos下,代表每个节拍的ms数
void TTime::Init()
{

    // 初始化为输出
    #if defined STM32F0
        timerTick = new Timer(Timer7);
    #elif defined STM32F1
        timerTick = new Timer(Timer6);
    #elif defined STM32F4
        timerTick = new Timer(Timer3);
    #endif 
    abc.Bind(timTickrefesh);
    timerTick->Register(abc);
    timerTick->Open();
    timerTick->SetFrequency(1000);
    //        timer2->Config();

    //初始化延迟函数
    //SYSTICK的时钟固定为HCLK时钟的1/8
    //SYSCLK:系统时钟
    #if defined STM32F0
    #elif defined STM32F1
        SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //选择外部时钟  HCLK/8
        fac_us = SystemCoreClock / 8000000; //为系统时钟的1/8  
        fac_ms = (ushort)fac_us *1000; //代表每个ms需要的systick时钟数   
    #elif defined STM32F4
        uint SYSCLK = 168;
        SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
        fac_us = SYSCLK / 8; //不论是否使用OS,fac_us都需要使用

        fac_ms = (u16)fac_us *1000; //非OS下,代表每个ms需要的systick时钟数   
    #endif 
}

// 当前滴答时钟
uint TTime::CurrentTicks()const
{
    return 0;
}

// 当前毫秒数
UInt64 TTime::Current()const
{
    UInt64 ret = 0;
    #if defined(STM32F1) || defined(STM32F4)	
        ret = this->Milliseconds *1000; //先转换为us
        //        ret += (TicksPerms - SysTick->VAL) / fac_us;
    #elif defined STM32F0
        ret = this->Milliseconds *1000; //先转换为us
        //        ret += (TicksPerms - SysTick->VAL) / fac_us;
    #endif 
    //return this->mCurrent *1000;
    return ret;
}

// 设置时间
void TTime::SetTime(UInt64 seconds){

}
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void TTime::Sleep(int nms, bool *running)const
{
    #if 1
        this->Delay(nms *1000);
    #else 
        #if defined STM32F1
            uint temp;
            SysTick->LOAD = (u32)nms *fac_ms; //时间加载(SysTick->LOAD为24bit)
            SysTick->VAL = 0x00; //清空计数器
            SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开始倒数  
            do
            {
                temp = SysTick->CTRL;
            }
            while ((temp &0x01) && !(temp &(1 << 16))); //等待时间到达   
            SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭计数器
            SysTick->VAL = 0X00; //清空计数器	 
        #elif defined STM32F4
            //SysTick->LOAD为24位寄存器,所以,最大延时为:
            //nms<=0xffffff*8*1000/SYSCLK
            //SYSCLK单位为Hz,nms单位为ms
            //对168M条件下,nms<=798ms 
            uint temp;
            SysTick->LOAD = (u32)nms *fac_ms; //时间加载(SysTick->LOAD为24bit)
            SysTick->VAL = 0x00; //清空计数器
            SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开始倒数 
            do
            {
                temp = SysTick->CTRL;
            }
            while ((temp &0x01) && !(temp &(1 << 16))); //等待时间到达   
            SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭计数器
            SysTick->VAL = 0X00; //清空计数器	  	   
        #endif 
    #endif 
}

// 微秒级延迟
void TTime::Delay(int nus)const
{
    #if 0
        uint ticks;
        uint told, tnow, tcnt = 0;
        uint reload = 0;
        #if defined(STM32F1) || defined(STM32F4)
            reload = SysTick->LOAD; //LOAD的值
        #elif defined STM32F0
            reload = SysTick->LOAD; //LOAD的值
        #endif 
        ticks = nus * fac_us; //需要的节拍数
        tcnt = 0;
        #if defined(STM32F1) || defined(STM32F4)
            told = SysTick->VAL; //刚进入时的计数器值
        #elif defined STM32F0
            told = SysTick->VAL; //刚进入时的计数器值
        #endif 
        while (1)
        {
            #if defined(STM32F1) || defined(STM32F4)
                tnow = SysTick->VAL;
            #elif defined STM32F0
                tnow = SysTick->VAL;
            #endif 
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
        }
    #else 
        #if defined STM32F0
            uint temp;
            SysTick->LOAD = 6 * nus;
            SysTick->VAL = 0X00; //清空计数器
            SysTick->CTRL = 0X01; //使能，减到零是无动作，采用外部时钟源
            do
            {
                temp = SysTick->CTRL; //读取当前倒计数值
            }
            while ((temp &0x01) && (!(temp &(1 << 16)))); //等待时间到达

            SysTick->CTRL = 0x00; //关闭计数器
            SysTick->VAL = 0X00; //清空计数器
        #elif defined STM32F1
            uint temp;
            SysTick->LOAD = nus * fac_us; //时间加载	  		 
            SysTick->VAL = 0x00; //清空计数器
            SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开始倒数	  
            do
            {
                temp = SysTick->CTRL;
            }
            while ((temp &0x01) && !(temp &(1 << 16))); //等待时间到达   
            SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭计数器
            SysTick->VAL = 0X00; //清空计数器	
        #elif defined STM32F4
            //nus为要延时的us数.	
            //注意:nus的值,不要大于798915us(最大值即2^24/fac_us@fac_us=21)
            uint temp;
            SysTick->LOAD = nus * fac_us; //时间加载	  		 
            SysTick->VAL = 0x00; //清空计数器
            SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开始倒数 	 
            do
            {
                temp = SysTick->CTRL;
            }
            while ((temp &0x01) && !(temp &(1 << 16))); //等待时间到达   
            SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭计数器
            SysTick->VAL = 0X00; //清空计数器 
        #endif 
    #endif 
}

uint TTime::TicksToUs(uint ticks)const
{
    if (!ticks)
    {
        return 0;
    }
    //1000000 = 64 * 15625 = 2^6 * 15625
    //(0xFFFFFFFF / (1000000 >> 6)) = 274877.90688
    //如果TICKS<274877,那么乘法不会溢出
    if (ticks <= (0xffffffff / (1000000 >> 6)))
    {
        return (ticks *(1000000 >> 6)) >> (15-6);
    }
    else
    {
        return ((UInt64)ticks *(1000000 >> 6)) >> (16-6);
    }
}

uint TTime::UsToTicks(uint us)const
{
    if (!us)
    {
        return 0;
    }

    //1000000 = 64 * 15625 = 2^6 * 15625
    //((0xffffffff +1) >> (15-6)) = 8388608 = 0x800000 = 1 << (32 - 9)
    //如果ticks<274877,那么乘法不会溢出
    if (us < (1 << (32-(15-6))))
    {
        return (us << (15-6)) / (1000000 >> 6);
    }
    else
    {
        return ((UInt64)us << (15-6)) / (1000000 >> 6);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
TimeWheel::TimeWheel(uint ms){

}

void TimeWheel::Reset(uint ms){

}
// 是否已过期
bool TimeWheel::Expired()
{
    return true;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
TimeCost::TimeCost()
{
    this->Start = 0;
}

void TimeCost::Reset(){}
// 逝去的时间，微秒
int TimeCost::Elapsed()const
{
    uint ret = 12;
    ret -= this->Start;
    return ret;
}

void TimeCost::Show(cstring format)const
{
    printf("执行 %d 微妙\r\n", 12);
}




#ifdef __cplusplus
    extern "C"
    {
    #endif 

    //以下为汇编函数
    //THUMB指令不支持汇编内联
    //采用如下方法实现执行汇编指令WFI  
    void WFI_SET(void)
    {
        #if defined(STM32F1) || defined(STM32F4)
            __ASM volatile("wfi");
        #elif defined STM32F0
            __ASM volatile("wfi");
        #endif 
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
