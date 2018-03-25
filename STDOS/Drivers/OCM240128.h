#ifndef _OCM240128_H
#define _OCM240128_H
#include "Device\Port.h"
class OCM240128
{
public:
	OCM240128();
	void SetPin(Pin ce = P0, Pin wr = P0, Pin rd = P0, Pin fs = P0, Pin led = P0, Pin sta0 = P0, Pin sta1 = P0, Pin sta3 = P0,Pin cd=P0);	
	void SetDataPin(Pin d0, Pin d1, Pin d2, Pin d3, Pin d4, Pin d5, Pin d6, Pin d7);
	void Init();//Һ����ʼ��
	void Clr();//	����
		
	void DispDot8x16(byte x, byte y, byte *text, byte mode);
	void DispDot16x16(byte x, byte y, byte *text, byte mode);
	void Draw_hline(byte x, byte y, uint16_t count);
	void Draw_vline(byte x, byte y, uint16_t count);
private:
	void busycheck(byte autowr);//���æ״̬
	void wcmd(byte cmd);
	void wdata(byte dat);
	void wcmd(byte dat, byte cmd);
	void wcmd2(byte dat1, byte dat2, byte cmd);
	void writedata(byte da);
private:
	OutputPort pinwr;
	OutputPort pinrd;
	OutputPort pince;
	OutputPort pinfs;
	OutputPort pinled;
	OutputPort pinsta0;
	OutputPort pinsta1;
	OutputPort pinsta3;
	OutputPort pincd;

	OutputPort data0;
	OutputPort data1;
	OutputPort data2;
	OutputPort data3;
	OutputPort data4;
	OutputPort data5;
	OutputPort data6;
	OutputPort data7;
};

#endif
