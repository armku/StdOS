#include "App\LCDOCM.h"

//#define _OCM240128_TEST_CPP
#ifdef  _OCM240128_TEST_CPP

LCDOCM ocm;
void OCM240128Test()
{	
	ocm.SetPin(PA6,PA7,PA5,PA1,PA0,PE8,PE9,PE11,PA4);
	ocm.SetDataPin(PE8,PE9,PE10,PE11,PE12,PE13,PE14,PE15);
	ocm.Init();		 //液晶初始化
	ocm.Clr();		 //	清屏
	ocm.DispStrAsc8x16(0,0,(uint8_t*)"1234567890ABCDEFabcd",1,1);
	ocm.DispStrAsc8x16(0,16,(uint8_t*)"1234567890ABCDEFabcd",1,0);		

	ocm.DispAscNum16x16(0,32,0);	
	ocm.DispAscNum16x16(2,32,1);
	ocm.DispAscNum16x16(4,32,2);
	ocm.DispAscNum16x16(6,32,3);
	ocm.DispAscNum16x16(8,32,4);
	ocm.DispAscNum16x16(10,32,5);
	ocm.DispAscNum16x16(12,32,6);
	ocm.DispAscNum16x16(14,32,7);
	ocm.DispAscNum16x16(16,32,8);
	ocm.DispAscNum16x16(18,32,9);
	ocm.DispAscNum16x16(20,32,10);

	ocm.DisplayHz16x16(0,48,(uint8_t*)"啊",0);
	ocm.DisplayHz16x16(2,48,(uint8_t*)"阿",1);
	ocm.DispStrHz16x16(0,64,(uint8_t*)"啊阿埃",3,1);
	ocm.DispStrHz16x16(1,80,(uint8_t*)"啊阿埃",3,0);
	
	ocm.Draw_hline(1, 112,10);
	ocm.Draw_vline(26,16,30);
}

#endif //  _OCM240128TEST
