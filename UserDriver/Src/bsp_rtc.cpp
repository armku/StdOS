#include "bsp_rtc.h"
#include <stdio.h>

/* ���жϱ�־���������ж�ʱ��1����ʱ�䱻ˢ��֮����0 */
__IO uint32_t TimeDisplay = 0;

/*
 * ��������NVIC_Configuration
 * ����  ������RTC���жϵ����ж����ȼ�Ϊ1�������ȼ�Ϊ0
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
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
 * ��������RTC_CheckAndConfig
 * ����  ����鲢����RTC
 * ����  �����ڶ�ȡRTCʱ��Ľṹ��ָ��
 * ���  ����
 * ����  ���ⲿ����
 */
void StmRtc::RTC_CheckAndConfig(struct rtc_time *tm)
{
    /*������ʱ��鱸�ݼĴ���BKP_DR1��������ݲ���0xA5A5,
    ������������ʱ�䲢ѯ���û�����ʱ��*/
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
        /*��������������ʱ��*/
        /*����Ƿ��������*/
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
            printf("\r\n\r\n Power On Reset occurred....");
        }
        /*����Ƿ�Reset��λ*/
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
            printf("\r\n\r\n External Reset occurred....");
        }

        printf("\r\n No need to configure RTC....");

        /*�ȴ��Ĵ���ͬ��*/
        RTC_WaitForSynchro();

        /*����RTC���ж�*/
        RTC_ITConfig(RTC_IT_SEC, ENABLE);

        /*�ȴ��ϴ�RTC�Ĵ���д�������*/
        RTC_WaitForLastTask();
    }
    /*������ʱ������꣬������У��ʱ�������PC13*/
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
 * ��������RTC_Configuration
 * ����  ������RTC
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
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
     * ��ΪRTCʱ���ǵ��ٵģ��ڻ�ʱ���Ǹ��ٵģ�����Ҫͬ��
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

//����ʱ��
void StmRtc::SetTime(uint seconds)
{
	/* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
	
	/* �޸ĵ�ǰRTC�����Ĵ������� */
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
