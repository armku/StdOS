#ifndef _SERIALPORTCONFIG_H
#define _SERIALPORTCONFIG_H
#include <stdint.h>
#include "Platform\Pin.h"
#include "Port.h"
#include "Core\Queue.h"

typedef enum
{
	Rising = 0x01,	// 上升沿
	Falling = 0x02,	// 下降沿
	Both = 0x03	// 上升下降沿
}Trigger;

class DeviceConfigHelper
{
public:
	//中断
	static void SetEXIT(int pinIndex, bool enable, Trigger trigger = Both);
	static void InputPort_OpenEXTI(Pin pin, Trigger trigger = Both);

	//串口
	static void ConfigCom(COM com, int baudRate);
	static void ComChgBaudRate(COM com, int baudRate);//改变波特率
	static void com1send();
	static void com2send();
	static void com3send();
	static void com4send();
	static void com5send();
	static void com1send(Buffer& bs);
	static void com2send(Buffer& bs);
	static void com3send(Buffer& bs);
	static void com4send(Buffer& bs);
	static void com5send(Buffer& bs);
	static void comSend(COM com, Buffer bs);//发送数据
	static OutputPort *pCOM1Rx485;
	static OutputPort *pCOM2Rx485;
	static OutputPort *pCOM3Rx485;
	static OutputPort *pCOM4Rx485;
	static OutputPort *pCOM5Rx485;

	//定时器
	static void TimeTickInit();//系统用定时器初始化
	static void TimerConfig(TIMER tim,int interval,int NVIC_PriorityGroup=0,int NVIC_IRQChannelPreemptionPriority = 0,int NVIC_IRQChannelSubPriorit=3);//定时器配置
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
	//系统
	static int CurrentTick();
	static uint32_t CurrentTicks1();

private:
	//中断

	//串口
	static void configCOM1(int baudRate);
	static void configCOM2(int baudRate);
	static void configCOM3(int baudRate);
	static void configCOM4(int baudRate);
	static void configCOM5(int baudRate);
	static void Com1ChgBaudRate(int baudRate);
	static void Com2ChgBaudRate(int baudRate);
	static void Com3ChgBaudRate(int baudRate);
	static void Com4ChgBaudRate(int baudRate);
	static void Com5ChgBaudRate(int baudRate);
	static void SerialPort_GetPins(Pin *txPin, Pin *rxPin, COM index, bool Remap = false);
	//定时器

};
#ifdef __cplusplus
extern "C" {
#endif
#define USECOM1 1
#define USECOM2 1
#define USECOM3 1
#define USECOM4 0
#define USECOM5 0

#ifdef USECOM1
extern Queue	Txx1;
extern Queue	Rxx1;
#endif // USECOM1

#ifdef USECOM2
extern Queue	Txx2;
extern Queue	Rxx2;
#endif // USECOM2

#ifdef USECOM3
extern Queue	Txx3;
extern Queue	Rxx3;
#endif // USECOM3

#ifdef USECOM4
extern Queue	Txx4;
extern Queue	Rxx4;
#endif // USECOM4

#ifdef USECOM5
extern Queue	Txx5;
extern Queue	Rxx5;
#endif // USECOM5

#ifdef __cplusplus
}
#endif

#endif // !_SERIALPORTCONFIG_H
