#include "Drivers\lcd_dr.h"

#ifdef DEBUG1
	CLcd_DR lcddr(PD3, PD6, PD7, PB3, PB4);
	void CLcd_DRtest()
	{
		
		lcddr.Init();
		for(int i=0;i<10;i++)
		{
			lcddr.Test(0x55,0xAA);
			Sys.Sleep(500);
			lcddr.Test(0x00,0x00);
			Sys.Sleep(500);
		}
		lcddr.DisplayStr(0,0,(byte *)"ST7565Rtest");
		lcddr.Flush();
	}
#endif