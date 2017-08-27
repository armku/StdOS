#include "RTC.h"
#include "TTime.h"

#ifdef STM32F0
    #include "stm32f0xx.h"
#elif defined STM32F1
    #include "stm32f10x.h"
#elif defined STM32F4
    #include "stm32f4xx.h"
#endif 

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

    #ifdef STM32F0
    #elif defined STM32F1
        /* Configure one bit for preemption priority */
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    #elif defined STM32F4
        /* Configure one bit for preemption priority */
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    #endif 
    /* Enable the RTC Interrupt */
    #ifdef STM32F0
    #elif defined STM32F1
        NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    #elif defined STM32F4
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    #endif 
    NVIC_Init(&NVIC_InitStructure);
    /* Enable PWR and BKP clocks */
    #ifdef STM32F0
    #elif defined STM32F1
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    #elif defined STM32F4

    #endif 
    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    #ifdef STM32F0
    #elif defined STM32F1	
        BKP_DeInit();
    #elif defined STM32F4
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
    #ifdef STM32F0
    #elif defined STM32F1
        RTC_WaitForLastTask();
    #elif defined STM32F4
    #endif 

    /* Enable the RTC Second */
    #ifdef STM32F0
    #elif defined STM32F1
        RTC_ITConfig(RTC_IT_SEC, ENABLE);

        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();

        /* Set RTC prescaler: set RTC period to 1sec */
        RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */

        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
    #elif defined STM32F4
    #endif 
}
int HardRTC::Sleep(int ms)
{
    return 0;
}
void HardRTC::LoadTime()
{
    uint totalSeconds = 0;
    #ifdef STM32F0
    #elif defined STM32F1
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
    #elif defined STM32F4
    #endif 
}
void HardRTC::SaveTime()
{
    uint seconds = Time.Seconds + Time.BaseSeconds;
    #ifdef STM32F0
    #elif defined STM32F1
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //使能PWR和BKP外设时钟  
        PWR_BackupAccessCmd(ENABLE); //使能RTC和后备寄存器访问 
        RTC_SetCounter(seconds); //设置RTC计数器的值

        RTC_WaitForLastTask(); //等待最近一次对RTC寄存器的写操作完成  
    #elif defined STM32F4
    #endif 
}











