#include "App\lcd_dr.h"

//#define _LCD_DT_TEST_CPP

#ifdef _LCD_DT_TEST_CPP
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
		lcddr.DisplayStr(0,0,(uint8_t *)"ST7565Rtest");
		lcddr.Flush();
	}
#endif
