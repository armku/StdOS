#include "RTC.h"
#include "TTime.h"
#ifdef STM32F1
    #include "stm32f10x.h"
#endif 
#ifdef STM32F4
    #include "stm32f4xx.h"
#endif 

HardRTC tt;

HardRTC::HardRTC()
{
	this->LowPower	= true;
	this->External	= false;
}

void HardRTC::Init()
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Enable the RTC Interrupt */
    #ifdef STM32F1
        NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    #endif 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Enable PWR and BKP clocks */
    #ifdef STM32F1
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    #endif 
    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    #ifdef STM32F1	
        BKP_DeInit();
    #endif 

    /* Enable LSE */
    RCC_LSEConfig(RCC_LSE_ON);

    /* Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){}

    /* Select LSE as RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC registers synchronization 
     * 因为RTC时钟是低速的，内环时钟是高速的，所以要同步
     */
    RTC_WaitForSynchro();

    /* Wait until last write operation on RTC registers has finished */
    #ifdef STM32F1
        RTC_WaitForLastTask();
    #endif 

    /* Enable the RTC Second */
    #ifdef STM32F1
        RTC_ITConfig(RTC_IT_SEC, ENABLE);

        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();

        /* Set RTC prescaler: set RTC period to 1sec */
        RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */

        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
    #endif 
}

void HardRTC::LoadTime()
{
    #ifdef STM32F1
        Time.Seconds = RTC_GetCounter() + 8 * 60 * 60;
    #endif 
}

void HardRTC::SaveTime()
{
    uint seconds = Time.Seconds; //先这么用
    #ifdef STM32F1
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //使能PWR和BKP外设时钟  
        PWR_BackupAccessCmd(ENABLE); //使能RTC和后备寄存器访问 
        RTC_SetCounter(seconds); //设置RTC计数器的值

        RTC_WaitForLastTask(); //等待最近一次对RTC寄存器的写操作完成  
    #endif 
}

int HardRTC::Sleep(int ms)
{
    return 0;
}

uint HardRTC::ReadBackup(byte addr)
{
    return 0;
}

void HardRTC::WriteBackup(byte addr, uint value)
{
	addr=addr;
	value=value;
}

HardRTC *HardRTC::Instance()
{
    return  &tt;
}

void HardRTC::Start(bool lowpower, bool external)
{
	lowpower=lowpower;
	external=external;
}

/* 秒中断标志，进入秒中断时置1，当时间被刷新之后清0 */
__IO uint32_t TimeDisplay = 0;

#ifdef __cplusplus
    extern "C"
    {
    #endif 
    void RTC_IRQHandler(void)
    {
        #ifdef STM32F1
            if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
            {
                /* Clear the RTC Second interrupt */
                RTC_ClearITPendingBit(RTC_IT_SEC);

                /* Enable time update */
                TimeDisplay = 1;

                /* Wait until last write operation on RTC registers has finished */
                RTC_WaitForLastTask();
            }
        #endif 
    }
    #ifdef __cplusplus
    }
#endif 
//测试时钟
#if 1
    DateTime now; //当前时间

    void TimeRefresh(void *param)
    {
        HardRTC *rtc = (HardRTC*)param;
        rtc->LoadTime();
        now = Time.Seconds;
        now.Show();
    }
    void RTCtest()
    {
        tt.LowPower = false;
        tt.External = false;

        tt.LoadTime();
        now = Time.Seconds;
        if (now.TotalSeconds() < 100)
        {
            now.Year = 2017;
            now.Month = 6;
            now.Day = 17;
            now.Hour = 14;
            now.Minute = 17;

            tt.Init();
            Time.Seconds = now.TotalSeconds();
            tt.SaveTime();
        }
        Sys.AddTask(TimeRefresh, &tt, 100, 1000, "TimeUp");
    }
#endif
