#ifndef _SERIALPORTCONFIG_H
#define _SERIALPORTCONFIG_H

#include "Delegate.h"
//#define USE_GPS
//#ifdef USE_GPS
//#include "GPS.h"
//#endif

//~~~~~~~~~~~~~~~USART Congfigure~~~~~~~~~~~//
#define USE_USART              //---USART-without-DMA--//
#define USE_USART1             //---USART1-without-DMA-//
#define USE_USART2            //---USART2-without-DMA-//
#define USE_USART3            //---USART3-without-DMA-//
//#define USE_UART4            //---UART4-without-DMA-//
//#define USE_UART5            //---UART5-without-DMA-//

//#define USE_USART_DMA          //---USART-use-DMA--//
//#define USE_USART1_DMA         //---USART1-use-DMA--//
//#define USE_USART2_DMA         //---USART2-use-DMA--//
//#define USE_USART3_DMA         //---USART3-use-DMA--//
//#define USE_UART4_DMA         //---UART4-use-DMA--//
//#define USE_UART5_DMA         //---UART5-use-DMA--//

//~~~~~~~~~~~~~~~~I2C Congfigure~~~~~~~~~~~~//
//#define USE_I2C                //----IIC----//
//#define USE_I2C1             	 //----IIC1---//
//#define USE_I2C2               //----IIC2---//

//~~~~~~~~~~~~~~~Timer Congfigure~~~~~~~~~~~//
//#define USE_TIMER              //---Timer---//
//#define USE_TIMER1              //---Timer1--//
//#define USE_TIMER2             //---Timer2--//
//#define USE_TIMER3            //---Timer3--//
//#define USE_TIMER4            //---Timer4--//
//#define USE_TIMER5            //---Timer5--//
//#define USE_TIMER6            //---Timer6--//
//#define USE_TIMER7            //---Timer7--//

//~~~~~~~~~~~~~~Capture Congfigure~~~~~~~~~~//
//#define USE_CAPTURE_TIM         //--Capture- With Timer-//
//#define USE_CAPTURE_TIM1       //--ICP-TIM1-//
//#define USE_CAPTURE_TIM2       //--ICP-TIM2-//
//#define USE_CAPTURE_TIM3       //--ICP-TIM3-//
//#define USE_CAPTURE_TIM4       //--ICP-TIM4-//

//#define USE_CAPTURE_EXIT        //--Capture-With EXIT-//
//#define USE_CAPTURE_EXIT0       //--ICP-EXIT0//
//#define USE_CAPTURE_EXIT1       //--ICP-EXIT1//
//#define USE_CAPTURE_EXIT2       //--ICP-EXIT2//
//#define USE_CAPTURE_EXIT3       //--ICP-EXIT3//
//#define USE_CAPTURE_EXIT4       //--ICP-EXIT4//
//#define USE_CAPTURE_EXIT5       //--ICP-EXIT5//
//#define USE_CAPTURE_EXIT6       //--ICP-EXIT6//
//#define USE_CAPTURE_EXIT7       //--ICP-EXIT7//
//#define USE_CAPTURE_EXIT8       //--ICP-EXIT8//
//#define USE_CAPTURE_EXIT9       //--ICP-EXIT9//
//#define USE_CAPTURE_EXIT10      //--ICP-EXIT10//
//#define USE_CAPTURE_EXIT11      //--ICP-EXIT11//
//#define USE_CAPTURE_EXIT12      //--ICP-EXIT12//
//#define USE_CAPTURE_EXIT13      //--ICP-EXIT13//
//#define USE_CAPTURE_EXIT14      //--ICP-EXIT14//
//#define USE_CAPTURE_EXIT15      //--ICP-EXIT15//


//~~~~~~~~~~~~~~~~EXTI Congfigure~~~~~~~~~~~//
//#define USE_EXTI0            //---EXTI0---//
//#define USE_EXTI1            //---EXTI1---//
//#define USE_EXTI2            //---EXTI2---//
//#define USE_EXTI3            //---EXTI3---//
//#define USE_EXTI4            //---EXTI4---//
//#define USE_EXTI5            //---EXTI5---//
//#define USE_EXTI6            //---EXTI6---//
//#define USE_EXTI7            //---EXTI7---//
//#define USE_EXTI8            //---EXTI8---//
//#define USE_EXTI9            //---EXTI9---//
//#define USE_EXTI10           //---EXTI10--//
//#define USE_EXTI11           //---EXTI11--//
//#define USE_EXTI12           //---EXTI12--//
//#define USE_EXTI13           //---EXTI13--//
//#define USE_EXTI14           //---EXTI14--//
//#define USE_EXTI15           //---EXTI15--//

//~~~~~~~~~~~~~Ultrasonic Congfigure~~~~~~~~~//
//#define USE_ULTRASONIC         //-Ultrasonic-//
//#define USE_ULTRASONIC_EXTI0   //-Ultr-EXTI0-//
//#define USE_ULTRASONIC_EXTI1   //-Ultr-EXTI1-//
//#define USE_ULTRASONIC_EXTI2   //-Ultr-EXTI2-//
//#define USE_ULTRASONIC_EXTI3   //-Ultr-EXTI3-//
//#define USE_ULTRASONIC_EXTI4   //-Ultr-EXTI4-//
//#define USE_ULTRASONIC_EXTI5   //-Ultr-EXTI5-//
//#define USE_ULTRASONIC_EXTI6   //-Ultr-EXTI6-//
//#define USE_ULTRASONIC_EXTI7   //-Ultr-EXTI7-//
//#define USE_ULTRASONIC_EXTI8   //-Ultr-EXTI8-//
//#define USE_ULTRASONIC_EXTI9   //-Ultr-EXTI9-//
//#define USE_ULTRASONIC_EXTI10  //-Ultr-EXTI10-//
//#define USE_ULTRASONIC_EXTI11  //-Ultr-EXTI11-//
//#define USE_ULTRASONIC_EXTI12  //-Ultr-EXTI12-//
//#define USE_ULTRASONIC_EXTI13  //-Ultr-EXTI13-//
//#define USE_ULTRASONIC_EXTI14  //-Ultr-EXTI14-//
//#define USE_ULTRASONIC_EXTI15  //-Ultr-EXTI15-//


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

#include <stdint.h>
#include "Pin.h"
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
	static uint32_t CurrentTicks1();

private:
	//中断

	//串口	
	static void SerialPort_GetPins(Pin *txPin, Pin *rxPin, COM index, bool Remap = false);

	//定时器
};

#endif // !_SERIALPORTCONFIG_H
