#include "Drivers\ST7565R.h"

#ifdef DEBUG1
	CLcd lcddr1(PD3, PD6, PD7, PB3, PB4);
	void ST7565Rtest()
	{
		
		lcddr1.Init();
		for(int i=0;i<10;i++)
		{
			lcddr1.Test(0x55,0xAA);
			Sys.Sleep(500);
			lcddr1.Test(0x00,0x00);
			Sys.Sleep(500);
		}
		lcddr1.Test(0x55,0xAA);
	}
#endif

