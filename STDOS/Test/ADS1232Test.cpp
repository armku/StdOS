#include "Drivers\ADS1232.h"

//≤‚ ‘1232
#ifdef DEBUG
ADS1232 ads1232;
int ads1232doutcnt;
float ads1232value;
void InputPort_OpenEXTI(Pin pin,InputPort::Trigger trigger);
void SetEXIT(int pinIndex, bool enable,InputPort::Trigger trigger=InputPort::Both);
void OnPress1232(InputPort &port, bool down)
{
	SetEXIT(PB14,true,InputPort::Falling);
    //debug_printf("Press P%c%d down=%d\r\n", _PIN_NAME(port._Pin), down);
	ads1232doutcnt++;
	ads1232value=ads1232.Read();
	InputPort_OpenEXTI(PB14,InputPort::Falling);
}
InputPort key1232dout(PB14, true, InputPort::DOWN);

void Show1232cnt(void* param)
{
	debug_printf("\r\n ADS1232CNT:%d value:%f",ads1232doutcnt);
	ads1232.test();
}
void ADS1232Test()
{
	ads1232.SetPin(PB14,PB13,PD5);
	ads1232.Init();
	
	key1232dout.Press = OnPress1232;
    key1232dout.UsePress();
    key1232dout.Open();
	
	Sys.AddTask(Show1232cnt,0,1000,1000,"Show1232cnt");
}
#endif
