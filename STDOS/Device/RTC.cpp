#include "RTC.h"
#include "TTime.h"

#ifdef STM32F0
    #include "stm32f0xx.h"
#elif defined STM32F1
    #include "stm32f10x.h"
#elif defined STM32F4
    #include "stm32f4xx.h"
#endif 

HardRTC::HardRTC()
{
    this->LowPower = true;
    this->External = false;
}

HardRTC *HardRTC::Instance()
{
    return new HardRTC();
}

void HardRTC::Start(bool lowpower, bool external)
{
    lowpower = lowpower;
    external = external;
}

/* ���жϱ�־���������ж�ʱ��1����ʱ�䱻ˢ��֮����0 */
__IO uint32_t TimeDisplay = 0;

#ifdef __cplusplus
    extern "C"
    {
    #endif 
    void RTC_IRQHandler(void)
    {
        #ifdef STM32F0
        #elif defined STM32F1
            if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
            {
                /* Clear the RTC Second interrupt */
                RTC_ClearITPendingBit(RTC_IT_SEC);

                /* Enable time update */
                TimeDisplay = 1;

                /* Wait until last write operation on RTC registers has finished */
                RTC_WaitForLastTask();
            }
        #elif defined STM32F4
        #endif 
    }
    #ifdef __cplusplus
    }
#endif 
//����ʱ��
#if 0
    DateTime now; //��ǰʱ��

    void TimeRefresh(void *param)
    {
        HardRTC *rtc = (HardRTC*)param;
        rtc->LoadTime();
        now = Time.BaseSeconds + Time.Seconds;
        now.Show();
        debug_printf("  BaseSeconds:%d Seconds:%d\r\n", Time.BaseSeconds, Time.Seconds);
    }
    void RTCtest()
    {
        HardRTC *rtc = HardRTC::Instance();
        rtc->LowPower = false;
        rtc->External = false;

        rtc->LoadTime();
        Time.BaseSeconds = Time.Seconds + Time.BaseSeconds;
        now = Time.BaseSeconds + Time.Seconds;
        if (now.TotalSeconds() < 100)
        {
            now.Year = 2017;
            now.Month = 6;
            now.Day = 17;
            now.Hour = 14;
            now.Minute = 17;

            rtc->Init(); //����ʱ��ʱ��ʼ�����������
            Time.BaseSeconds = now.TotalSeconds() - Time.Seconds;
            rtc->LoadTime();
            rtc->SaveTime();
        }
        Sys.AddTask(TimeRefresh, rtc, 100, 1000, "TimeUp");
    }
#endif 

