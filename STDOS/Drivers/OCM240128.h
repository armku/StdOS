#ifndef _OCM240128_H
#define _OCM240128_H
#include "Device\Port.h"
class OCM240128
{
public:
	OCM240128();
	void SetPin(Pin ce = P0, Pin wr = P0, Pin rd = P0, Pin fs = P0, Pin led = P0, Pin sta0 = P0, Pin sta1 = P0, Pin sta3 = P0,Pin cd=P0);	
	void Init();//“∫æß≥ı ºªØ
	void Clr();//	«Â∆¡
		
	void DispDot8x16(byte x, byte y, byte *text, byte mode);
	void DispDot16x16(byte x, byte y, byte *text, byte mode);
	void Draw_hline(byte x, byte y, ushort count);
	void Draw_vline(byte x, byte y, ushort count);
private:
	void busycheck(byte autowr);//ºÏ≤‚√¶◊¥Ã¨
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

#endif
