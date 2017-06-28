#include <stdio.h>
#include "SerialPort.h"
#include "Drivers\AT24CXX.h"
#include "Drivers\Button.h"
#include "Port.h"
#include "WatchDog.h"
#include "SString.h"
#include "string.h"
#include "DateTime.h"
#include "List.h"
#include "Pwm.h"
//#include "Modbus\Modbus1.h"
#include "Spi.h"
#include "RTC.h"
#include "Flash.h"
#include "Drivers\lcd_dr.h"
#include "TInterrupt.h"

const byte vers[]="yyyy-MM-dd HH:mm:ss";

OutputPort led1(PD0);//led1(PB0);
OutputPort led2(PD1);//led2(PF7);
OutputPort led3(PD2);//led3(PF8);

//���� PC13 PA0
//InputPort exti(PC13); //PA1 PB3     PA0 PC13
//InputPort exti1(PA0);
//void OnKeyPress(InputPort* pin, bool down, void *param)
//{
//    //led1.Write(onoff);
//    led2 = !led2;
////    printf("�ж����ţ�P%c%d ֵ%d \r\n", _PIN_NAME(pin), down);
//}

/*
ISO-V2:PB5����485����
ISO-V3:PC2����485����
 */
//ModbusSlave ModbusSlave;
//static uint OnUsartRead(ITransport *transport, Buffer &bs, void *para)
//{
//    ModbusSlave.Process(bs, para);
//    return 0;
//}
//static uint OnUsart1Read(ITransport *transport, Buffer &bs, void *para)
//{
//   bs.Show(true);
//    return 0;
//}
//InputPort key0(PA0);
void LedTask(void *param)
{
    OutputPort *leds = (OutputPort*)param;
    *leds = ! * leds;
//    led2 = key0;
}

//#define namee "StdOS"

//uint OnUsart1Read(ITransport *transport, Buffer &bs, void *param, void *param2)
//{
//    transport->Write(bs);
//    return bs.Length();
//}

////�����¼�
//void OnPress(InputPort &port, bool down)
//{
//    debug_printf("Press P%c%d down=%d\r\n", _PIN_NAME(port._Pin), down);
//}
volatile uint time2cnt = 0; // ms ��ʱ���� 
void LedTest(void *param)
{
    if (time2cnt >= 199)
     /* 1000 * 1 ms = 1s ʱ�䵽 */
    {
        time2cnt = 0;
        led3 = !led3;
    }
}
void tim2refesh(void * param)
{
	time2cnt++;
}
DateTime now;//��ǰʱ��
void TimeRefresh(void* param)
{
	HardRtc * rtc=(HardRtc*)param;
	rtc->GetTime(now);
	now.Show();
}

