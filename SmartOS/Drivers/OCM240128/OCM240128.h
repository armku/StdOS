#ifndef _OCM240128_H
#define _OCM240128_H
#include "OnChip\Port.h"

#define NULL 0
class OCM240128
{
public:
	OCM240128();
	void SetPin(Pin ce = P0, Pin wr = P0, Pin rd = P0, Pin fs = P0, Pin led = P0, Pin cd=P0);	
	void SetDataPin(Pin d0, Pin d1, Pin d2, Pin d3, Pin d4, Pin d5, Pin d6, Pin d7);
	void Init();//“∫æß≥ı ºªØ
	void Clr();//	«Â∆¡
		
	void DispDot8x16(byte x, byte y, byte *text, byte mode);
	void DispDot16x16(byte x, byte y, byte *text, byte mode);
	void Draw_hline(byte x, byte y, ushort count);
	void Draw_vline(byte x, byte y, ushort count);
	void Test();
private:
	void busycheck(byte autowr);//ºÏ≤‚√¶◊¥Ã¨
	void wcmd(byte cmd);
	void wdata(byte dat);
	void wcmd(byte dat, byte cmd);
	void wcmd2(byte dat1, byte dat2, byte cmd);
	void writedata(byte da);
private:
	Port pinwr;
	Port pinrd;
	Port pince;
	/*Port pinfs;*/
	Port pinled;
	Port pincd;

	Port data0;
	Port data1;
	Port data2;
	Port data3;
	Port data4;
	Port data5;
	Port data6;
	Port data7;
};

#endif
