#include "SerialPort.h"
#include "Timer.h"

OutputPort led1(PF9,false);
OutputPort led2(PF10,false);
InputPort key0(PE4,true);
InputPort key1(PE3,true);
InputPort key2(PE2,true);
InputPort keyup(PA0,true,InputPort::DOWN);

void LedTask(void *param)
{
    OutputPort *leds = (OutputPort*)param;
    *leds = ! * leds;	
    led2 = keyup;
}

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
int main(void)
{
	Sys.Init();
	#if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 		
	SerialPort::GetMessagePort()->Register(OnUsart1Read);
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
    timer2 = new Timer(Timer3);
    abc.Bind(tim2refesh);
    timer2->Register(abc);
    timer2->Open();
    timer2->SetFrequency(1000);
    //        timer2->Config();
}
