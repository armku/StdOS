#include "PwmSolo.h"

#ifdef DEBUG
	PwmSolo pwm(10000, 2700,false);
	void PwmSoloTest()
	{
		pwm.Set(PA6);
		pwm.Open();
		pwm.SetOutPercent(5);
	}
#endif
