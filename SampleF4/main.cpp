#include "SerialPort.h"

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
void SerialTest(void *param)
{
	static int i=0;
	debug_printf("hello %4d\r\n",i++);
}
void w25q128test();
int main(void)
{
	TSys &sys = (TSys &)(Sys);
	sys.Init();
	#if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 	
	//W25Q64Test();
	//test();
	//w25q128test();
	Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");
	//Sys.AddTask(SerialTest,0,0,1000,"Serialtest");

    Sys.Start();
}
