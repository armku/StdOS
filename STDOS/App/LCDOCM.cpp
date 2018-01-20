#include "LCDOCM.h"

void LCDOCM::Display_string_8x16(byte x, byte y, byte *text, byte num, byte mode)
{
	while (*text != 0)//数据未结束
	{
		this->Displaydot8x16(x, y, text, mode);
		text += 1;
		x += 1;
	}
}
void LCDOCM::Display_string_16x16(byte x, byte y, byte *text, byte num, byte mode)
{
	while (*text != 0)//数据未结束
	{
		this->DisplayHz16x16(x, y, text, mode);
		text += 2;
		x += 2;
	}
}
void LCDOCM::Display_shuzi_16x16(byte x, byte y, byte text)
{
	this->DispDot16(x,y,(byte*)shuzi_16x16[text],0);
}


