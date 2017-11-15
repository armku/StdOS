#include "SerialPort.h"
#include "Timer.h"
#include "Spi.h"

#if 0
    //正点原子开发板F4
    OutputPort led1(PF9, false);
    OutputPort led2(PF10, false);
#else 
    //安福莱开发板F4-V5
    OutputPort led1(PI10, false);
    OutputPort led2(PF7, false);
    OutputPort led3(PC2, false);
    OutputPort led4(PC2, false);
#endif 
void LedTask(void *param)
{
    OutputPort *leds = (OutputPort*)param;
    *leds = ! * leds;
    led2 = !led2;
    led3 = !led3;
    led4 = !led4;
    //led2 = keyup;
}

uint OnUsart1Read(ITransport *transport, Buffer &bs, void *para, void *para2)
{
    bs.Show();
	//transport->Write(bs);
    return 0;
}

uint time6cnt;
void TimerTask(void *param)
{
    //    static int i = 0;
    //    printf("\r\n%d: cnt:%d", i++, time6cnt);
}

void InterruptTest();
void w25q128test();
void ssd1309Test();
void ds18b20test();
void SRamTest();
void bsp_Init();
void PwmSoloTest();
int main(void)
{
    Sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 
    SerialPort::GetMessagePort()->Register(OnUsart1Read);
    Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
    Sys.AddTask(TimerTask, &led1, 0, 1000, "TimerTask");
    //InterruptTest();
    //w25q128test();  
	//ssd1309Test();
	//ds18b20test();
	//SRamTest();
	PwmSoloTest();
	
    Sys.Start();
}