Delegate<Timer&> abc;
//CLcd_DR lcddr1(PD3, PD6, PD7, PB3, PB4);
//PWM ledLCD(PD12);
void timetest();
int main(void)
{
//    SerialPort *sp1;
    TSys &sys = (TSys &)(Sys);
    //    #if 0
    //        sys.Codec = codec;
    //	#endif

//    sys.Name = (char*)namee;
	byte aa=vers[0];
	aa=aa;
    //Rtc��ȡʱ��
    HardRtc *Rtc = HardRtc::Instance();
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
    sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
//        sp1 = SerialPort::GetMessagePort();
        //sp1->Register(OnUsart1Read, sp1);

        WatchDog::Start(20000, 10000);
    #else 
        WatchDog::Start();
    #endif 
    //    #if 0
    //        //flash ���һ����Ϊ������
    //        Config::Current = &Config::CreateFlash();
    //    #endif 

//    InputPort key(PC13);
//    key.Press = OnPress;
//    key.UsePress();
//    key.Open();
    //    exti.Register(OnKeyPress);
    //    exti1.Register(OnKeyPress);
    //	SerialPort::GetMessagePort()->Register(OnUsart1Read);

    // ��ʼ��Ϊ���
	Timer timer2(Timer3);
//	abc.Bind(tim2refesh);
//	timer2.Register(abc);	
    timer2.Open();
//	timer2.SetFrequency(1);
//	timer2.Config();
//	ledLCD.Init();
//	ledLCD.SetOutPercent(50);
			timetest();
    Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
    Sys.AddTask(LedTest, nullptr, 0, 10, "LedTest");
	Sys.AddTask(TimeRefresh,Rtc,100,1000,"TimeUp");
    Sys.Start();
}
#include "stm32f10x.h"
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// ����TIMx_CLK,x[2,3,4,5],���ڲ�ʱ��CK_INT=72M
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);
	
		// �Զ���װ�ؼĴ����ܵ�ֵ(����ֵ)
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    // �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж�
	  // ʱ��Ԥ��Ƶ��Ϊ71����������������ʱ��CK_CNT = CK_INT / (71+1)=1M
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		// ʱ�ӷ�Ƶ���� ��û���õ������ù�
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// �ظ���������ֵ��ͨ�ö�ʱ��û�У����ù�
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // ��ʼ����ʱ��TIMx, x[2,3,4,5]
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		
		// ����������жϱ�־λ
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(TIM2, ENABLE);																		
    
		// ��ʱ�ر�TIMx,x[2,3,4,5]��ʱ�ӣ��ȴ�ʹ��
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, DISABLE);   
}
void TIM3_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// ����TIMx_CLK,x[2,3,4,5],���ڲ�ʱ��CK_INT=72M
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE);
	
		// �Զ���װ�ؼĴ����ܵ�ֵ(����ֵ)
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    // �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж�
	  // ʱ��Ԥ��Ƶ��Ϊ71����������������ʱ��CK_CNT = CK_INT / (71+1)=1M
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		// ʱ�ӷ�Ƶ���� ��û���õ������ù�
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// �ظ���������ֵ��ͨ�ö�ʱ��û�У����ù�
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // ��ʼ����ʱ��TIMx, x[2,3,4,5]
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		
		// ����������жϱ�־λ
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(TIM3, ENABLE);																		
    
		// ��ʱ�ر�TIMx,x[2,3,4,5]��ʱ�ӣ��ȴ�ʹ��
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, DISABLE);   
}
void TIM4_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// ����TIMx_CLK,x[2,3,4,5],���ڲ�ʱ��CK_INT=72M
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4, ENABLE);
	
		// �Զ���װ�ؼĴ����ܵ�ֵ(����ֵ)
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    // �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж�
	  // ʱ��Ԥ��Ƶ��Ϊ71����������������ʱ��CK_CNT = CK_INT / (71+1)=1M
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		// ʱ�ӷ�Ƶ���� ��û���õ������ù�
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// �ظ���������ֵ��ͨ�ö�ʱ��û�У����ù�
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // ��ʼ����ʱ��TIMx, x[2,3,4,5]
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
		
		// ����������жϱ�־λ
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(TIM4, ENABLE);																		
    
		// ��ʱ�ر�TIMx,x[2,3,4,5]��ʱ�ӣ��ȴ�ʹ��
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4, DISABLE);   
}
void TIM5_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// ����TIMx_CLK,x[2,3,4,5],���ڲ�ʱ��CK_INT=72M
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM5, ENABLE);
	
		// �Զ���װ�ؼĴ����ܵ�ֵ(����ֵ)
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    // �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж�
	  // ʱ��Ԥ��Ƶ��Ϊ71����������������ʱ��CK_CNT = CK_INT / (71+1)=1M
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		// ʱ�ӷ�Ƶ���� ��û���õ������ù�
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// �ظ���������ֵ��ͨ�ö�ʱ��û�У����ù�
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // ��ʼ����ʱ��TIMx, x[2,3,4,5]
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
		
		// ����������жϱ�־λ
    TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(TIM5, ENABLE);																		
    
		// ��ʱ�ر�TIMx,x[2,3,4,5]��ʱ�ӣ��ȴ�ʹ��
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM5, DISABLE);   
}
void TIM6_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// ����TIMx_CLK,x[6,7],���ڲ�ʱ��CK_INT=72M
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);
	
		// �Զ���װ�ؼĴ����ܵ�ֵ(����ֵ)
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    // �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж�
	  // ʱ��Ԥ��Ƶ��Ϊ71����������������ʱ��CK_CNT = CK_INT / (71+1)=1M
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		// ʱ�ӷ�Ƶ���� ��������ʱ��TIM6��TIM7û�У����ù�
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// ����������ģʽ��������ʱ��TIM6��TIM7ֻ�����ϼ�����û�м���ģʽ�����ã����ù�
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// �ظ���������ֵ��������ʱ��TIM6��TIM7û�У����ù�
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // ��ʼ����ʱ��TIMx, x[6,7]
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
		
		// ����������жϱ�־λ
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(TIM6, ENABLE);																		
    
		// ��ʱ�ر�TIMx,x[6,7]��ʱ�ӣ��ȴ�ʹ��
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, DISABLE);   
}
void TIM7_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// ����TIMx_CLK,x[6,7],���ڲ�ʱ��CK_INT=72M
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM7, ENABLE);
	
		// �Զ���װ�ؼĴ����ܵ�ֵ(����ֵ)
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    // �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж�
	  // ʱ��Ԥ��Ƶ��Ϊ71����������������ʱ��CK_CNT = CK_INT / (71+1)=1M
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		// ʱ�ӷ�Ƶ���� ��������ʱ��TIM6��TIM7û�У����ù�
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// ����������ģʽ��������ʱ��TIM6��TIM7ֻ�����ϼ�����û�м���ģʽ�����ã����ù�
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// �ظ���������ֵ��������ʱ��TIM6��TIM7û�У����ù�
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // ��ʼ����ʱ��TIMx, x[6,7]
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
		
		// ����������жϱ�־λ
    TIM_ClearFlag(TIM7, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(TIM7, ENABLE);																		
    
		// ��ʱ�ر�TIMx,x[6,7]��ʱ�ӣ��ȴ�ʹ��
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM7, DISABLE);   
}
void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void TIM3_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void TIM4_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void TIM5_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void TIM6_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void TIM7_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void time2test()
{
	TIM2_Configuration();
	
	/* ���û�����ʱ�� TIMx,x[6,7]���ж����ȼ� */
	TIM2_NVIC_Configuration();

	/* ������ʱ�� TIMx,x[6,7] ���¿�ʱ�ӣ���ʼ��ʱ */
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);
}
void time3test()
{
	TIM3_Configuration();
	
	/* ���û�����ʱ�� TIMx,x[6,7]���ж����ȼ� */
	TIM3_NVIC_Configuration();

	/* ������ʱ�� TIMx,x[6,7] ���¿�ʱ�ӣ���ʼ��ʱ */
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE);
}
void time4test()
{
	TIM4_Configuration();
	
	/* ���û�����ʱ�� TIMx,x[6,7]���ж����ȼ� */
	TIM4_NVIC_Configuration();

	/* ������ʱ�� TIMx,x[6,7] ���¿�ʱ�ӣ���ʼ��ʱ */
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4, ENABLE);
}
void time5test()
{
	TIM5_Configuration();
	
	/* ���û�����ʱ�� TIMx,x[6,7]���ж����ȼ� */
	TIM5_NVIC_Configuration();

	/* ������ʱ�� TIMx,x[6,7] ���¿�ʱ�ӣ���ʼ��ʱ */
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM5, ENABLE);
}
void time6test()
{
	TIM6_Configuration();
	
	/* ���û�����ʱ�� TIMx,x[6,7]���ж����ȼ� */
	TIM6_NVIC_Configuration();

	/* ������ʱ�� TIMx,x[6,7] ���¿�ʱ�ӣ���ʼ��ʱ */
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);
}
void time7test()
{
	TIM7_Configuration();
	
	/* ���û�����ʱ�� TIMx,x[6,7]���ж����ȼ� */
	TIM7_NVIC_Configuration();

	/* ������ʱ�� TIMx,x[6,7] ���¿�ʱ�ӣ���ʼ��ʱ */
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM7, ENABLE);
}
void timetest()
{
//	time2test();
//	time3test();
//	time4test();
//	time5test();
//	time6test();
//	time7test();
	
}
