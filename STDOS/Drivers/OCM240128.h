#ifndef _OCM240128_H
#define _OCM240128_H
#include "Sys.h"
class OCM240128
{
public:
	OCM240128();
	void LCD_DataPort_Out();//���������߶���Ϊ���
	void LCD_CMDPort_Out();//���������߶���Ϊ���
	void LCD_DataPort_In();
	void NOP();//��ʱ����
	void LCD_busy_check(byte autowr);//���æ״̬
	void LCD_Wcmd(byte cmd);
	void LCD_Wdata(byte dat);
	void LCD_Wdata_cmd(byte dat, byte cmd);
	void LCD_Wdata2_cmd(byte dat1, byte dat2, byte cmd);
	void Display_char_8x16(byte x, byte y, byte *text, byte mode);
	void Display_string_8x16(byte x, byte y, byte *text, byte num, byte mode);
	void Display_str_16x16(byte x, byte y, byte *text, byte mode);
	void Display_string_16x16(byte x, byte y, byte *text, byte num, byte mode);
	void Display_shuzi_16x16(byte x, byte y, byte text);
	void Draw_hline(byte x, byte y, ushort count);
	void Draw_vline(byte x, byte y, ushort count);
	void LCD_TEST();
	void LCD_Init();//Һ����ʼ��
	void LCD_Clr();//	����
	void LCD_Clr_char(byte x, byte y, ushort count);
private:

};
extern const byte  ascii_table_8x16[95][16];
extern const byte  hanzi_16x16[][32];
extern const byte  shuzi_16x16[][32];
extern const byte  GB3212[][32];
#endif
