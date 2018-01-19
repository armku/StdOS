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
#include "stm32f10x.h"	
//管脚定义，移植修改区
//**************************************************************************************************************************
#define  LCD_DATA_GPIO    GPIOE
#define  GPIOCLK_DATA     RCC_APB2Periph_GPIOE
#define  LCD_CMD_GPIO     GPIOA
#define  GPIOCLK_CMD      RCC_APB2Periph_GPIOA

#define  STA0      GPIO_Pin_8
#define  STA1      GPIO_Pin_9
#define  STA3      GPIO_Pin_11

#define  LCD_GPIO_DAT   GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15
#define  LCD_GPIO_CMD	  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7

#define  LCD_WriteData(x)   {LCD_DATA_GPIO->BSRR = x<<8 & 0xff00;LCD_DATA_GPIO->BRR = ((~x)<<8) & 0xff00;}  //高8位的数据

/************************************************************************************************
@f_name: void LCD12864_DataPort_Out(void)
@brief:	 将数据总线定义为输出
@param:	 None
@return: None
************************************************************************************************/
void OCM240128::LCD_DataPort_Out()
{
	GPIO_InitTypeDef  GPIO_InitStructure;	//定义结构体			
	RCC_APB2PeriphClockCmd(GPIOCLK_DATA, ENABLE);  //

	GPIO_InitStructure.GPIO_Pin  = LCD_GPIO_DAT;		//数据口配置成推挽输出模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(LCD_DATA_GPIO , &GPIO_InitStructure);    //IO口初始化函数（使能上述配置）	  
}
/************************************************************************************************
@f_name: void LCD12864_DataPort_In(void)
@brief:	 将数据总线定义为输入
@param:	 None
@return: None
************************************************************************************************/
void OCM240128::LCD_DataPort_In()
{
	GPIO_InitTypeDef  GPIO_InitStructure;	//定义结构体			
	RCC_APB2PeriphClockCmd(GPIOCLK_DATA, ENABLE);  //

	GPIO_InitStructure.GPIO_Pin  = LCD_GPIO_DAT;		//数据口配置成浮空输入模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入模式 
	GPIO_Init(LCD_DATA_GPIO , &GPIO_InitStructure);    //IO口初始化函数（使能上述配置）	  
}
/************************************************************************************************
@f_name: void LCD12864_CMDPort_Out(void)
@brief:	 将命令总线定义为输出
@param:	 None
@return: None
************************************************************************************************/
void OCM240128::LCD_CMDPort_Out()
{
	GPIO_InitTypeDef  GPIO_InitStructure;	//定义结构体			
	RCC_APB2PeriphClockCmd(GPIOCLK_CMD, ENABLE);  //

	GPIO_InitStructure.GPIO_Pin  = LCD_GPIO_CMD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_CMD_GPIO , &GPIO_InitStructure);	  //初始化IO口配置  
}
/************************************************************************************************
@f_name: void NOP(void)
@brief:	 延时函数
@param:	 None
@return: None
************************************************************************************************/
void OCM240128::NOP()
{ byte i;	for(i=0; i<100; i++); } 
/************************************************************************************************
@f_name: byte LCD12864_busy(void)
@brief:	 检测忙状态
@param:	 None
@return: None
************************************************************************************************/
void OCM240128::LCD_busy_check(byte autowr)
{
	LCD_DataPort_In();
	this->pincd = 1;
	this->pinwr = 1;
	this->pinrd = 0;
	if (autowr)
	{
		while (this->pinsta3==0)
		{
			
		}
	}
	else
	{
		while((this->pinsta0==0)||(this->pinsta1==0))
		{
			Sys.Delay(10);
		};
	}
	this->pinrd = 1;
	LCD_DataPort_Out();
}
/************************************************************************************************
@f_name: void LCD12684_Wcmd_L(byte dat)
@brief:	 写命令
@param:	 byte dat  输入指令
@return: None
************************************************************************************************/
void OCM240128::LCD_Wcmd (byte cmd)
{
	LCD_busy_check(0);
	this->pincd = 1;
	this->pinrd = 1;
	LCD_WriteData(cmd);
	this->pinwr = 0;
	this->pinwr = 1;
}

/************************************************************************************************
@f_name: void LCD12684_Wcmd_R(byte dat)
@brief:	 写数据
@param:	 byte dat  输入指令
@return: None
************************************************************************************************/
void OCM240128::LCD_Wdata(byte dat)
{
	LCD_busy_check(0);
	this->pincd = 0;
	this->pinrd = 1;
	LCD_WriteData(dat);
	this->pinwr = 0;
	this->pinwr = 1;
}
/************************************************************************************************
@f_name: void LCD12684_Wdat_L(byte dat)
@brief:	 写一个数据和一个命令
@param:	 byte dat 输入数据
@return: None
************************************************************************************************/
void OCM240128::LCD_Wdata_cmd (byte dat,byte cmd)
{
	LCD_Wdata(dat);
	LCD_Wcmd(cmd);
}

