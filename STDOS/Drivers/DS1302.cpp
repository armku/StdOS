#include "DS1302.h"
/*********************************************************************/
/* 实时时钟模块 时钟芯片型号：DS1302 */
/*********************************************************************/
//------------------------时钟定义常量----------------------------------
#define R_Time_Addr 	    (0x81)//读-时间指令
#define W_Time_Addr		(0x80)//写-时间指令
#define R_CONTROL_Addr	(0x8f)//读-写保护指令
#define W_CONTROL_Addr	(0x8e)//写-写保护指令
#define R_TRICKLE_Addr	(0x91)//读-慢速充电指令
#define W_TRICKLE_Addr	(0x90)//写-慢速充电指令
#define R_CLOCK_Addr	    (0xbf)//时钟
#define W_CLOCK_Addr	    (0xbe)//时钟
#define R_RAM_Addr		(0xc1)//读-存储器指令
#define W_RAM_Addr		(0xc0)//写-存储器指令

byte buffer[20];
DS1302::DS1302()
{
}

DS1302::~DS1302()
{
}
void DS1302::SetPin(Pin pinsck, Pin pinio, Pin pinrst)
{
	this->psck.Set(pinsck);
	this->pio.Set(pinio);
	this->prst.Set(pinrst);

	this->psck.Invert = false;
	this->pio.Invert = false;
	this->prst.Invert = false;

	this->psck.OpenDrain = true;
	this->pio.OpenDrain = true;
	this->prst.OpenDrain = true;

	this->psck.Open();
	this->pio.Open();
	this->prst.Open();
}
void DS1302::Init()
{
	this->psck = 0;
	this->prst = 0;

}

//设置时间
void DS1302::SetTime(uint seconds)
{

}
//设置时间-北京时间格式
void DS1302::SetTime(DateTime & dt)
{	
	if (dt.Second > 59)
	{
		dt.Second = 59;
	}
	buffer[0] = ((dt.Second / 10 % 10) << 4) + dt.Second % 10;
	buffer[1] = ((dt.Minute / 10 % 10) << 4) + dt.Minute % 10;
	buffer[2] = ((dt.Hour / 10 % 10) << 4) + dt.Hour % 10;
	buffer[3] = ((dt.Day / 10 % 10) << 4) + dt.Day % 10;
	buffer[4] = ((dt.Month / 10 % 10) << 4) + dt.Month % 10;
	buffer[5] = ((dt.DayOfWeek() / 10 % 10) << 4) + dt.DayOfWeek() % 10;
	buffer[6] = (((dt.Year%100) / 10 % 10) << 4) + (dt.Year % 100) % 10;

	DS_Set1302(buffer);
}
//读取时间	
DateTime& DS1302::GetTime(DateTime & dt)
{	
	DS_Get1302(buffer);

	dt.Second = (buffer[0] >> 4) * 10 + (buffer[0] & 0x0f); //sec 0-59
	dt.Minute = (buffer[1] >> 4) * 10 + (buffer[1] & 0x0f); //min 0-59
	dt.Hour = (buffer[2] >> 4) * 10 + (buffer[2] & 0x0f); //hour 0-23
	dt.Day = (buffer[3] >> 4) * 10 + (buffer[3] & 0x0f); //date-month 1-31
	dt.Month = (buffer[4] >> 4) * 10 + (buffer[4] & 0x0f); //month-year  1-12
	//dt.DayOfWeek = (buffer[5] >> 4) * 10 + (buffer[5] & 0x0f); //day-week	  1-7
	dt.Year = (buffer[6] >> 4) * 10 + (buffer[6] & 0x0f)+2000; //year		  0-99	
	
	return dt;
}

void DS1302::set1302()
{
	byte   i;
	this->prst = 1;
	Sys.Delay(10);
}
void DS1302::reset1302()
{
	byte   i;
	this->prst = 0;
	Sys.Delay(10);
}
/********************************************************************
*
* 名称: Bytein
* 说明:
* 功能: 往DS1302写入1Byte数据
* 调用:
* 输入: ucDa 写入的数据
* 返回值: 无
***********************************************************************/
void DS1302::Bytein(byte ucDa)
{
	byte   i;

	for (i = 0; i < 8; i++)
	{
		if (ucDa & 0x01)
		{
			this->pio = 1; /*相当于汇编中的 RRC */
		}
		else
		{
			this->pio = 0; /*相当于汇编中的 RRC */
		}
		Sys.Delay(1);
		this->psck = 1;
		Sys.Delay(1);
		this->psck = 0;
		Sys.Delay(1);
		ucDa = ucDa >> 1;
	}
}

