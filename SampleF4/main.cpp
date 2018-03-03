#include "SerialPort.h"

#if 1
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
//    led3 = !led3;
//    led4 = !led4;
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

SerialPort gascom(COM3);
static char com4rx[1024],com4tx[1024];
uint OnUsart4Read(ITransport *transport, Buffer &bs, void *para, void *para2)
{
	bs.Show();
    return 0;
}
void gpsinit()
{
	gascom.Tx.SetBuf(com4tx,ArrayLength(com4tx));
	gascom.Rx.SetBuf(com4rx,ArrayLength(com4rx));
	gascom.SetBaudRate(38400);
	gascom.Register(OnUsart4Read);
	gascom.Open();
}
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
    
	gpsinit();
    Sys.Start();
}
