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

/// 配置USART1接收中断
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

int main(void)
{
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
        NVIC_Configuration();
        Sys.ShowInfo();

        //        WatchDog::Start(20000, 10000);
        //    #else 
        //        WatchDog::Start();
    #endif 
    //    #if 0
    //        //flash 最后一块作为配置区
    //        Config::Current = &Config::CreateFlash();
    //    #endif 

    //    exti.Register(OnKeyPress);
    //    exti1.Register(OnKeyPress);
    //	SerialPort::GetMessagePort()->Register(OnUsart1Read);

    // 初始化为输出


    Sys.AddTask(LedTask, &led, 0, 500, "LedTask");

    Sys.Start();
}
