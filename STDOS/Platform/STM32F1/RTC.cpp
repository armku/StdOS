#include "RTC.h"
#include "TTime.h"
#include "stm32f10x.h"

int AlarmHandler(void *a2, int a3, int a4)
{
	return 0;
}
int RTC_Configuration(int a1, int a2)
{
	
}
int SYSCLKConfig_STOP(void)
{
}
bool RTC_WaitForLastTask2(unsigned int a1)
{
	return false;
}
uint HardRTC::ReadBackup(byte addr)
{
    return 0;
}

void HardRTC::WriteBackup(byte addr, uint value)
{
    addr = addr;
    value = value;
}
void HardRTC::Init()
{
    NVIC_InitTypeDef NVIC_InitStructure;

        /* Configure one bit for preemption priority */
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    /* Enable the RTC Interrupt */
        NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Enable PWR and BKP clocks */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
        BKP_DeInit();
    
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
        RTC_WaitForLastTask();
    
    /* Enable the RTC Second */
        RTC_ITConfig(RTC_IT_SEC, ENABLE);

        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();

        /* Set RTC prescaler: set RTC period to 1sec */
        RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */

        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
}
int HardRTC::Sleep(int ms)
{
    return 0;
}
void HardRTC::LoadTime()
{
    uint totalSeconds = 0;
        totalSeconds = RTC_GetCounter();
        Time.Seconds = totalSeconds;
        if (totalSeconds > Time.BaseSeconds)
        {
            Time.Seconds = totalSeconds - Time.BaseSeconds;
        }
        else
        {
            Time.Seconds = 0;
        } 
}
void HardRTC::SaveTime()
{
    uint seconds = Time.Seconds + Time.BaseSeconds;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //使能PWR和BKP外设时钟  
        PWR_BackupAccessCmd(ENABLE); //使能RTC和后备寄存器访问 
        RTC_SetCounter(seconds); //设置RTC计数器的值

        RTC_WaitForLastTask(); //等待最近一次对RTC寄存器的写操作完成
}

#ifdef __cplusplus
/* 秒中断标志，进入秒中断时置1，当时间被刷新之后清0 */
__IO uint32_t TimeDisplay = 0;
    extern "C"
    {
    #endif 
    void RTC_IRQHandler(void)
    {
            if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
            {
                /* Clear the RTC Second interrupt */
                RTC_ClearITPendingBit(RTC_IT_SEC);

                /* Enable time update */
                TimeDisplay = 1;

                /* Wait until last write operation on RTC registers has finished */
                RTC_WaitForLastTask();
            } 
    }
    #ifdef __cplusplus
    }
#endif 
