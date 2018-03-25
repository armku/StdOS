#include "RTC.h"
#include "TTime.h"
#include "Platform\stm32.h"

int AlarmHandler(void *a2, int a3, int a4)
{
	return 0;
}
int RTC_Configuration(int a1, int a2)
{
	return 0;
}
int SYSCLKConfig_STOP(void)
{
	return 0;
}
bool RTC_WaitForLastTask2(unsigned int a1)
{
	return false;
}
uint32_t HardRTC::ReadBackup(uint8_t addr)
{
    return 0;
}

void HardRTC::WriteBackup(uint8_t addr, uint32_t value)
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
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Enable PWR and BKP clocks */
    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    
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
    
    /* Enable the RTC Second */     
}
int HardRTC::Sleep(int ms)
{
    return 0;
}
void HardRTC::LoadTime()
{
//    uint32_t totalSeconds = 0;     
}
void HardRTC::SaveTime()
{
//    uint32_t seconds = Time.Seconds + Time.BaseSeconds;    
}
//设置时间
void HardRTC::SetTime(uint32_t seconds)
{

}

#ifdef __cplusplus
/* 秒中断标志，进入秒中断时置1，当时间被刷新之后清0 */
__IO uint32_t TimeDisplay = 0;
    extern "C"
    {
    #endif 
    void RTC_IRQHandler(void)
    {        
    }
    #ifdef __cplusplus
    }
#endif 
