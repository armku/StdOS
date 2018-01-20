#ifndef _OCM240128_H
#define _OCM240128_H
#include "Device\Port.h"
class OCM240128
{
public:
	OCM240128();
	void SetPin(Pin ce = P0, Pin wr = P0, Pin rd = P0, Pin fs = P0, Pin led = P0, Pin sta0 = P0, Pin sta1 = P0, Pin sta3 = P0,Pin cd=P0);	
	void Init();//Һ����ʼ��
	void Clr();//	����
	void Clrchar(byte x, byte y, ushort count);
	void Displaydot8x16(byte x, byte y, byte *text, byte mode);
	void DispDot8x16(byte x, byte y, byte *text, byte mode);
	void DispDot16x16(byte x, byte y, byte *text, byte mode);
	void Draw_hline(byte x, byte y, ushort count);
	void Draw_vline(byte x, byte y, ushort count);
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
};

extern const byte  ascii_table_8x16[95][16];
extern const byte  hanzi_16x16[][32];
extern const byte  shuzi_16x16[][32];
extern const byte  GB3212[][32];
#endif
