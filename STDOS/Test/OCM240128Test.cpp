#include "App\LCDOCM.h"

#define _OCM240128TEST
#ifdef  _OCM240128TEST

LCDOCM ocm;
void OCM240128Test()
{	
	ocm.SetPin(PA6,PA7,PA5,PA1,PA0,PE8,PE9,PE11,PA4);
	ocm.SetDataPin(PE8,PE9,PE10,PE11,PE12,PE13,PE14,PE15);
	ocm.Init();		 //“∫æß≥ı ºªØ
	ocm.Clr();		 //	«Â∆¡
	ocm.Display_string_8x16(0,0,(byte*)"1234567890ABCDEFabcd",1,1);
	ocm.Display_string_8x16(0,16,(byte*)"1234567890ABCDEFabcd",1,0);		

	ocm.Display_shuzi_16x16(0,32,0);	
	ocm.Display_shuzi_16x16(2,32,1);
	ocm.Display_shuzi_16x16(4,32,2);
	ocm.Display_shuzi_16x16(6,32,3);
	ocm.Display_shuzi_16x16(8,32,4);
	ocm.Display_shuzi_16x16(10,32,5);
	ocm.Display_shuzi_16x16(12,32,6);
	ocm.Display_shuzi_16x16(14,32,7);
	ocm.Display_shuzi_16x16(16,32,8);
	ocm.Display_shuzi_16x16(18,32,9);
	ocm.Display_shuzi_16x16(20,32,10);

	ocm.DisplayHz16x16(0,48,(byte*)"∞°",0);
	ocm.DisplayHz16x16(2,48,(byte*)"∞¢",1);
	ocm.Display_string_16x16(0,64,(byte*)"∞°∞¢∞£",3,1);
	ocm.Display_string_16x16(1,80,(byte*)"∞°∞¢∞£",3,0);
	
	ocm.Draw_hline(1, 112,10);
	ocm.Draw_vline(26,16,30);
}

#endif //  _OCM240128TEST
