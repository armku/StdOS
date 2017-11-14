#include "PwmSolo.h"

#ifdef DEBUG
	PwmSolo pwm(10000, 2700,false);
	PwmSolo pwm1[3];
	void PwmSoloTest()
	{
		pwm.Set(PA3);
		pwm.Open();
		pwm.SetOutPercent(5);
		
		pwm1[0].Set(PA2);
		pwm1[1].Set(PA6);
		pwm1[2].Set(PA7);
		for(int i=0;i<3;i++)
		{
			pwm1[i].Open();
			pwm1[i].SetOutPercent(i*10+10);
		}
	}
#endif
