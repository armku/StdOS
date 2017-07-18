#include "RTC.h"
#ifdef STM32F1
	#include "stm32f10x.h"
#endif
#ifdef STM32F4
	#include "stm32f4xx.h"
#endif

HardRtc hardRtc;

/* 秒中断标志，进入秒中断时置1，当时间被刷新之后清0 */
__IO uint32_t TimeDisplay = 0;

void HardRtc::RTC_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Enable the RTC Interrupt */
    #ifdef STM32F1
		NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	#endif
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
//生成实例
HardRtc * HardRtc::Instance()
{
	return &hardRtc;
}
void HardRtc::Start(bool b1 ,bool b2)
{
	
}

void HardRtc::RTC_Configuration(void)
{
    /* Enable PWR and BKP clocks */
    #ifdef STM32F1
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	#endif
    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    #ifdef STM32F1	
		BKP_DeInit();
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
    #ifdef STM32F1
		RTC_WaitForLastTask();
	#endif
		
    /* Enable the RTC Second */
    #ifdef STM32F1
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();

		/* Set RTC prescaler: set RTC period to 1sec */
		RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */
	
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
	#endif
}

//设置时间
void HardRtc::SetTime(uint seconds)
{	
	#ifdef STM32F1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
	PWR_BackupAccessCmd(ENABLE);	//使能RTC和后备寄存器访问 
	RTC_SetCounter(seconds);	//设置RTC计数器的值

	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  
	#endif
}
//设置时间-北京时间格式
void HardRtc::SetTime(DateTime & dt)
{
	this->SetTime(dt.TotalSeconds());
}
void HardRtc::Init()
{
	this->RTC_NVIC_Config();
	this->RTC_Configuration();
}
//读取时间
DateTime& HardRtc::GetTime(DateTime & dt)
{
	#ifdef STM32F1
	dt=RTC_GetCounter() + 8 * 60 * 60;
	#endif
	return dt;
}
#ifdef __cplusplus
    extern "C"
    {
    #endif 
    void RTC_IRQHandler(void)
    {
		#ifdef STM32F1
        if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
        {
            /* Clear the RTC Second interrupt */
            RTC_ClearITPendingBit(RTC_IT_SEC);

            /* Enable time update */
            TimeDisplay = 1;

            /* Wait until last write operation on RTC registers has finished */
            RTC_WaitForLastTask();
        }
		#endif
    }
    #ifdef __cplusplus
    }
#endif
//测试时钟
#if 1
DateTime now;//当前时间
HardRtc *Rtc;
void TimeRefresh(void* param)
{
	HardRtc * rtc=(HardRtc*)param;
	rtc->GetTime(now);
	now.Show();
}
void RTCtest()
{
	Rtc = HardRtc::Instance();
    Rtc->LowPower = false;
    Rtc->External = false;
    
	Rtc->GetTime(now);
	if(now.TotalSeconds()<100)
	{
		now.Year=2017;
		now.Month=6;
		now.Day=17;
		now.Hour=14;
		now.Minute=17;
		
		Rtc->Init();
		Rtc->Start(false, false);
		Rtc->SetTime(now);
	}
	Sys.AddTask(TimeRefresh,Rtc,100,1000,"TimeUp");
}
#endif
