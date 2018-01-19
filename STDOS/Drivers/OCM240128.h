#ifndef _OCM240128_H
#define _OCM240128_H
#include "Device\Port.h"
class OCM240128
{
public:
	OCM240128();
	void SetPin(Pin ce = P0, Pin wr = P0, Pin rd = P0, Pin fs = P0, Pin led = P0, Pin sta1 = P0, Pin sta2 = P0, Pin sta3 = P0);
	void LCD_DataPort_Out();//将数据总线定义为输出
	void LCD_CMDPort_Out();//将命令总线定义为输出
	void LCD_DataPort_In();
	void NOP();//延时函数
	void LCD_busy_check(byte autowr);//检测忙状态
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
	void LCD_Init();//液晶初始化
	void LCD_Clr();//	清屏
	void LCD_Clr_char(byte x, byte y, ushort count);
private:
	OutputPort pinwr;
	OutputPort pinrd;
	OutputPort pince;
	OutputPort pinfs;
	OutputPort pinled;
	OutputPort pinsta1;
	OutputPort pinsta2;
	OutputPort pinsta3;
};
extern const byte  ascii_table_8x16[95][16];
extern const byte  hanzi_16x16[][32];
extern const byte  shuzi_16x16[][32];
extern const byte  GB3212[][32];
#endif
