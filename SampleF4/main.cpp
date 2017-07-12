#include "Sys.h"
#include "Device\Port.h"
//#include "Device\SerialPort.h"

void LedTask(void* param)
{
    auto leds	= (OutputPort*)param;
    *leds = !*leds;
}

void OnPress(InputPort& port, bool down)
{
    debug_printf("Press P%c%d down=%d\r\n", _PIN_NAME(port._Pin), down);
}

int main(void)
{
    // 初始化系统
    //Sys.Clock = 72000000;
    //Sys.MessagePort = COM1;
    Sys.Init();
    //Sys.InitClock();
    Sys.ShowInfo();

    // 初始化为输出
	OutputPort leds[] = {PD0, PD1};
	for(int i=0; i<ArrayLength(leds); i++)
		leds[i].Invert = true;

    // 初始化为中断输入
    Pin ips[] = { PE9, PE8, PE4, PE5, PE6, PE7};
    InputPort btns[6];
	for(int i=0; i<ArrayLength(btns); i++)
	{
		btns[i].Index = i;
		btns[i].Set(ips[i]);
		btns[i].Press = OnPress;
		btns[i].UsePress();
		btns[i].Open();
	}

	Sys.AddTask(LedTask, &leds[0], 500, 500, "Led闪烁");

	/*Buffer::Test();
	Array::Test();
	String::Test();
	DateTime::Test();
	IList::Test();
	IDictionary::Test();*/
	/*OutputPort power(PE2);
	power = true;
	SerialPort::Test();*/

    Sys.Start();
}
