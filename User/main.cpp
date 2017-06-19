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
#include "..\STDOS\Modbus\Modbus1.h"
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

const byte vers[]="yyyy-MM-dd HH:mm:ss";

OutputPort led1(PB0);
OutputPort led2(PF7);
OutputPort led3(PF8);

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
    led2 = key0;
}

#define namee "StdOS"

uint OnUsart1Read(ITransport *transport, Buffer &bs, void *param, void *param2)
{
    transport->Write(bs);
    return bs.Length();
}

//按键事件
void OnPress(InputPort &port, bool down)
{
    debug_printf("Press P%c%d down=%d\r\n", _PIN_NAME(port._Pin), down);
}
volatile uint time2cnt = 0; // ms 计时变量 
void LedTest(void *param)
{
    if (time2cnt >= 1)
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

int main(void)
{
    SerialPort *sp1;
    TSys &sys = (TSys &)(Sys);
    //    #if 0
    //        sys.Codec = codec;
    //	#endif

    sys.Name = (char*)namee;
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
        sp1 = SerialPort::GetMessagePort();
        sp1->Register(OnUsart1Read, sp1);

        WatchDog::Start(20000, 10000);
    #else 
        WatchDog::Start();
    #endif 
    //    #if 0
    //        //flash 最后一块作为配置区
    //        Config::Current = &Config::CreateFlash();
    //    #endif 

    InputPort key(PC13);
    key.Press = OnPress;
    key.UsePress();
    key.Open();
    //    exti.Register(OnKeyPress);
    //    exti1.Register(OnKeyPress);
    //	SerialPort::GetMessagePort()->Register(OnUsart1Read);

    // 初始化为输出
	Timer timer2(Timer2);
	abc.Bind(tim2refesh);
	timer2.Register(abc);	
    timer2.Open();
	timer2.SetFrequency(1);
			
    Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
    Sys.AddTask(LedTest, nullptr, 0, 10, "LedTest");
	Sys.AddTask(TimeRefresh,Rtc,100,1000,"TimeUp");
    Sys.Start();
}
