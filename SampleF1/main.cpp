#include "SerialPort.h"
#include "Drivers\AT24CXX.h"
#include "Drivers\W25QXXX.h"
#include "SString.h"
#include "string.h"
#include "List.h"
#include "Spi.h"
#include "Flash.h"
#include "Drivers\lcd_dr.h"
#include "TInterrupt.h"
#include "Timer.h"
#include "TTime.h"
#include "Drivers\RX8025T.h"


class TTTTime
{
    public:
        uint Seconds; // 全局秒数，系统启动后总秒数。累加
        UInt64 Milliseconds; // 全局毫秒数，系统启动后总毫秒（1000ms整部分）。累加
        uint CurrentTicks; // 当前滴答时钟
        UInt64 Current; // 当前毫秒数
};
TTTTime now123;
//InputPort paa11(PA11,false);
OutputPort paaa1(PA11);
void Test12(void *param)
{
    //	int aa=paa11;aa++;
    now123.Seconds = Time.Seconds;
    now123.Milliseconds = Time.Milliseconds;
    now123.Current = Time.Current();
    now123.CurrentTicks = Time.CurrentTicks();
}

#if 1
    OutputPort led1(PB0, false);
    OutputPort led2(PF7, false);
    OutputPort led3(PF8, false);
#else 
    OutputPort led1(PD0, false);
    OutputPort led2(PD1, false);
    OutputPort led3(PD2, false);
    OutputPort ledLCD(PD12, false);
#endif 

void LedTask(void *param)
{
    auto leds = (OutputPort*)param;
    *leds = ! * leds;
    //    led2 = key0;
    //paaa1=!paaa1;
    paaa1 = 0;
}
uint time6cnt;
uint OnUsart1Read(ITransport *transport, Buffer &bs, void *para, void *para2)
{
	time6cnt++;
    //transport->Write(bs);
	bs.Show(true);
    return 0;
}

uint OnUsart3Read(ITransport *transport, Buffer &bs, void *para, void *para2)
{
	time6cnt++;
    //transport->Write(bs);
	bs.Show(true);
    return 0;
}


//void TimerTask(void *param)
//{
////    static int i = 0;
////    debug_printf("\r\n%d: cnt:%d", i++, time6cnt);
//}

//SerialPort *sp2;
//SerialPort *sp3;

void InterruptTest();
void ADS1232Test();
void ModbusTest();
void streamtest();

void ESP8266Test();
void ssd1309Test();
void ds18b20test();
void ST7565Rtest();
void keyTest();
void FifoTest();
void SerialPortTest();
void PwmSoloTest();
void WaitHandleTest();
void StrintTest();
void n24l01Test();
void BlinkPortTest();
void FlushPortTest();

//void Test(void * param)
//{
//	auto aa="helloworld";
//	Buffer bs((char *)aa,ArrayLength(aa));
////	sp2->Write(bs);
//	bs.Show();
//}
void Test(void * param)
{
	int cur=Time.Current();	
	Sys.Sleep(20);
	int cur2=Time.Current();
	debug_printf("\r\n aa:%ld--%ld %ld",cur,cur2,cur2-cur);
}

int main(void)
{
    Sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 
	

    SerialPort::GetMessagePort()->Register(OnUsart1Read);
		
//	sp3=new SerialPort(COM3);
//	sp3->Register(OnUsart3Read);
//	sp3->Open();
	
    //Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
//    Sys.AddTask(TimerTask, &led1, 0, 1000, "TimerTask");
//	Sys.AddTask(Test,0,2000,2000,"Test");
    //Sys.AddTask(Test12, 0, 600, 1000, "Test");
	Sys.AddTask(Test, 0, 600, 1000, "Test");
	
	//StrintTest();
    //IList::Test();
    //ADS1232Test();
    //streamtest();
    //ModbusTest();
    //InterruptTest();
	//ssd1309Test();
    //ESP8266Test();
	//ds18b20test();
	//n24l01Test();
	//keyTest();
	//SerialPortTest();
	//ST7565Rtest();
	//PwmSoloTest();
	//WaitHandleTest();
	//BlinkPortTest();
	FlushPortTest();
    Sys.Start();
}
