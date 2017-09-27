#include <stdio.h>
#include "SerialPort.h"
#include "Drivers\AT24CXX.h"
#include "Drivers\W25QXXX.h"
#include "Drivers\Button.h"
#include "SString.h"
#include "string.h"
#include "List.h"
#include "Spi.h"
#include "Flash.h"
#include "Drivers\lcd_dr.h"
#include "TInterrupt.h"
#include "Timer.h"
#include "TTime.h"
#include "Drivers\RX8025T.h"

class TTTTime
{
    public:
        uint Seconds; // ȫ��������ϵͳ���������������ۼ�
        UInt64 Milliseconds; // ȫ�ֺ�������ϵͳ�������ܺ��루1000ms�����֣����ۼ�
        uint CurrentTicks; // ��ǰ�δ�ʱ��
        UInt64 Current; // ��ǰ������
};
TTTTime now123;
void Test12(void *param)
{
    now123.Seconds = Time.Seconds;
    now123.Milliseconds = Time.Milliseconds;
    now123.Current = Time.Current();
    now123.CurrentTicks = Time.CurrentTicks();
}

#if 1
    OutputPort led1(PB0, false);
    OutputPort led2(PF7, false);
    OutputPort led3(PF8, false);
#else 
    OutputPort led1(PD0, false);
    OutputPort led2(PD1, false);
    OutputPort led3(PD2, false);
    OutputPort ledLCD(PD12, false);
#endif 

void LedTask(void *param)
{
    auto leds = (OutputPort*)param;
    *leds = ! * leds;
    //    led2 = key0;
}

uint OnUsart1Read(ITransport *transport, Buffer &bs, void *para, void *para2)
{
	transport->Write(bs);
    return 0;
}

uint time6cnt;
void TimerTask(void *param)
{
    static int i = 0;
    printf("\r\n%d: cnt:%d", i++, time6cnt);
}

SerialPort *sp1;
/* exti line config */
void EXTI_Pxy_Config(); 
int main(void)
{
    Sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 

	sp1=SerialPort::GetMessagePort();
    SerialPort::GetMessagePort()->Register(OnUsart1Read);
    
    Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
    Sys.AddTask(TimerTask, &led1, 0, 1000, "TimerTask");
    Sys.AddTask(Test12, 0, 600, 1000, "Test");
	
	IList::Test();
	/* exti line config */
	EXTI_Pxy_Config(); 
		
    Sys.Start();
}
#include "stm32f10x.h"
 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration()
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* �����ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


 /**
  * @brief  ���� PA0 Ϊ���жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  */
void EXTI_Pxy_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline clock and AFIO clock */
	RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO),ENABLE);
												
	/* config the NVIC */
	NVIC_Configuration();

	/* EXTI line gpio config*/	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // ��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* EXTI line mode config */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�������ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}

