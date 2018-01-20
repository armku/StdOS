#include "OCM240128.h"

OCM240128::OCM240128()
{
	this->pince.Invert = 0;
	this->pinwr.Invert = 0;
	this->pinrd.Invert = 0;
	this->pinfs.Invert = 0;
	this->pinled.Invert = 0;
	this->pinsta0.Invert = 0;
	this->pinsta1.Invert = 0;
	this->pinsta3.Invert = 0;
	this->pincd.Invert = 0;

	this->pince.OpenDrain = false;
	this->pinwr.OpenDrain = false;
	this->pinrd.OpenDrain = false;
	this->pinfs.OpenDrain = false;
	this->pinled.OpenDrain = false;
	this->pinsta0.OpenDrain = false;
	this->pinsta1.OpenDrain = false;
	this->pinsta3.OpenDrain = false;
	this->pincd.OpenDrain = false;
}

void OCM240128::SetPin(Pin ce, Pin wr, Pin rd, Pin fs, Pin led, Pin sta0, Pin sta1, Pin sta3,Pin cd)
{
	this->pince.Set(ce);
	this->pinwr.Set(wr);
	this->pinrd.Set(rd);
	this->pinfs.Set(fs);
	this->pinled.Set(led);
	this->pinsta0.Set(sta0);
	this->pinsta1.Set(sta1);
	this->pinsta3.Set(sta3);	
	this->pincd.Set(cd);

	this->pince.Open();
	this->pinwr.Open();
	this->pinrd.Open();
	this->pinfs.Open();
	this->pinled.Open();
	this->pinsta0.Open();
	this->pinsta1.Open();
	this->pinsta3.Open();
	this->pincd.Open();
}

extern const byte  ascii_table_8x16[95][16];
extern const byte  hanzi_16x16[][32];
extern const byte  shuzi_16x16[][32];
extern const byte  GB3212[][32];

/************************************************************************************************
@f_name: byte LCD12864_busy(void)
@brief:	 检测忙状态
@param:	 None
@return: None
************************************************************************************************/
void OCM240128::busycheck(byte autowr)
{
	#if 0
//	LCD_DataPort_In();
//	this->pincd = 1;
//	this->pinwr = 1;
//	this->pinrd = 0;
//	if (autowr)
//	{
//		while (this->pinsta3==0)
//		{
//			
//		}
//	}
//	else
//	{
//		while((this->pinsta0==0)||(this->pinsta1==0))
//		{
//			Sys.Delay(10);
//		};
//	}
//	this->pinrd = 1;
//	LCD_DataPort_Out();
	#else
	Sys.Delay(10);
	#endif
}
/************************************************************************************************
@f_name: void LCD12684_Wcmd_L(byte dat)
@brief:	 写命令
@param:	 byte dat  输入指令
@return: None
************************************************************************************************/
void OCM240128::wcmd (byte cmd)
{
	this->busycheck(0);
	this->pincd = 1;
	this->pinrd = 1;
	this->writedata(cmd);
	this->pinwr = 0;
	this->pinwr = 1;
}

/************************************************************************************************
@f_name: void LCD12684_Wcmd_R(byte dat)
@brief:	 写数据
@param:	 byte dat  输入指令
@return: None
************************************************************************************************/
void OCM240128::wdata(byte dat)
{
	this->busycheck(0);
	this->pincd = 0;
	this->pinrd = 1;
	this->writedata(dat);
	this->pinwr = 0;
	this->pinwr = 1;
}
/************************************************************************************************
@f_name: void LCD12684_Wdat_L(byte dat)
@brief:	 写一个数据和一个命令
@param:	 byte dat 输入数据
@return: None
************************************************************************************************/
void OCM240128::wcmd (byte dat,byte cmd)
{
	this->wdata(dat);
	this->wcmd(cmd);
}

/************************************************************************************************
@f_name: void LCD12684_Wdat_R(byte dat)
@brief:	 写两个数据一个命令
@param:	 byte dat 输入数据
@return: None
************************************************************************************************/
void OCM240128::wcmd2 (byte dat1,byte dat2,byte cmd)
{
	this->wdata(dat1);
	this->wdata(dat2);
	this->wcmd(cmd);
}

/************************************************************************************************
@f_name: void Display_char_8x16(byte hh,ushort page,ushort column,byte text)
@brief:	 显示字符
@param:	 None
@return: None
************************************************************************************************/
void OCM240128::Display_char_8x16(byte x,byte y,byte *text,byte mode)
{
	ushort add;
	byte i,j,k;
	add = y * 0x20 + x + 0x800;
	i=add;
	j=add>>8;
	for (k = 0; k < 16; k++)
  {
    this->wcmd2(i, j, 0x24);
		if(mode)
    this->wcmd(ascii_table_8x16[*text-32][k], 0xc0);
		else
		this->wcmd(~ascii_table_8x16[*text-32][k], 0xc0);	
    add = add + 0x20;
		i=add;
		j=add>>8;
  }
}

