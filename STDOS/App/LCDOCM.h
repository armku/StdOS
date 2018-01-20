#ifndef _LCDOCM_H
#define _LCDOCM_H
#include "Drivers\OCM240128.h"
class LCDOCM :public OCM240128
{
public:
	void DispStrAsc8x16(byte x, byte y, byte *text, byte num, byte mode);
	void DispStrHz16x16(byte x, byte y, byte *text, byte num, byte mode);
	void DispAscNum16x16(byte x, byte y, byte text);
	void DisplayHz16x16(byte x, byte y, byte *text, byte mode);
	void DisplayAsc8x16(byte x, byte y, byte *text, byte mode);
	void Clrchar(byte x, byte y, ushort count);
};

#endif // _LCDOCM_H
