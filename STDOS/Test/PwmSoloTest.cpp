#include "PwmSolo.h"
#include "stm32f4xx.h" 

#ifdef DEBUG
	PwmSolo pwm(10000, 2700,false);
	PwmSolo pwm1[3];
	void TIM14_PWM_Init(u32 arr,u32 psc);
	void PwmSoloTest()
	{
		#ifdef STM32F1
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
		#elif defined STM32F4
		
		pwm.Set(PF8);
		pwm.Open();
		pwm.SetOutPercent(85);
		#endif		
	}
#endif
