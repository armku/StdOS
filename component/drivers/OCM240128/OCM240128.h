#ifndef _OCM240128_H
#define _OCM240128_H
#include "OnChip\Port.h"
class OCM240128
{
public:
	OCM240128();
	void SetPin(Pin ce = P0, Pin wr = P0, Pin rd = P0, Pin fs = P0, Pin led = P0, Pin sta0 = P0, Pin sta1 = P0, Pin sta3 = P0,Pin cd=P0);	
	void SetDataPin(Pin d0, Pin d1, Pin d2, Pin d3, Pin d4, Pin d5, Pin d6, Pin d7);
	void Init();//“∫æß≥ı ºªØ
	void Clr();//	«Â∆¡
		
	void DispDot8x16(uint8_t x, uint8_t y, uint8_t *text, uint8_t mode);
	void DispDot16x16(uint8_t x, uint8_t y, uint8_t *text, uint8_t mode);
	void Draw_hline(uint8_t x, uint8_t y, uint16_t count);
	void Draw_vline(uint8_t x, uint8_t y, uint16_t count);
private:
	void busycheck(uint8_t autowr);//ºÏ≤‚√¶◊¥Ã¨
	void wcmd(uint8_t cmd);
	void wdata(uint8_t dat);
	void wcmd(uint8_t dat, uint8_t cmd);
	void wcmd2(uint8_t dat1, uint8_t dat2, uint8_t cmd);
	void writedata(uint8_t da);
private:
	mcuGpio pinwr;
	mcuGpio pinrd;
	mcuGpio pince;
	mcuGpio pinfs;
	mcuGpio pinled;
	mcuGpio pinsta0;
	mcuGpio pinsta1;
	mcuGpio pinsta3;
	mcuGpio pincd;

	mcuGpio data0;
	mcuGpio data1;
	mcuGpio data2;
	mcuGpio data3;
	mcuGpio data4;
	mcuGpio data5;
	mcuGpio data6;
	mcuGpio data7;
};

#endif
