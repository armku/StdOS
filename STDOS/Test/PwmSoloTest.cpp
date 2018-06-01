#include "Device\PwmSolo.h"

#define _PWMSOLO_TEST_CPP
#ifdef _PWMSOLO_TEST_CPP

PwmSolo pwm(PA6, 10000, 2700);

void PwmTestRoutin123(void *param)
{
	static int i = 0;
	if (i++ > 100)
		i = 0;
	pwm.SetOutPercent(i);
}

void PwmSoloTestInit()
{
	pwm.SetOutPercent(0);
	Sys.AddTask(PwmTestRoutin123,0,0,100,"pwmtest");
}
#endif // _PWMSOLO_TEST_CPP
