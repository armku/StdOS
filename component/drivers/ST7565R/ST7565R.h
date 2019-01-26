#ifndef _ST7565R_H
#define _ST7565R_H
/*
Һ���� 128��64��������
���Ͻ�����Ϊ��0��0�������½�����Ϊ(127,63)
 */

#include "../../../StdOS/Core/Type.h"
#include "../../../Bsp/Porting.h"

class CLcd
{
public:
	CLcd();
	void SetPin(Pin pinrs, Pin pinsclk, Pin pinsid, Pin pinres, Pin pincs);

	void Init(); //��ʼ��
	void Cls(uint16_t x0 = 0, uint16_t y0 = 0, uint16_t width = 128, uint16_t height = 64); //����ָ��λ��
	void Test(uint8_t data1, uint8_t data2);
	void Point(uint16_t x0, uint16_t y0, uint16_t color = 1); //����,Ϊ���еĻ�������
	void DisplayImage(uint8_t *pbuf, uint16_t color = 1, uint32_t showtype = 1); //��ͼ��
	void Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	void Rect(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height); //����
	void RectSolid(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height); //ʵ�ľ���
	void Circle(uint16_t x0, uint16_t y0, uint16_t r); //��Բ
	void Display4x8(uint16_t x, uint16_t y, uint8_t *dp);
	void Display5x8(uint16_t x, uint16_t y, uint8_t *dp);
	void Display6x12(uint16_t x, uint16_t y, uint8_t *dp);
	void Display6x16(uint16_t x, uint16_t y, uint8_t *dp);
	void Display8x16(uint16_t x, uint16_t y, uint8_t *dp);
	void Display12x12(uint16_t x, uint16_t y, uint8_t *dp);
	void Display16x16(uint16_t x, uint16_t y, uint8_t *dp);
	void Display16x32(uint16_t x, uint16_t y, uint8_t *dp);
	void Display32x32(uint16_t x, uint16_t y, uint8_t *dp);
	void Flush(); //��ʾ������ˢ�µ���        
	void ShadowReset(); //��Ӱ���ָ�λ
	void ShadowOn(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t id = 0); //��Ӱ���ã�Ĭ������ͨ��0
	struct
	{
		uint16_t x;
		uint16_t y;
		uint16_t width;
		uint16_t height;
		bool enable;
	} ShadowRect[3]; //3����Ӱ����ʵ���ض�������

protected:
	void writeData(uint8_t data1);
	void writeCMD(uint8_t data1);
	void SetAddress(uint8_t page, uint8_t column); //д���ַ ҳ ��    
	bool inShadow(uint16_t x, uint16_t y); //�Ƿ�����Ӱ����
	uint16_t readPoint(uint16_t x, uint16_t y); //��ȡ��ɫ
	uint8_t Interface_Table[8][128]; //��ʾ������		        
private:
	mcuGpio pPinRS;
	mcuGpio pPinsclk;
	mcuGpio pPinsid;
	mcuGpio pPinres;
	mcuGpio pPincs;
};
#endif
