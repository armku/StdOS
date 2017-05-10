#include <stdio.h>
#include "SerialPort.h"
#include "AT24CXX.h"
#include "Button.h"
#include "Port.h"
#include "WatchDog.h"
#include "SString.h"
#include "string.h"
#include "DateTime.h"
#include "List.h"
#include "Pwm.h"
#include "Modbus1.h"
#include "Spi.h"
#include "RTC.h"
#include "Flash.h"
#include "TInterrupt.h"

typedef enum
{
    KEY_UP = 1,  // 模拟输入 
    KEY_DOWN = 1 << 1,  // 输入
    KEY_LEFT = 1 << 2,  // 浮空输入
    KEY_RIGHT = 1 << 3,  // 推挽式输入
    KEY_OK = 1 << 4,  // 开漏输出
    KEY_MENU = 1 << 5,  // 推挽式输出
    KEY_NULL = 0
} KEY_MODE;

OutputPort led1(PB0);
OutputPort led2(PF7);
OutputPort led(PF8);

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
InputPort key0(PA0);
void LedTask(void *param)
{
    OutputPort *leds = (OutputPort*)param;
    *leds = ! * leds;
    led1 = key0;
}

#define namee "StdOS"

uint OnUsart1Read(ITransport *transport, Buffer &bs, void *param,void *param2)
{
	transport->Write(bs);
	return bs.Length();
}
//按键事件
void OnPress(InputPort& port,bool down)
{
	debug_printf("Press P%c%d down=%d\r\n",_PIN_NAME(port._Pin),down);
}
void TIM2_Configuration(void);
void TIM2_NVIC_Configuration(void);
#include "stm32f10x_tim.h"
volatile u32 time2cnt = 0; // ms 计时变量 
void LedTest(void *param)
{
    if ( time2cnt >= 1000 ) /* 1000 * 1 ms = 1s 时间到 */
    {
      time2cnt = 0;   
		led=!led; 
    }    
}
int main(void)
{
    SerialPort *sp1;
    TSys &sys = (TSys &)(Sys);
    //    #if 0
    //        sys.Codec = codec;
    //	#endif

    sys.Name = (char*)namee;
    //Rtc提取时间
    //    HardRtc *Rtc = HardRtc::Instance();
    //    Rtc->LowPower = false;
    //    Rtc->External = false;
    //    Rtc->Init();
    //    Rtc->Start(false, false);
    sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
        sp1 = SerialPort::GetMessagePort();
        sp1->Register(OnUsart1Read, sp1);

        //        WatchDog::Start(20000, 10000);
        //    #else 
        //        WatchDog::Start();
    #endif 
    //    #if 0
    //        //flash 最后一块作为配置区
    //        Config::Current = &Config::CreateFlash();
    //    #endif 

	InputPort key(PC13);
	key.Press	= OnPress;
	key.UsePress();
	key.Open();
    //    exti.Register(OnKeyPress);
    //    exti1.Register(OnKeyPress);
    //	SerialPort::GetMessagePort()->Register(OnUsart1Read);

    // 初始化为输出
	
	/* TIM2 定时配置 */	
  TIM2_Configuration();
	
	/* 实战定时器的中断优先级 */
	TIM2_NVIC_Configuration();

	/* TIM2 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	
	
	
    //Sys.AddTask(LedTask, &led, 0, 500, "LedTask");
	Sys.AddTask(LedTest,nullptr,0,10,"LedTest");
    Sys.Start();
}

/// TIM2中断优先级配置
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

/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * 中断周期为 = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数 
 */
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		/* 设置TIM2CLK 为 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    //TIM_DeInit(TIM2);
	
	/* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
	  /* 时钟预分频数为72 */
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		/* 对外部时钟进行采样的时钟分频,这里没有用到 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(TIM2, ENABLE);																		
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/*先关闭等待使用*/    
}
