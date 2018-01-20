#include "App\LCDOCM.h"
#include "Device\Port.h"
#include "Sys.h"

#define _OCM240128TEST
#ifdef  _OCM240128TEST

LCDOCM ocm;

OutputPort ocmd0;
OutputPort ocmd1;
OutputPort ocmd2;
OutputPort ocmd3;
OutputPort ocmd4;
OutputPort ocmd5;
OutputPort ocmd6;
OutputPort ocmd7;
void OCM240128Test()
{
	ocmd0.Invert = 0;
	ocmd1.Invert = 0;
	ocmd2.Invert = 0;
	ocmd3.Invert = 0;
	ocmd4.Invert = 0;
	ocmd5.Invert = 0;
	ocmd6.Invert = 0;
	ocmd7.Invert = 0;

	ocmd0.OpenDrain = false;
	ocmd1.OpenDrain = false;
	ocmd2.OpenDrain = false;
	ocmd3.OpenDrain = false;
	ocmd4.OpenDrain = false;
	ocmd5.OpenDrain = false;
	ocmd6.OpenDrain = false;
	ocmd7.OpenDrain = false;

	ocmd0.Set(PE8);
	ocmd1.Set(PE9);
	ocmd2.Set(PE10);
	ocmd3.Set(PE11);
	ocmd4.Set(PE12);
	ocmd5.Set(PE13);
	ocmd6.Set(PE14);
	ocmd7.Set(PE15);

	ocmd0.Open();
	ocmd1.Open();
	ocmd2.Open();
	ocmd3.Open();
	ocmd4.Open();
	ocmd5.Open();
	ocmd6.Open();
	ocmd7.Open();

	
	ocm.SetPin(PA6,PA7,PA5,PA1,PA0,PE8,PE9,PE11,PA4);
	ocm.Init();		 //液晶初始化
	ocm.Clr();		 //	清屏
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

	ocm.DisplayHz16x16(0,48,(byte*)"啊",0);
	ocm.DisplayHz16x16(2,48,(byte*)"阿",1);
	ocm.Display_string_16x16(0,64,(byte*)"啊阿埃",3,1);
	ocm.Display_string_16x16(1,80,(byte*)"啊阿埃",3,0);
	
	ocm.Draw_hline(1, 112,10);
	ocm.Draw_vline(26,16,30);
}

//高8位的数据
void OCM240128::writedata(byte da)
{
	ocmd0 = da & (1 << 0);
	ocmd1 = da & (1 << 1);
	ocmd2 = da & (1 << 2);
	ocmd3 = da & (1 << 3);
	ocmd4 = da & (1 << 4);
	ocmd5 = da & (1 << 5);
	ocmd6 = da & (1 << 6);
	ocmd7 = da & (1 << 7);
}

#endif //  _OCM240128TEST
