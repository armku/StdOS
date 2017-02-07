#include "bsp_rtc.h"
#include <stdio.h>

/* ���жϱ�־���������ж�ʱ��1����ʱ�䱻ˢ��֮����0 */
__IO uint32_t TimeDisplay = 0;

/*���ڣ���Ф������ASCII��*/
uint8_t const *WEEK_STR[] = 
{
    "��", "һ", "��", "��", "��", "��", "��"
};
uint8_t const *zodiac_sign[] = 
{
    "��", "��", "ţ", "��", "��", "��", "��", "��", "��", "��", "��", "��"
};


/*
 * ��������NVIC_Configuration
 * ����  ������RTC���жϵ����ж����ȼ�Ϊ1�������ȼ�Ϊ0
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void RTC_NVIC_Config(void)
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
void RTC_CheckAndConfig(struct rtc_time *tm)
{
    /*������ʱ��鱸�ݼĴ���BKP_DR1��������ݲ���0xA5A5,
    ������������ʱ�䲢ѯ���û�����ʱ��*/
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
    {
        printf("\r\n\r\n RTC not yet configured....");

        /* RTC Configuration */
        RTC_Configuration();

        printf("\r\n\r\n RTC configured....");

        /* Adjust time by users typed on the hyperterminal */
        Time_Adjust(tm);

        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
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
void RTC_Configuration(void)
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

/*****
 * ������  : USART_Scanf
 * ����    : ��΢�������ն˻�ȡ����ֵ(��ASCII��ת��Ϊ����)
������ר����RTC��ȡʱ�䣬��������������Ӧ�ã�Ҫ�޸�һ��
 * ����    : - value �û��ڳ����ն����������ֵ
 * ���    : �����ַ���ASCII���Ӧ����ֵ
 * ����    ���ڲ�����
 ***/

static uint8_t USART_Scanf(uint32_t value)
{
    uint32_t index = 0;
    uint32_t tmp[2] = 
    {
        0, 0
    };

    while (index < 2)
    {
        /* Loop until RXNE = 1 */
        while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET){}

        tmp[index++] = (USART_ReceiveData(USART1));
        if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
         /*����0��9��ASCII��Ϊ0x30��0x39*/
        {
            printf("\n\rPlease enter valid number between 0 and 9 -->:  ");
            index--;
        }
    }

    /* ���������ַ���ASCII��ת��Ϊ����*/
    index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) *10);

    /* Checks */
    if (index > value)
    {
        printf("\n\rPlease enter valid number between 0 and %d", value);
        return 0xFF;
    }
    return index;
}

void Time_Regulate(struct rtc_time *tm)
{
    tm->tm_year = 1970;
    tm->tm_mon = 1;
    tm->tm_mday = 1;
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;
} 

/*
 * ��������Time_Adjust
 * ����  ��ʱ�����
 * ����  �����ڶ�ȡRTCʱ��Ľṹ��ָ��
 * ���  ����
 * ����  ���ⲿ����
 */
void Time_Adjust(struct rtc_time *tm)
{
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Get time entred by the user on the hyperterminal */
    Time_Regulate(tm);

    /* Get wday */
    GregorianDay(tm);

    /* �޸ĵ�ǰRTC�����Ĵ������� */
    RTC_SetCounter(mktimev(tm));

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
} 

/*
 * ��������Time_Display
 * ����  ����ʾ��ǰʱ��ֵ
 * ����  ��-TimeVar RTC����ֵ����λΪ s
 * ���  ����
 * ����  ���ڲ�����
 */
void Time_Display(uint32_t TimeVar, struct rtc_time *tm)
{
    static uint32_t FirstDisplay = 1;
    uint32_t BJ_TimeVar;
    uint8_t str[15]; // �ַ����ݴ�  	

    /*  �ѱ�׼ʱ��ת��Ϊ����ʱ��*/
    BJ_TimeVar = TimeVar + 8 * 60 * 60;

    to_tm(BJ_TimeVar, tm); /*�Ѷ�ʱ����ֵת��Ϊ����ʱ��*/

    if ((!tm->tm_hour && !tm->tm_min && !tm->tm_sec) || (FirstDisplay))
    {

        GetChinaCalendar((u16)tm->tm_year, (u8)tm->tm_mon, (u8)tm->tm_mday, str);
        printf("\r\n ����������%0.2d%0.2d,%0.2d,%0.2d", str[0], str[1], str[2], str[3]);

        GetChinaCalendarStr((u16)tm->tm_year, (u8)tm->tm_mon, (u8)tm->tm_mday, str);
        printf("\r\n ����ũ����%s\r\n", str);

        if (GetJieQiStr((u16)tm->tm_year, (u8)tm->tm_mon, (u8)tm->tm_mday, str))
            printf("\r\n ����ũ����%s\r\n", str);

        FirstDisplay = 0;
    } 

    /* ���ʱ���������ʱ�� */
    printf(" UNIXʱ��� = %d ��ǰʱ��Ϊ: %d��(%s��) %d�� %d�� (����%s)  %0.2d:%0.2d:%0.2d\r", TimeVar, tm->tm_year, zodiac_sign[(tm->tm_year - 3) % 12], tm->tm_mon, tm->tm_mday,
        WEEK_STR[tm->tm_wday], tm->tm_hour, tm->tm_min, tm->tm_sec);
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
