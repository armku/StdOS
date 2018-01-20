#include "Drivers\PCF8563.h"
#include "stdio.h"
#include "Sys.h"

#define _PCF8563TEST
#ifdef _PCF8563TEST

PCF8563 pcf;

DateTime pcfnow;

void pcfroutin(void * param)
{
	pcf.GetDateTime(pcfnow);
	//pcfnow.Second = pcf.readaddr(2);
	pcfnow.Show();
}
void PCF8563Test()
{
	pcf.IIC.SetPin(PB5,PB6);
	pcf.IIC.Open();
	
	Sys.AddTask(pcfroutin, 0, 2000, 2000, "pcfrou");
}
#endif // _PCF8563TEST
