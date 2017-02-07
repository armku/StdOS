#include "bsp_rtc.h"
#include <stdio.h>

/* 秒中断标志，进入秒中断时置1，当时间被刷新之后清0 */
__IO uint32_t TimeDisplay = 0;

/*
 * 函数名：NVIC_Configuration
 * 描述  ：配置RTC秒中断的主中断优先级为1，次优先级为0
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
 void StmRtc::RTC_NVIC_Config(void)
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
}


/*
 * 函数名：RTC_CheckAndConfig
 * 描述  ：检查并配置RTC
 * 输入  ：用于读取RTC时间的结构体指针
 * 输出  ：无
 * 调用  ：外部调用
 */
void StmRtc::RTC_CheckAndConfig(struct rtc_time *tm)
{
    /*在启动时检查备份寄存器BKP_DR1，如果内容不是0xA5A5,
    则需重新配置时间并询问用户调整时间*/
    if (BKP_ReadBackupRegister(BKP_DR1) != 0x2234)
    {        
        RTC_Configuration();
        
       
		tm->tm_year = 1970;
    tm->tm_mon = 1;
    tm->tm_mday = 1;
    tm->tm_hour = 8;
    tm->tm_min = 0;
    tm->tm_sec = 0;
		this->SetTime(mktimev(tm));
		
        BKP_WriteBackupRegister(BKP_DR1, 0x2234);
    } 
    else
    {
        /*启动无需设置新时钟*/
        /*检查是否掉电重启*/
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
            printf("\r\n\r\n Power On Reset occurred....");
        }
        /*检查是否Reset复位*/
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
            printf("\r\n\r\n External Reset occurred....");
        }

        printf("\r\n No need to configure RTC....");

        /*等待寄存器同步*/
        RTC_WaitForSynchro();

        /*允许RTC秒中断*/
        RTC_ITConfig(RTC_IT_SEC, ENABLE);

        /*等待上次RTC寄存器写操作完成*/
        RTC_WaitForLastTask();
    }
    /*定义了时钟输出宏，则配置校正时钟输出到PC13*/
    #ifdef RTCClockOutput_Enable
        /* Enable PWR and BKP clocks */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

        /* Allow access to BKP Domain */
        PWR_BackupAccessCmd(ENABLE);

        /* Disable the Tamper Pin */
        BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
        functionality must be disabled */

        /* Enable RTC Clock Output on Tamper Pin */
        BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
    #endif 

    /* Clear reset flags */
    RCC_ClearFlag();

}

/*
 * 函数名：RTC_Configuration
 * 描述  ：配置RTC
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void StmRtc::RTC_Configuration(void)
{
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

//设置时间
void StmRtc::SetTime(uint seconds)
{
	/* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
	
	/* 修改当前RTC计数寄存器内容 */
    RTC_SetCounter(seconds);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}
#ifdef __cplusplus
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
