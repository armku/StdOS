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

//按键 PC13 PA0
//InputPort exti(PC13); //PA1 PB3     PA0 PC13
//InputPort exti1(PA0);
//void OnKeyPress(InputPort* pin, bool down, void *param)
//{
//    //led1.Write(onoff);
//    led2 = !led2;
////    printf("中断引脚：P%c%d 值%d \r\n", _PIN_NAME(pin), down);
//}

/*
ISO-V2:PB5控制485方向
ISO-V3:PC2控制485方向
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

////按键事件
//void OnPress(InputPort &port, bool down)
//{
//    debug_printf("Press P%c%d down=%d\r\n", _PIN_NAME(port._Pin), down);
//}
volatile uint time2cnt = 0; // ms 计时变量 
void LedTest(void *param)
{
    if (time2cnt >= 199)
     /* 1000 * 1 ms = 1s 时间到 */
    {
        time2cnt = 0;
        led3 = !led3;
    }
}
void tim2refesh(void * param)
{
	time2cnt++;
}
DateTime now;//当前时间
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
    //Rtc提取时间
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
    //        //flash 最后一块作为配置区
    //        Config::Current = &Config::CreateFlash();
    //    #endif 

//    InputPort key(PC13);
//    key.Press = OnPress;
//    key.UsePress();
//    key.Open();
    //    exti.Register(OnKeyPress);
    //    exti1.Register(OnKeyPress);
    //	SerialPort::GetMessagePort()->Register(OnUsart1Read);

    // 初始化为输出
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
		
		// 开启TIMx_CLK,x[2,3,4,5],即内部时钟CK_INT=72M
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);
	
		// 自动重装载寄存器周的值(计数值)
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    // 累计 TIM_Period个频率后产生一个更新或者中断
	  // 时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		// 时钟分频因子 ，没有用到，不用管
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// 重复计数器的值，通用定时器没有，不用管
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // 初始化定时器TIMx, x[2,3,4,5]
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		
		// 清除计数器中断标志位
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(TIM2, ENABLE);																		
    
		// 暂时关闭TIMx,x[2,3,4,5]的时钟，等待使用
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, DISABLE);   
}
void TIM3_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// 开启TIMx_CLK,x[2,3,4,5],即内部时钟CK_INT=72M
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE);
	
		// 自动重装载寄存器周的值(计数值)
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    // 累计 TIM_Period个频率后产生一个更新或者中断
	  // 时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		// 时钟分频因子 ，没有用到，不用管
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// 重复计数器的值，通用定时器没有，不用管
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // 初始化定时器TIMx, x[2,3,4,5]
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		
		// 清除计数器中断标志位
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(TIM3, ENABLE);																		
    
		// 暂时关闭TIMx,x[2,3,4,5]的时钟，等待使用
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, DISABLE);   
}
void TIM4_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// 开启TIMx_CLK,x[2,3,4,5],即内部时钟CK_INT=72M
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4, ENABLE);
	
		// 自动重装载寄存器周的值(计数值)
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    // 累计 TIM_Period个频率后产生一个更新或者中断
	  // 时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		// 时钟分频因子 ，没有用到，不用管
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// 重复计数器的值，通用定时器没有，不用管
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // 初始化定时器TIMx, x[2,3,4,5]
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
		
		// 清除计数器中断标志位
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(TIM4, ENABLE);																		
    
		// 暂时关闭TIMx,x[2,3,4,5]的时钟，等待使用
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4, DISABLE);   
}
void TIM5_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// 开启TIMx_CLK,x[2,3,4,5],即内部时钟CK_INT=72M
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM5, ENABLE);
	
		// 自动重装载寄存器周的值(计数值)
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    // 累计 TIM_Period个频率后产生一个更新或者中断
	  // 时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		// 时钟分频因子 ，没有用到，不用管
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// 重复计数器的值，通用定时器没有，不用管
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // 初始化定时器TIMx, x[2,3,4,5]
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
		
		// 清除计数器中断标志位
    TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(TIM5, ENABLE);																		
    
		// 暂时关闭TIMx,x[2,3,4,5]的时钟，等待使用
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM5, DISABLE);   
}
void TIM6_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// 开启TIMx_CLK,x[6,7],即内部时钟CK_INT=72M
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);
	
		// 自动重装载寄存器周的值(计数值)
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    // 累计 TIM_Period个频率后产生一个更新或者中断
	  // 时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		// 时钟分频因子 ，基本定时器TIM6和TIM7没有，不用管
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// 计数器计数模式，基本定时器TIM6和TIM7只能向上计数，没有计数模式的设置，不用管
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// 重复计数器的值，基本定时器TIM6和TIM7没有，不用管
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // 初始化定时器TIMx, x[6,7]
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
		
		// 清除计数器中断标志位
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(TIM6, ENABLE);																		
    
		// 暂时关闭TIMx,x[6,7]的时钟，等待使用
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, DISABLE);   
}
void TIM7_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// 开启TIMx_CLK,x[6,7],即内部时钟CK_INT=72M
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM7, ENABLE);
	
		// 自动重装载寄存器周的值(计数值)
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    // 累计 TIM_Period个频率后产生一个更新或者中断
	  // 时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		// 时钟分频因子 ，基本定时器TIM6和TIM7没有，不用管
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// 计数器计数模式，基本定时器TIM6和TIM7只能向上计数，没有计数模式的设置，不用管
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// 重复计数器的值，基本定时器TIM6和TIM7没有，不用管
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // 初始化定时器TIMx, x[6,7]
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
		
		// 清除计数器中断标志位
    TIM_ClearFlag(TIM7, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(TIM7, ENABLE);																		
    
		// 暂时关闭TIMx,x[6,7]的时钟，等待使用
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
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void TIM7_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void time2test()
{
	TIM2_Configuration();
	
	/* 配置基本定时器 TIMx,x[6,7]的中断优先级 */
	TIM2_NVIC_Configuration();

	/* 基本定时器 TIMx,x[6,7] 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);
}
void time3test()
{
	TIM3_Configuration();
	
	/* 配置基本定时器 TIMx,x[6,7]的中断优先级 */
	TIM3_NVIC_Configuration();

	/* 基本定时器 TIMx,x[6,7] 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE);
}
void time4test()
{
	TIM4_Configuration();
	
	/* 配置基本定时器 TIMx,x[6,7]的中断优先级 */
	TIM4_NVIC_Configuration();

	/* 基本定时器 TIMx,x[6,7] 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4, ENABLE);
}
void time5test()
{
	TIM5_Configuration();
	
	/* 配置基本定时器 TIMx,x[6,7]的中断优先级 */
	TIM5_NVIC_Configuration();

	/* 基本定时器 TIMx,x[6,7] 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM5, ENABLE);
}
void time6test()
{
	TIM6_Configuration();
	
	/* 配置基本定时器 TIMx,x[6,7]的中断优先级 */
	TIM6_NVIC_Configuration();

	/* 基本定时器 TIMx,x[6,7] 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);
}
void time7test()
{
	TIM7_Configuration();
	
	/* 配置基本定时器 TIMx,x[6,7]的中断优先级 */
	TIM7_NVIC_Configuration();

	/* 基本定时器 TIMx,x[6,7] 重新开时钟，开始计时 */
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