#ifdef STM32F01
    /* ���Ͷ��� ------------------------------------------------------------------*/
    typedef struct
    {
        uint8_t Year;
        uint8_t Month;
        uint8_t Date;
        uint8_t Week;
        uint8_t Hour;
        uint8_t Minute;
        uint8_t Second;
    } RTC_DateTimeTypeDef;

    typedef enum
    {
        RTC_OK = 0x00,  //��ȷ
        RTC_ERR,  //����
        RTC_TIMEOUT,  //��ʱ
        RTC_EXIT,  //�˳�
        RTC_OVERFLOW,  //���
    } RTC_RESULT;

    /* �궨�� --------------------------------------------------------------------*/
    #define SEC_DAY                   (24*3600)                //һ������
    #define SEC_YEAR                  (365*SEC_DAY)            //һ������(ƽ��)

    /* ���� ----------------------------------------------------------------------*/
    __IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;

    #define RTC_CLOCK_SOURCE_LSI                     //ʹ��LSIʱ��
    #define RTC_FLAG_BKP              0xA55A         //��־

    /************************************************
    �������� �� RTC_Configuration
    ��    �� �� RTC����
    ��    �� �� ��
    �� �� ֵ �� ��
    ��    �� �� MCD Application Team
     *************************************************/
    void RTC_Configuration(void)
    {
        /* 1��ʹ��LSIʱ�� */
        #if defined (RTC_CLOCK_SOURCE_LSI)

            RCC_LSICmd(ENABLE); //ʹ��LSIʱ��

            while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
            {
                //�ȴ�LSI����
            }

            RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI); //ѡ��LSIʱ��

            SynchPrediv = 0x18F;
            AsynchPrediv = 0x63;

            /* 2��ʹ��LSEʱ�� */
        #elif defined (RTC_CLOCK_SOURCE_LSE)

            RCC_LSEConfig(RCC_LSE_ON); //ʹ��LSIʱ��

            while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
            {
                //�ȴ�LSI����
            }

            RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); //ѡ��LSIʱ��

            SynchPrediv = 0xFF;
            AsynchPrediv = 0x7F;

        #else 
            #error Please select the RTC Clock source inside the main.c file
        #endif 

        RCC_RTCCLKCmd(ENABLE); //ʹ��RTC
        RTC_WaitForSynchro(); //�ȴ�ͬ��
    }

    /************************************************
    �������� �� RTC_DateRegulate
    ��    �� �� RTC���ڽ���
    ��    �� �� RTC_DateTimeStructure -- RTC�ṹ��
    �� �� ֵ �� RTC_OK ----------------- �ɹ�
    RTC_ERR ---------------- ����
    ��    �� �� Huang Fugui
     *************************************************/
    RTC_RESULT RTC_DateRegulate(RTC_DateTimeTypeDef RTC_DateTimeStructure)
    {
        RTC_DateTypeDef RTC_DateStructure;

        if (RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == SUCCESS)
        {
            return RTC_OK;
        }
        else
        {
            return RTC_ERR;
        }
    }

    /************************************************
    �������� �� RTC_TimeRegulate
    ��    �� �� RTCʱ�����
    ��    �� �� RTC_DateTimeStructure -- RTC�ṹ��
    �� �� ֵ �� RTC_OK ----------------- �ɹ�
    RTC_ERR ---------------- ����
    ��    �� �� Huang Fugui
     *************************************************/
    RTC_RESULT RTC_TimeRegulate(RTC_DateTimeTypeDef RTC_DateTimeStructure)
    {
        RTC_TimeTypeDef RTC_TimeStructure;

        RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
        RTC_TimeStructure.RTC_Hours = RTC_DateTimeStructure.Hour;
        RTC_TimeStructure.RTC_Minutes = RTC_DateTimeStructure.Minute;
        RTC_TimeStructure.RTC_Seconds = RTC_DateTimeStructure.Second;

        if (RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == SUCCESS)
        {
            return RTC_OK;
        }
        else
        {
            return RTC_ERR;
        }
    }
    /************************************************
    �������� �� RTC_SetDateTime
    ��    �� �� RTC����ʱ������
    ��    �� �� RTC_DateTimeStructure -- RTC�ṹ��
    �� �� ֵ �� RTC_OK ----------------- �ɹ�
    RTC_ERR ---------------- ����
    ��    �� �� Huang Fugui
     *************************************************/
    RTC_RESULT RTC_SetDateTime(RTC_DateTimeTypeDef RTC_DateTimeStructure)
    {
        if (RTC_ERR == RTC_DateRegulate(RTC_DateTimeStructure))
        {
            return RTC_ERR;
        }

        if (RTC_ERR == RTC_TimeRegulate(RTC_DateTimeStructure))
        {
            return RTC_ERR;
        }

        return RTC_OK;
    }

    /************************************************
    �������� �� RTC_Initializes
    ��    �� �� RTC��ʼ��
    ��    �� �� ��
    �� �� ֵ �� ��
    ��    �� �� Huang Fugui
     *************************************************/
    void RTC_Initializes(void)
    {
        RTC_InitTypeDef RTC_InitStructure;
        RTC_DateTimeTypeDef RTC_DateTimeStructure;

        PWR_BackupAccessCmd(ENABLE); //�������RTC

        if (RTC_ReadBackupRegister(RTC_BKP_DR0) != RTC_FLAG_BKP)
        {
            RTC_Configuration(); //����RTC

            RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
            RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
            RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
            if (RTC_Init(&RTC_InitStructure) == ERROR)
            {
                while (1)
                    ;
                //��ʼ��ʧ��
            }

            RTC_DateTimeStructure.Hour = 23;
            RTC_DateTimeStructure.Minute = 59;
            RTC_DateTimeStructure.Second = 59;
            RTC_SetDateTime(RTC_DateTimeStructure);
            RTC_WriteBackupRegister(RTC_BKP_DR0, RTC_FLAG_BKP);
        }
        else
        {
            #ifdef RTC_CLOCK_SOURCE_LSI
                RCC_LSICmd(ENABLE);
            #endif 

            RTC_WaitForSynchro(); //�ȴ�RTC��RTC_APBʱ��ͬ��
        }
    }


    /************************************************
    �������� �� RTC_GetDateTime
    ��    �� �� ��ȡRTC����ʱ�䣨�����������ڣ�
    ��    �� �� RTC_DateTimeStructure -- RTC�ṹ��
    �� �� ֵ �� ��
    ��    �� �� Huang Fugui
     *************************************************/
    void RTC_GetDateTime(DateTime &dt)
    {
        RTC_TimeTypeDef RTC_TimeStructure;
        RTC_DateTypeDef RTC_DateStructure;

        RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
        RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

        dt.Year = RTC_DateStructure.RTC_Year + 2000;
        dt.Month = RTC_DateStructure.RTC_Month;
        dt.Day = RTC_DateStructure.RTC_Date;

        dt.Hour = RTC_TimeStructure.RTC_Hours;
        dt.Minute = RTC_TimeStructure.RTC_Minutes;
        dt.Second = RTC_TimeStructure.RTC_Seconds;
    }
    DateTime now;
    void RTC_Demo(void *param)
    {
        RTC_GetDateTime(now); //��ȡ��ǰʱ��
        now.Show();
        printf("\r\n");
    }

    void BSP_Configuration(void) //Ӳ����ʼ������
    {
        //	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//����ADC�˿ڶ˿ڸ���ʹ��	

        RTC_Initializes();
    }

    void rtcf0test()
    {
        BSP_Configuration(); //����Ӳ����ʼ������
        Sys.AddTask(RTC_Demo, 0, 0, 1000, "RTC_Demo");
    }

#endif
