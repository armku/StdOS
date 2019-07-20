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
		
	void DispDot8x16(uint8_t x, uint8_t y, uint8_t *text, uint8_t mode);
	void DispDot16x16(uint8_t x, uint8_t y, uint8_t *text, uint8_t mode);
	void Draw_hline(uint8_t x, uint8_t y, uint16_t count);
	void Draw_vline(uint8_t x, uint8_t y, uint16_t count);
	void Test();
private:
	void busycheck(uint8_t autowr);//ºÏ≤‚√¶◊¥Ã¨
	void wcmd(uint8_t cmd);
	void wdata(uint8_t dat);
	void wcmd(uint8_t dat, uint8_t cmd);
	void wcmd2(uint8_t dat1, uint8_t dat2, uint8_t cmd);
	void writedata(uint8_t da);
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
