#ifndef _LCDOCM_H
#define _LCDOCM_H
#include "Drivers\OCM240128.h"
class LCDOCM :public OCM240128
{
public:
	void Display_string_8x16(byte x, byte y, byte *text, byte num, byte mode);
	void Display_string_16x16(byte x, byte y, byte *text, byte num, byte mode);
	void Display_shuzi_16x16(byte x, byte y, byte text);
	void DisplayHz16x16(byte x, byte y, byte *text, byte mode);
	void DisplayAsc8x16(byte x, byte y, byte *text, byte mode);
};

#endif // _LCDOCM_H
