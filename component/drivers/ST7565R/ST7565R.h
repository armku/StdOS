#ifndef _ST7565R_H
#define _ST7565R_H
/*
液晶屏 128×64（长×宽）
左上角坐标为（0，0），右下角坐标为(127,63)
 */

#include "../../../StdOS/Core/Type.h"
#include "../../../Bsp/Porting.h"

class CLcd
{
public:
	CLcd();
	void SetPin(Pin pinrs, Pin pinsclk, Pin pinsid, Pin pinres, Pin pincs);

	void Init(); //初始化
	void Cls(uint16_t x0 = 0, uint16_t y0 = 0, uint16_t width = 128, uint16_t height = 64); //清理指定位置
	void Test(uint8_t data1, uint8_t data2);
	void Point(uint16_t x0, uint16_t y0, uint16_t color = 1); //画点,为所有的基础操作
	void DisplayImage(uint8_t *pbuf, uint16_t color = 1, uint32_t showtype = 1); //画图形
	void Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	void Rect(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height); //矩形
	void RectSolid(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height); //实心矩形
	void Circle(uint16_t x0, uint16_t y0, uint16_t r); //画圆
	void Display4x8(uint16_t x, uint16_t y, uint8_t *dp);
	void Display5x8(uint16_t x, uint16_t y, uint8_t *dp);
	void Display6x12(uint16_t x, uint16_t y, uint8_t *dp);
	void Display6x16(uint16_t x, uint16_t y, uint8_t *dp);
	void Display8x16(uint16_t x, uint16_t y, uint8_t *dp);
	void Display12x12(uint16_t x, uint16_t y, uint8_t *dp);
	void Display16x16(uint16_t x, uint16_t y, uint8_t *dp);
	void Display16x32(uint16_t x, uint16_t y, uint8_t *dp);
	void Display32x32(uint16_t x, uint16_t y, uint8_t *dp);
	void Flush(); //显示缓冲区刷新到屏        
	void ShadowReset(); //阴影部分复位
	void ShadowOn(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t id = 0); //阴影启用，默认启用通道0
	struct
	{
		uint16_t x;
		uint16_t y;
		uint16_t width;
		uint16_t height;
		bool enable;
	} ShadowRect[3]; //3组阴影区域，实现特定动作。

protected:
	void writeData(uint8_t data1);
	void writeCMD(uint8_t data1);
	void SetAddress(uint8_t page, uint8_t column); //写入地址 页 列    
	bool inShadow(uint16_t x, uint16_t y); //是否在阴影区域
	uint16_t readPoint(uint16_t x, uint16_t y); //读取颜色
	uint8_t Interface_Table[8][128]; //显示缓冲区		        
private:
	mcuGpio pPinRS;
	mcuGpio pPinsclk;
	mcuGpio pPinsid;
	mcuGpio pPinres;
	mcuGpio pPincs;
};
#endif
