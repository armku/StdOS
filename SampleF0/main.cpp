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
/* ���Ͷ��� ------------------------------------------------------------------*/
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
  RTC_OK = 0x00,                  //��ȷ
  RTC_ERR,                        //����
  RTC_TIMEOUT,                    //��ʱ
  RTC_EXIT,                       //�˳�
  RTC_OVERFLOW,                   //���
}RTC_RESULT;

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

  RCC_LSICmd(ENABLE);                            //ʹ��LSIʱ��

  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {                                              //�ȴ�LSI����
  }

  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);        //ѡ��LSIʱ��

  SynchPrediv = 0x18F;
  AsynchPrediv = 0x63;

/* 2��ʹ��LSEʱ�� */
#elif defined (RTC_CLOCK_SOURCE_LSE)

  RCC_LSEConfig(RCC_LSE_ON);                     //ʹ��LSIʱ��

  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {                                              //�ȴ�LSI����
  }

  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);        //ѡ��LSIʱ��
  
  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;

#else
  #error Please select the RTC Clock source inside the main.c file
#endif

  RCC_RTCCLKCmd(ENABLE);                         //ʹ��RTC
  RTC_WaitForSynchro();                          //�ȴ�ͬ��
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
�������� �� RTC_SetDateTime
��    �� �� RTC����ʱ������
��    �� �� RTC_DateTimeStructure -- RTC�ṹ��
�� �� ֵ �� RTC_OK ----------------- �ɹ�
            RTC_ERR ---------------- ����
��    �� �� Huang Fugui
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

  PWR_BackupAccessCmd(ENABLE);                   //�������RTC

  if(RTC_ReadBackupRegister(RTC_BKP_DR0) != RTC_FLAG_BKP)
  {
    RTC_Configuration();                         //����RTC

    RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
    RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    if(RTC_Init(&RTC_InitStructure) == ERROR)
    {
      while(1);          //��ʼ��ʧ��
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

    RTC_WaitForSynchro();  //�ȴ�RTC��RTC_APBʱ��ͬ��
  }
}


/************************************************
�������� �� RTC_GetDateTime
��    �� �� ��ȡRTC����ʱ�䣨�����������ڣ�
��    �� �� RTC_DateTimeStructure -- RTC�ṹ��
�� �� ֵ �� ��
��    �� �� Huang Fugui
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

	RTC_GetDateTime(&RTC_DateTimeStructure);   //��ȡ��ǰʱ��		
	printf("ʱ�䣺%0.2d:%0.2d:%0.2d\r\n",
	RTC_DateTimeStructure.Hour,
	RTC_DateTimeStructure.Minute,
	RTC_DateTimeStructure.Second);
}



void RCC_Configuration(void)
{  
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);//����A�˿�ʱ��ʹ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);//����C�˿�ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//����ADC�˿ڶ˿ڸ���ʹ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//���ô���1ʹ��
}
void BSP_Configuration(void)//Ӳ����ʼ������
{	
	RCC_Configuration();//����ʱ�ӳ�ʼ������
	
	RTC_Initializes();
}
static void NVIC_Configuration(void)//�ж����ȼ�����
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
	BSP_Configuration();//����Ӳ����ʼ������
	NVIC_Configuration();//�����ж����ȼ���ʼ������
	Sys.AddTask(LedTask, &ledss, 0, 500, "LedTask");
	Sys.AddTask(RTC_Demo, &ledss, 0, 1000, "RTC_Demo");

    Sys.Start();
}