void OCM240128::Display_string_8x16(byte x,byte y,byte *text,byte num,byte mode)
{
	while(*text!=0)//数据未结束
	{  						     
		Display_char_8x16(x,y,text,mode);
		text+=1;
		x+=1;		
	}		
}
/************************************************************************************************
@f_name: void Display_char_8x16(byte hh,ushort page,ushort column,byte text)
@brief:	 显示字符
@param:	 None
@return: None
************************************************************************************************/
void OCM240128::Display_str_16x16(byte x,byte y,byte *text,byte mode)
{
	ushort add;
	byte i,j,k;
	byte qh,ql;
	ushort add1;
	
	qh=*text;
	ql=*(text+1);
	if(qh<58)
	add1=qh-48;
	else	
	add1=((qh-0xb0)*94+(ql-0x96));	
	add = y * 0x20 + x + 0x800;
	i=add;
	j=add>>8;
//	if(qh>0xb0)
//	{
//				for (k = 0; k < 32; k=k+2)
//		{
//			this->wcmd2(i, j, 0x24);
//			if(mode)
//			{this->wcmd(GB3212[11][k], 0xc0);
//			this->wcmd(GB3212[11][k+1], 0xc0);}
//			else
//			{this->wcmd(~GB3212[11][k], 0xc0);
//			this->wcmd(~GB3212[11][k+1], 0xc0);}			
//			add = add + 0x20;
//			i=add;
//			j=add>>8;
//		}
//	}
//	else
	{
		for (k = 0; k < 32; k=k+2)
		{
			this->wcmd2(i, j, 0x24);
			if(mode)
			{this->wcmd(GB3212[add1][k], 0xc0);
			this->wcmd(GB3212[add1][k+1], 0xc0);}
			else
			{this->wcmd(~GB3212[add1][k], 0xc0);
			this->wcmd(~GB3212[add1][k+1], 0xc0);}			
			add = add + 0x20;
			i=add;
			j=add>>8;
		}
	}
}

void OCM240128::Display_string_16x16(byte x,byte y,byte *text,byte num,byte mode)
{
    while(*text!=0)//数据未结束
    {  						     
			Display_str_16x16(x,y,text,mode);
			text+=2;
      x+=2;		
    }						    
}

void OCM240128::Display_shuzi_16x16(byte x,byte y,byte text)
{
	ushort add;
	byte i,j,k;
	add = y * 0x20 + x + 0x800;
	i=add;
	j=add>>8;
	for (k = 0; k < 32; k=k+2)
  {
    this->wcmd2(i, j, 0x24);
    this->wcmd(shuzi_16x16[text][k], 0xc0);
		this->wcmd(shuzi_16x16[text][k+1], 0xc0);
    add = add + 0x20;
		i=add;
		j=add>>8;
  }
}


//画横线
void OCM240128::Draw_hline(byte x,byte y,ushort count)
{
	ushort add;
	byte i,j;
	add = y * 0x20 + x + 0x800;
	i=add;
	j=add>>8;
  this->wcmd2(i, j, 0x24);
  for (i = 0; i < count; i++)
  {
    this->wcmd(0xff, 0xc0);
  }
}

//画竖线
void OCM240128::Draw_vline(byte x, byte y,ushort count)
{
	ushort add;
	byte i,j,k;
	add = y * 0x20 + x + 0x800;
	i=add;
	j=add>>8;
  for (k = 0; k < count; k++)
  {
    this->wcmd2(i, j, 0x24);
    this->wcmd(0x01, 0xc0);
    add = add + 0x20;
		i=add;
		j=add>>8;
  }
}
/************************************************************************************************
@f_name: void LCD12864_Init(void)
@brief:	 液晶初始化
@param:	 None
@return: None
************************************************************************************************/
void OCM240128::Init()
{	
	this->pinfs = 0;
  this->pince = 0;
	this->pinwr = 1;
	this->pinrd = 1;
	
  this->wcmd2(0x00, 0x00, 0x40);
  this->wcmd2(0x20, 0x00, 0x41);
  this->wcmd2(0x00, 0x08, 0x42);
  this->wcmd2(0x20, 0x00, 0x43);
  this->wcmd(0xa1); //光标形状
  this->wcmd(0x80);
  this->wcmd(0x98);
  this->pince = 1;
  /*this->pinled = 1;*/
}

void OCM240128::Clr()
{
  ushort i;
  this->wcmd2(0x00, 0x00, 0x24);
  this->wcmd(0xb0);
  for (i = 0; i < 8192; i++)
  {
    this->wdata(0);
  }
  this->wcmd(0xb2);
  this->wcmd(0x98); //禁止光标闪动
}

void OCM240128::Clrchar(byte x, byte y,ushort count)
{
	byte i;
	for(i=0;i<count;i++)
	{  						     
		Display_char_8x16(x,y,0,1);
		x+=1;		
	}		
}
