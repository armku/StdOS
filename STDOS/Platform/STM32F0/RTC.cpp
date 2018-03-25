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

    /* Enable the RTC Interrupt */
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
//////////////////////////////////////////////////////////////////////////////////

#ifdef STM32F01
    /* 类型定义 ------------------------------------------------------------------*/
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
        RTC_OK = 0x00,  //正确
        RTC_ERR,  //错误
        RTC_TIMEOUT,  //超时
        RTC_EXIT,  //退出
        RTC_OVERFLOW,  //溢出
    } RTC_RESULT;

    /* 宏定义 --------------------------------------------------------------------*/
    #define SEC_DAY                   (24*3600)                //一天秒数
    #define SEC_YEAR                  (365*SEC_DAY)            //一年秒数(平年)

    /* 变量 ----------------------------------------------------------------------*/
    __IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;

    #define RTC_CLOCK_SOURCE_LSI                     //使用LSI时钟
    #define RTC_FLAG_BKP              0xA55A         //标志

    /************************************************
    函数名称 ： RTC_Configuration
    功    能 ： RTC配置
    参    数 ： 无
    返 回 值 ： 无
    作    者 ： MCD Application Team
     *************************************************/
    void RTC_Configuration(void)
    {
        /* 1、使用LSI时钟 */
        #if defined (RTC_CLOCK_SOURCE_LSI)

            RCC_LSICmd(ENABLE); //使能LSI时钟

            while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
            {
                //等待LSI就绪
            }

            RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI); //选择LSI时钟

            SynchPrediv = 0x18F;
            AsynchPrediv = 0x63;

            /* 2、使用LSE时钟 */
        #elif defined (RTC_CLOCK_SOURCE_LSE)

            RCC_LSEConfig(RCC_LSE_ON); //使能LSI时钟

            while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
            {
                //等待LSI就绪
            }

            RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); //选择LSI时钟

            SynchPrediv = 0xFF;
            AsynchPrediv = 0x7F;

        #else 
            #error Please select the RTC Clock source inside the main.c file
        #endif 

        RCC_RTCCLKCmd(ENABLE); //使能RTC
        RTC_WaitForSynchro(); //等待同步
    }

    /************************************************
    函数名称 ： RTC_DateRegulate
    功    能 ： RTC日期矫正
    参    数 ： RTC_DateTimeStructure -- RTC结构体
    返 回 值 ： RTC_OK ----------------- 成功
    RTC_ERR ---------------- 错误
    作    者 ： Huang Fugui
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
    函数名称 ： RTC_TimeRegulate
    功    能 ： RTC时间矫正
    参    数 ： RTC_DateTimeStructure -- RTC结构体
    返 回 值 ： RTC_OK ----------------- 成功
    RTC_ERR ---------------- 错误
    作    者 ： Huang Fugui
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
    函数名称 ： RTC_SetDateTime
    功    能 ： RTC日期时间设置
    参    数 ： RTC_DateTimeStructure -- RTC结构体
    返 回 值 ： RTC_OK ----------------- 成功
    RTC_ERR ---------------- 错误
    作    者 ： Huang Fugui
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
    函数名称 ： RTC_Initializes
    功    能 ： RTC初始化
    参    数 ： 无
    返 回 值 ： 无
    作    者 ： Huang Fugui
     *************************************************/
    void RTC_Initializes(void)
    {
        RTC_InitTypeDef RTC_InitStructure;
        RTC_DateTimeTypeDef RTC_DateTimeStructure;

        PWR_BackupAccessCmd(ENABLE); //允许操作RTC

        if (RTC_ReadBackupRegister(RTC_BKP_DR0) != RTC_FLAG_BKP)
        {
            RTC_Configuration(); //配置RTC

            RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
            RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
            RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
            if (RTC_Init(&RTC_InitStructure) == ERROR)
            {
                while (1)
                    ;
                //初始化失败
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

            RTC_WaitForSynchro(); //等待RTC与RTC_APB时钟同步
        }
    }


    /************************************************
    函数名称 ： RTC_GetDateTime
    功    能 ： 读取RTC日期时间（含日历、星期）
    参    数 ： RTC_DateTimeStructure -- RTC结构体
    返 回 值 ： 无
    作    者 ： Huang Fugui
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
        RTC_GetDateTime(now); //读取当前时间
        now.Show();
        printf("\r\n");
    }

    void BSP_Configuration(void) //硬件初始化函数
    {
        //	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//设置ADC端口端口复用使能	

        RTC_Initializes();
    }

    void rtcf0test()
    {
        BSP_Configuration(); //调用硬件初始化函数
        Sys.AddTask(RTC_Demo, 0, 0, 1000, "RTC_Demo");
    }

#endif