/********************************************************************
*
* 名称: Byteout
* 说明:
* 功能: 往DS1302读出1Byte数据
* 调用:
* 输入: 无
* 返回值:ucDa 写入的数据
***********************************************************************/
byte DS1302::Byteout()
{
	byte   i, ucDa;
	this->pio = 1;
	for (i = 0; i < 8; i++)
	{
		ucDa >>= 1;
		this->pio = 1;
		if (this->pio)
		{
			ucDa |= 0X80;
		}
		Sys.Delay(1);
		this->psck = 1;
		Sys.Delay(1);
		this->psck = 0;
		Sys.Delay(1);
	}
	return (ucDa);
}
/********************************************************************
*
* 名称: DS_W1302
* 说明:
* 功能: 往DS1302读出1Byte数据
* 调用:
* 输入: ucAddr:地址命令 ；ucDa:写入的数据
* 返回值: 无
***********************************************************************/
void DS1302::DS_W1302(byte ucAddr, byte ucDa)
{
	set1302();
	Bytein(ucAddr); /* 地址，命令 */
	Bytein(ucDa); /* 写1Byte数据*/
	reset1302();
}
/********************************************************************
*
* 名称: DS_R1302
* 说明:
* 功能: 往DS1302读出1Byte数据
* 调用:
* 输入: ucAddr:地址命令 ；ucDa:写入的数据
* 返回值: 无
***********************************************************************/
byte DS1302::DS_R1302(byte ucAddr)
{
	byte   ucDa;

	set1302();

	Bytein(ucAddr); /* 地址，命令 */
	ucDa = Byteout(); /* 读1Byte数据 */

	reset1302();
	return (ucDa);
}
/********************************************************************
*
* 名称: DS_Set1302
* 说明:
* 功能: 设置初始时间
* 调用: v_W1302()
* 输入: pSecDa: 初始时间地址。初始时间格式为: 秒 分 时 日 月 星期 年
* 7Byte (BCD码) 1B 1B 1B 1B 1B 1B 1B
* 返回值: 无
***********************************************************************/
void DS1302::DS_Set1302(byte *pSecDa)
{
	byte   i;
	byte   ucAddr = W_Time_Addr;

	//开写保护
	DS_W1302(W_CONTROL_Addr, 0x00);
	for (i = 0; i < 7; i++)
	{
		DS_W1302(ucAddr, *pSecDa); /* 秒 分 时 日 月 星期 年 */
		pSecDa++;
		ucAddr += 2;
	}
	//写保护
	DS_W1302(W_CONTROL_Addr, 0x80);
}
/********************************************************************
*
* 名称: v_Get1302
* 说明:
* 功能: 读取DS1302当前时间
* 调用: uc_R1302()
* 输入: ucCurtime: 保存当前时间地址。当前时间格式为: 秒 分 时 日 月 星期 年
* 7Byte (BCD码) 1B 1B 1B 1B 1B 1B 1B
* 返回值: 无
***********************************************************************/
void DS1302::DS_Get1302(byte *ucCurtime)
{
	byte   i;
	byte   ucAddr = R_Time_Addr;

	for (i = 0; i < 7; i++)
	{
		ucCurtime[i] = DS_R1302(ucAddr); /*格式为: 秒 分 时 日 月 星期
										 年 */
		ucAddr += 2;
	}
}
/********************************************************************
*
* 名称: DS_W_Ram
* 说明:
* 功能: 向存储器写数据
* 调用: DS_W1302()
* 输入: ucCurtime: 保存当前时间地址。当前时间格式为: 秒 分 时 日 月 星期 年
* 7Byte (BCD码) 1B 1B 1B 1B 1B 1B 1B
* 返回值: 无
***********************************************************************/
void DS1302::dsWriteByte(byte Addr, byte ucDa)
{
	Addr <<= 1;
	DS_W1302(W_CONTROL_Addr, 0x00); //开写保护
	DS_W1302(W_RAM_Addr | Addr, ucDa);
	DS_W1302(W_CONTROL_Addr, 0x80); //写保护
}
/********************************************************************
*
* 名称: DS_R_Ram
* 说明:
* 功能: 从存储器读数据
* 调用: DS_R1302()
* 输入: ucCurtime: 保存当前时间地址。当前时间格式为: 秒 分 时 日 月 星期 年
* 7Byte (BCD码) 1B 1B 1B 1B 1B 1B 1B
* 返回值: 无
***********************************************************************/
byte DS1302::dsReadByte(byte Addr)
{
	Addr <<= 1;
	return DS_R1302(R_RAM_Addr | Addr);
}
//#endif 
//向存储器写数据
#if 0
void dsWriteWord(byte Addr, INT16U ucDa)
{
	dsWriteByte(Addr, ucDa & 0xff);
	dsWriteByte(Addr + 1, ucDa >> 8);
}

//从存储器读数据
INT16U dsReadWord(byte addr)
{
	INT16U   ret;

	ret = dsReadByte(addr + 1);
	ret <<= 8;
	ret |= dsReadByte(addr);

	return ret;
}
#endif 
void DS1302::dsWritePage(byte Addr, byte *da, byte len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		dsWriteByte(Addr + i, *(da + i));
	}
}

void DS1302::dsReadPage(byte Addr, byte *da, byte len)
{
	byte   i;
	for (i = 0; i < len; i++)
	{
		*(da + i) = dsReadByte(Addr + i);
	}
}
