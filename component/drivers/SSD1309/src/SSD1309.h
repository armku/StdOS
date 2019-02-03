#ifndef _SSD1309_H
#define _SSD1309_H
#include "../../../../StdOS/Kernel/Sys.h"
#include "../../../../Bsp/Porting.h"

class SSD1309
{
public:
	void SetPinSpi(Pin sclk, Pin sdin, Pin dc, Pin res = P0, Pin cs = P0);
	void DisplayOn();
	void DisplayOff();
	void Init();
	void Clear(char ch = 0x00);
	void ShowChar(uint8_t x, uint8_t y, uint8_t chr);
	void ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);
	void ShowString(uint8_t x, uint8_t y, char *p, uint8_t interval = 0);
	void ShowCHinese(uint8_t x, uint8_t y, char *hz);
	void DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]);
protected:
	void SetPos(uint8_t x, uint8_t y);
	void _wrcmd(uint8_t cmd);//Ð´ÃüÁî
	void _wrdata(uint8_t da);//Ð´Êý¾Ý			       
private:
	uint32_t oled_pow(uint8_t m, uint8_t n);
	uint16_t SearchhzIndex(uint8_t lo, uint8_t hi); //²éÕÒºº×Ö±àÂëÎ»ÖÃ
	void ShowCHinese11(uint8_t x, uint8_t y, uint8_t no);
	void WRByte(uint8_t dat, uint8_t cmd);
protected:
	//SPI
	mcuGpio _cs;
	mcuGpio _res;
	mcuGpio _dc;
	mcuGpio _sclk;
	mcuGpio _sdin;
	//8080
	mcuGpio _wr;
	mcuGpio _rd;
};
#endif
