#include "Port.h"
#include "stm32f0xx.h"

const byte vers[] = "yyyy-MM-dd HH:mm:ss";
OutputPort led1(PC6, true);
OutputPort led2(PC7, true);
OutputPort led3(PC8, true);
OutputPort led4(PC9, true);

OutputPort ledss[]={led1,led2,led3,led4};

void LedTask(void *param)
{
	static byte flag=1;
	OutputPort *leds = (OutputPort*)param;
	
	flag<<=1;
	flag&=0x0f;
	if(flag==0)
	{
		flag=1;
	}
	
	leds[0]=flag&0x01;
	leds[1]=flag&0x02;
	leds[2]=flag&0x04;
	leds[3]=flag&0x08;
	
    //    led2 = key0;
}

#define namee "StdOS"
/* 类型定义 ------------------------------------------------------------------*/
typedef struct
{
  //uint8_t Year;
  //uint8_t Month;
  //uint8_t Date;
  //uint8_t Week;
  uint8_t Hour;
  uint8_t Minute;
  uint8_t Second;
}RTC_DateTimeTypeDef;

typedef enum{
  RTC_OK = 0x00,                  //正确
  RTC_ERR,                        //错误
  RTC_TIMEOUT,                    //超时
  RTC_EXIT,                       //退出
  RTC_OVERFLOW,                   //溢出
}RTC_RESULT;

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

  RCC_LSICmd(ENABLE);                            //使能LSI时钟

  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {                                              //等待LSI就绪
  }

  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);        //选择LSI时钟

  SynchPrediv = 0x18F;
  AsynchPrediv = 0x63;

/* 2、使用LSE时钟 */
#elif defined (RTC_CLOCK_SOURCE_LSE)

  RCC_LSEConfig(RCC_LSE_ON);                     //使能LSI时钟

  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {                                              //等待LSI就绪
  }

  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);        //选择LSI时钟
  
  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;

#else
  #error Please select the RTC Clock source inside the main.c file
#endif

  RCC_RTCCLKCmd(ENABLE);                         //使能RTC
  RTC_WaitForSynchro();                          //等待同步
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

  if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == SUCCESS)
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

  if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == SUCCESS)
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
  if(RTC_ERR == RTC_DateRegulate(RTC_DateTimeStructure))
  {
    return RTC_ERR;
  }

  if(RTC_ERR == RTC_TimeRegulate(RTC_DateTimeStructure))
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

  PWR_BackupAccessCmd(ENABLE);                   //允许操作RTC

  if(RTC_ReadBackupRegister(RTC_BKP_DR0) != RTC_FLAG_BKP)
  {
    RTC_Configuration();                         //配置RTC

    RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
    RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    if(RTC_Init(&RTC_InitStructure) == ERROR)
    {
      while(1);          //初始化失败
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

    RTC_WaitForSynchro();  //等待RTC与RTC_APB时钟同步
  }
}


/************************************************
函数名称 ： RTC_GetDateTime
功    能 ： 读取RTC日期时间（含日历、星期）
参    数 ： RTC_DateTimeStructure -- RTC结构体
返 回 值 ： 无
作    者 ： Huang Fugui
*************************************************/
void RTC_GetDateTime(RTC_DateTimeTypeDef *RTC_DateTimeStructure)
{
  RTC_TimeTypeDef RTC_TimeStructure;

  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
  RTC_DateTimeStructure->Hour = RTC_TimeStructure.RTC_Hours;
  RTC_DateTimeStructure->Minute = RTC_TimeStructure.RTC_Minutes;
  RTC_DateTimeStructure->Second = RTC_TimeStructure.RTC_Seconds;
}

void RTC_Demo(void *param)
{
	RTC_DateTimeTypeDef RTC_DateTimeStructure;

	RTC_GetDateTime(&RTC_DateTimeStructure);   //读取当前时间		
	printf("时间：%0.2d:%0.2d:%0.2d\r\n",
	RTC_DateTimeStructure.Hour,
	RTC_DateTimeStructure.Minute,
	RTC_DateTimeStructure.Second);
}



void RCC_Configuration(void)
{  
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);//设置A端口时钟使能
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);//设置C端口时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//设置ADC端口端口复用使能	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//设置串口1使能
}
void BSP_Configuration(void)//硬件初始化函数
{	
	RCC_Configuration();//调用时钟初始化函数
	
	RTC_Initializes();
}
static void NVIC_Configuration(void)//中断优先级函数
{
	NVIC_InitTypeDef   NVIC_InitStructure;

	// rs232
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
int main()
{
	Sys.Name = (char*)namee;
    Sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 
	BSP_Configuration();//调用硬件初始化函数
	NVIC_Configuration();//调用中断优先级初始化函数
	Sys.AddTask(LedTask, &ledss, 0, 500, "LedTask");
	Sys.AddTask(RTC_Demo, &ledss, 0, 1000, "RTC_Demo");

    Sys.Start();
}
