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
extern const byte  ascii_table_8x16[95][16];
extern const byte  hanzi_16x16[][32];
extern const byte  shuzi_16x16[][32];
extern const byte  GB3212[][32];
#endif // _LCDOCM_H
