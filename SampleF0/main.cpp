#include "Timer.h"
#include "SerialPort.h"
#include "Drivers\AT24CXX.h"
#include "Drivers\W25QXXX.h"
#include "stm32f0xx.h"

const byte vers[] = "yyyy-MM-dd HH:mm:ss";
OutputPort led1(PC6, true);
OutputPort led2(PC7, true);
OutputPort led3(PC8, true);
OutputPort led4(PC9, true);

OutputPort ledss[] = 
{
    led1, led2, led3, led4
};

void LedTask(void *param)
{
    static byte flag = 1;
    OutputPort *leds = (OutputPort*)param;

    flag <<= 1;
    flag &= 0x0f;
    if (flag == 0)
    {
        flag = 1;
    }

    leds[0] = flag &0x01;
    leds[1] = flag &0x02;
    leds[2] = flag &0x04;
    leds[3] = flag &0x08;

    //    led2 = key0;
}

#define namee "StdOS"
//void AT24C02Test();
uint OnUsart1Read(ITransport *transport, Buffer &bs, void *para, void *para2)
{
    bs.Show(true);
    return 0;
}

uint time6cnt;
void TimerTask(void *param)
{
    static int i = 0;
    printf("\r\n%d: cnt:%d", i++, time6cnt);
}
void TimeTest();
int main()
{
    Sys.Name = (char*)namee;
    Sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 
    //	AT24C02Test();  
	TimeTest();
    Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
    Sys.AddTask(TimerTask, &led1, 0, 1000, "TimerTask");

    Sys.Start();
}

Delegate < Timer & > abc;
void tim2refesh(void *param)
{
    time6cnt++;
}

Timer *timer2;
void TimeTest()
{
    // 初始化为输出
    timer2 = new Timer(Timer6);
    abc.Bind(tim2refesh);
    timer2->Register(abc);
    timer2->Open();
    //        timer2->SetFrequency(1000);
    //        timer2->Config();
}