/************************************************************************************************
@f_name: void LCD12684_Wdat_R(byte dat)
@brief:	 写两个数据一个命令
@param:	 byte dat 输入数据
@return: None
************************************************************************************************/
void OCM240128::LCD_Wdata2_cmd (byte dat1,byte dat2,byte cmd)
{
	LCD_Wdata(dat1);
	LCD_Wdata(dat2);
	LCD_Wcmd(cmd);
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
    LCD_Wdata2_cmd(i, j, 0x24);
		if(mode)
    LCD_Wdata_cmd(ascii_table_8x16[*text-32][k], 0xc0);
		else
		LCD_Wdata_cmd(~ascii_table_8x16[*text-32][k], 0xc0);	
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
//			LCD_Wdata2_cmd(i, j, 0x24);
//			if(mode)
//			{LCD_Wdata_cmd(GB3212[11][k], 0xc0);
//			LCD_Wdata_cmd(GB3212[11][k+1], 0xc0);}
//			else
//			{LCD_Wdata_cmd(~GB3212[11][k], 0xc0);
//			LCD_Wdata_cmd(~GB3212[11][k+1], 0xc0);}			
//			add = add + 0x20;
//			i=add;
//			j=add>>8;
//		}
//	}
//	else
	{
		for (k = 0; k < 32; k=k+2)
		{
			LCD_Wdata2_cmd(i, j, 0x24);
			if(mode)
			{LCD_Wdata_cmd(GB3212[add1][k], 0xc0);
			LCD_Wdata_cmd(GB3212[add1][k+1], 0xc0);}
			else
			{LCD_Wdata_cmd(~GB3212[add1][k], 0xc0);
			LCD_Wdata_cmd(~GB3212[add1][k+1], 0xc0);}			
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
    LCD_Wdata2_cmd(i, j, 0x24);
    LCD_Wdata_cmd(shuzi_16x16[text][k], 0xc0);
		LCD_Wdata_cmd(shuzi_16x16[text][k+1], 0xc0);
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
  LCD_Wdata2_cmd(i, j, 0x24);
  for (i = 0; i < count; i++)
  {
    LCD_Wdata_cmd(0xff, 0xc0);
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
    LCD_Wdata2_cmd(i, j, 0x24);
    LCD_Wdata_cmd(0x01, 0xc0);
    add = add + 0x20;
		i=add;
		j=add>>8;
  }
}

void OCM240128::LCD_TEST()
{
	Display_string_8x16(0,0,(byte*)"1234567890ABCDEFabcd",1,1);
	Display_string_8x16(0,16,(byte*)"1234567890ABCDEFabcd",1,0);		

	Display_shuzi_16x16(0,32,0);	
	Display_shuzi_16x16(2,32,1);
	Display_shuzi_16x16(4,32,2);
	Display_shuzi_16x16(6,32,3);
	Display_shuzi_16x16(8,32,4);
	Display_shuzi_16x16(10,32,5);
	Display_shuzi_16x16(12,32,6);
	Display_shuzi_16x16(14,32,7);
	Display_shuzi_16x16(16,32,8);
	Display_shuzi_16x16(18,32,9);
	Display_shuzi_16x16(20,32,10);

	Display_str_16x16(0,48,(byte*)"啊",0);
	Display_str_16x16(2,48,(byte*)"阿",1);
	Display_string_16x16(0,64,(byte*)"啊阿埃",3,1);
	Display_string_16x16(1,80,(byte*)"啊阿埃",3,0);
	
	Draw_hline(1, 112,10);
	Draw_vline(26,16,30);
}
/************************************************************************************************
@f_name: void LCD12864_Init(void)
@brief:	 液晶初始化
@param:	 None
@return: None
************************************************************************************************/
void OCM240128::LCD_Init()
{	
	this->pinfs = 0;
  this->pince = 0;
	this->pinwr = 1;
	this->pinrd = 1;
	
  LCD_Wdata2_cmd(0x00, 0x00, 0x40);
  LCD_Wdata2_cmd(0x20, 0x00, 0x41);
  LCD_Wdata2_cmd(0x00, 0x08, 0x42);
  LCD_Wdata2_cmd(0x20, 0x00, 0x43);
  LCD_Wcmd(0xa1); //光标形状
  LCD_Wcmd(0x80);
  LCD_Wcmd(0x98);
  this->pince = 1;
  /*this->pinled = 1;*/
}

void OCM240128::LCD_Clr()
{
  ushort i;
  LCD_Wdata2_cmd(0x00, 0x00, 0x24);
  LCD_Wcmd(0xb0);
  for (i = 0; i < 8192; i++)
  {
    LCD_Wdata(0);
  }
  LCD_Wcmd(0xb2);
  LCD_Wcmd(0x98); //禁止光标闪动
}

void OCM240128::LCD_Clr_char(byte x, byte y,ushort count)
{
	byte i;
	for(i=0;i<count;i++)
	{  						     
		Display_char_8x16(x,y,0,1);
		x+=1;		
	}		
}
