#ifndef _SERIALPORTCONFIG_H
#define _SERIALPORTCONFIG_H

#include "Core\Delegate.h"

//~~~~~~~~~~~~~~~USART Congfigure~~~~~~~~~~~//
#define USE_USART              //---USART-without-DMA--//
#define USE_USART1             //---USART1-without-DMA-//
#define USE_USART2            //---USART2-without-DMA-//
#define USE_USART3            //---USART3-without-DMA-//
//#define USE_UART4            //---UART4-without-DMA-//
//#define USE_UART5            //---UART5-without-DMA-//

#ifdef USE_USART
#include "USART.h"
#endif
#ifdef USE_USART_DMA
#include "USART.h"
#endif

#ifdef USE_I2C
#include "I2C.h"
#endif

#ifdef USE_CAPTURE_TIM
#include "InputCapture_TIM.h"
#endif

#ifdef USE_CAPTURE_EXIT
#include "InputCapture_EXIT.h"
#endif

#ifdef USE_TIMER
#include "Timer.h"
#endif

#ifdef USE_ULTRASONIC
#include "Ultrasonic.h"
#endif

//old

#include "Platform\Pin.h"
#include "Port.h"

typedef enum
{
	Rising = 0x01,	// 上升沿
	Falling = 0x02,	// 下降沿
	Both = 0x03	// 上升下降沿
}Trigger;

class DeviceConfigCenter
{
public:
	//中断
	static void SetEXIT(int pinIndex, bool enable, Trigger trigger = Both);
	static void InputPort_OpenEXTI(Pin pin, Trigger trigger = Both);

	static Func PExit0;
	static Func PExit1;
	static Func PExit2;
	static Func PExit3;
	static Func PExit4;
	static Func PExit5;
	static Func PExit6;
	static Func PExit7;
	static Func PExit8;
	static Func PExit9;
	static Func PExit10;
	static Func PExit11;
	static Func PExit12;
	static Func PExit13;
	static Func PExit14;
	static Func PExit15;

	//串口	
			
	//定时器
	static void TimeTickInit();//系统用定时器初始化
	static void TimerConfig(TIMER tim, int interval, int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);//定时器配置
	static void Timer0Config(int interval);//定时器配置
	static void Timer1Config(int interval);//定时器配置
	static void Timer2Config(int interval);//定时器配置
	static void Timer3Config(int interval);//定时器配置
	static void Timer4Config(int interval);//定时器配置
	static void Timer5Config(int interval);//定时器配置
	static void Timer6Config(int interval);//定时器配置
	static void Timer7Config(int interval);//定时器配置
	static void Timer8Config(int interval);//定时器配置
	static void TimerConfigNvic(TIMER tim, int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);
	static void Timer0ConfigNvic(int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);
	static void Timer1ConfigNvic(int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);
	static void Timer2ConfigNvic(int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);
	static void Timer3ConfigNvic(int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);
	static void Timer4ConfigNvic(int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);
	static void Timer5ConfigNvic(int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);
	static void Timer6ConfigNvic(int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);
	static void Timer7ConfigNvic(int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);
	static void Timer8ConfigNvic(int NVIC_PriorityGroup = 0, int NVIC_IRQChannelPreemptionPriority = 0, int NVIC_IRQChannelSubPriorit = 3);

	static Func PTim2Update;
	static Func PTim3Update;
	static Func PTim4Update;
	static Func PTim5Update;
	static Func PTim6Update;
	static Func PTim7Update;
	static Func PTim8Update;
	//系统
	static int CurrentTick();

private:
	//中断

	//串口	
	static void SerialPort_GetPins(Pin *txPin, Pin *rxPin, COM index, bool Remap = false);

	//定时器
};

#endif // !_SERIALPORTCONFIG_H
