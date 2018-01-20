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
	this->DispDot16x16(x,y,(byte*)shuzi_16x16[text],0);
}


/************************************************************************************************
@f_name: void Display_char_8x16(byte hh,ushort page,ushort column,byte text)
@brief:	 显示字符
@param:	 None
@return: None
************************************************************************************************/
void LCDOCM::DisplayHz16x16(byte x, byte y, byte *text, byte mode)
{
	byte qh, ql;
	ushort add1;

	qh = *text;
	ql = *(text + 1);
	if (qh < 58)
		add1 = qh - 48;
	else
		add1 = ((qh - 0xb0) * 94 + (ql - 0x96));
	this->DispDot16x16(x, y, (byte*)GB3212[add1], mode);
}


