#include <stdio.h>
#include "SerialPort.h"
#include "W24xxx.h"
#include "Button.h"
#include "Port.h"
#include "WatchDog.h"
#include "CString.h"
#include "string.h"
#include "DateTime.h"
#include "List.h"
#include "Pwm.h"
#include "Modbus.h"
#include "Spi.h"
#include "HardRtc.h"
#include "Flash.h"
#include "stmflash.h"

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

OutputPort led1(PB0, true);
OutputPort led2(PF7, true);

//按键 PC13 PA0
InputPort exti(PC13); //PA1 PB3     PA0 PC13
InputPort exti1(PA0);
#define _PIN_NAME(pin) ('A' + (pin >> 4)), (pin & 0x0F)
void OnKeyPress(Pin pin, bool down, void *param)
{
    //led1.Write(onoff);
    led2 = !led2;
    printf("中断引脚：P%c%d 值%d \r\n", _PIN_NAME(pin), down);
}

/*
ISO-V2:PB5控制485方向
ISO-V3:PC2控制485方向
 */
OutputPort rs485(PC2);
ModbusSlave ModbusSlave;
static uint OnUsartRead(ITransport *transport, Buffer &bs, void *para)
{
    ModbusSlave.Process(bs, para);
    return 0;
}

SerialPort sp2(COM2);
void LedTask(void *param)
{
    OutputPort *leds = (OutputPort*)param;
    *leds = ! * leds;
}

void TestAT45DB();
#define namee "StdOS"
int main(void)
{
    TSys &sys = (TSys &)(Sys);
    #if 0
        sys.Codec = codec;
	#endif
	
    sys.Name = (char*)namee;    
    //Rtc提取时间
    HardRtc *Rtc = HardRtc::Instance();
    Rtc->LowPower = false;
    Rtc->External = false;
    Rtc->Init();
    Rtc->Start(false, false);
    sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();

        WatchDog::Start(20000, 10000);
    #else 
        WatchDog::Start();
    #endif 
    #if 0
        //flash 最后一块作为配置区
        Config::Current = &Config::CreateFlash();
    #endif 

    sp2.RS485 = &rs485;
    rs485 = 0;
    sp2.Register(OnUsartRead);
    sp2.Open();
    //SerialPort::GetMessagePort()->Register(OnUsartRead);

    exti.Register(OnKeyPress);
    exti1.Register(OnKeyPress);

    //PWM pwm1(PC9);
	PWM pwm1(PB1);
    pwm1.Init();
    pwm1.SetOutPercent(50);

    // 初始化为输出
    OutputPort led(PF8);
    
    Sys.AddTask(LedTask, &led, 0, 500, "LedTask");
	
	//Flash::TestFlash();
	STMFLASH::Test();
    //Buffer::Test();
    Sys.Start();
}
