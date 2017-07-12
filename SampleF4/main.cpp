#include "SerialPort.h"

OutputPort led1(PF9,false);
OutputPort led2(PF10,false);

void LedTask(void *param)
{
    OutputPort *leds = (OutputPort*)param;
    *leds = ! * leds;
    //    led2 = key0;
}
int main(void)
{
	TSys &sys = (TSys &)(Sys);
	sys.Init();
    #if DEBUG
        Sys.MessagePort = COM1;
        Sys.ShowInfo();
    #endif 	
	//W25Q64Test();
	Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");

    Sys.Start();
}
