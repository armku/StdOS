#include "DS1302.h"
/*********************************************************************/
/* ʵʱʱ��ģ�� ʱ��оƬ�ͺţ�DS1302 */
/*********************************************************************/
//------------------------ʱ�Ӷ��峣��----------------------------------
#define R_Time_Addr 	    (0x81)//��-ʱ��ָ��
#define W_Time_Addr		(0x80)//д-ʱ��ָ��
#define R_CONTROL_Addr	(0x8f)//��-д����ָ��
#define W_CONTROL_Addr	(0x8e)//д-д����ָ��
#define R_TRICKLE_Addr	(0x91)//��-���ٳ��ָ��
#define W_TRICKLE_Addr	(0x90)//д-���ٳ��ָ��
#define R_CLOCK_Addr	    (0xbf)//ʱ��
#define W_CLOCK_Addr	    (0xbe)//ʱ��
#define R_RAM_Addr		(0xc1)//��-�洢��ָ��
#define W_RAM_Addr		(0xc0)//д-�洢��ָ��

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

//����ʱ��
void DS1302::SetTime(uint seconds)
{

}
//����ʱ��-����ʱ���ʽ
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
//��ȡʱ��	
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
* ����: Bytein
* ˵��:
* ����: ��DS1302д��1Byte����
* ����:
* ����: ucDa д�������
* ����ֵ: ��
***********************************************************************/
void DS1302::Bytein(byte ucDa)
{
	byte   i;

	for (i = 0; i < 8; i++)
	{
		if (ucDa & 0x01)
		{
			this->pio = 1; /*�൱�ڻ���е� RRC */
		}
		else
		{
			this->pio = 0; /*�൱�ڻ���е� RRC */
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
* ����: Byteout
* ˵��:
* ����: ��DS1302����1Byte����
* ����:
* ����: ��
* ����ֵ:ucDa д�������
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
* ����: DS_W1302
* ˵��:
* ����: ��DS1302����1Byte����
* ����:
* ����: ucAddr:��ַ���� ��ucDa:д�������
* ����ֵ: ��
***********************************************************************/
void DS1302::DS_W1302(byte ucAddr, byte ucDa)
{
	set1302();
	Bytein(ucAddr); /* ��ַ������ */
	Bytein(ucDa); /* д1Byte����*/
	reset1302();
}
/********************************************************************
*
* ����: DS_R1302
* ˵��:
* ����: ��DS1302����1Byte����
* ����:
* ����: ucAddr:��ַ���� ��ucDa:д�������
* ����ֵ: ��
***********************************************************************/
byte DS1302::DS_R1302(byte ucAddr)
{
	byte   ucDa;

	set1302();

	Bytein(ucAddr); /* ��ַ������ */
	ucDa = Byteout(); /* ��1Byte���� */

	reset1302();
	return (ucDa);
}
/********************************************************************
*
* ����: DS_Set1302
* ˵��:
* ����: ���ó�ʼʱ��
* ����: v_W1302()
* ����: pSecDa: ��ʼʱ���ַ����ʼʱ���ʽΪ: �� �� ʱ �� �� ���� ��
* 7Byte (BCD��) 1B 1B 1B 1B 1B 1B 1B
* ����ֵ: ��
***********************************************************************/
void DS1302::DS_Set1302(byte *pSecDa)
{
	byte   i;
	byte   ucAddr = W_Time_Addr;

	//��д����
	DS_W1302(W_CONTROL_Addr, 0x00);
	for (i = 0; i < 7; i++)
	{
		DS_W1302(ucAddr, *pSecDa); /* �� �� ʱ �� �� ���� �� */
		pSecDa++;
		ucAddr += 2;
	}
	//д����
	DS_W1302(W_CONTROL_Addr, 0x80);
}
/********************************************************************
*
* ����: v_Get1302
* ˵��:
* ����: ��ȡDS1302��ǰʱ��
* ����: uc_R1302()
* ����: ucCurtime: ���浱ǰʱ���ַ����ǰʱ���ʽΪ: �� �� ʱ �� �� ���� ��
* 7Byte (BCD��) 1B 1B 1B 1B 1B 1B 1B
* ����ֵ: ��
***********************************************************************/
void DS1302::DS_Get1302(byte *ucCurtime)
{
	byte   i;
	byte   ucAddr = R_Time_Addr;

	for (i = 0; i < 7; i++)
	{
		ucCurtime[i] = DS_R1302(ucAddr); /*��ʽΪ: �� �� ʱ �� �� ����
										 �� */
		ucAddr += 2;
	}
}
/********************************************************************
*
* ����: DS_W_Ram
* ˵��:
* ����: ��洢��д����
* ����: DS_W1302()
* ����: ucCurtime: ���浱ǰʱ���ַ����ǰʱ���ʽΪ: �� �� ʱ �� �� ���� ��
* 7Byte (BCD��) 1B 1B 1B 1B 1B 1B 1B
* ����ֵ: ��
***********************************************************************/
void DS1302::dsWriteByte(byte Addr, byte ucDa)
{
	Addr <<= 1;
	DS_W1302(W_CONTROL_Addr, 0x00); //��д����
	DS_W1302(W_RAM_Addr | Addr, ucDa);
	DS_W1302(W_CONTROL_Addr, 0x80); //д����
}
/********************************************************************
*
* ����: DS_R_Ram
* ˵��:
* ����: �Ӵ洢��������
* ����: DS_R1302()
* ����: ucCurtime: ���浱ǰʱ���ַ����ǰʱ���ʽΪ: �� �� ʱ �� �� ���� ��
* 7Byte (BCD��) 1B 1B 1B 1B 1B 1B 1B
* ����ֵ: ��
***********************************************************************/
byte DS1302::dsReadByte(byte Addr)
{
	Addr <<= 1;
	return DS_R1302(R_RAM_Addr | Addr);
}
//#endif 
//��洢��д����
#if 0
void dsWriteWord(byte Addr, INT16U ucDa)
{
	dsWriteByte(Addr, ucDa & 0xff);
	dsWriteByte(Addr + 1, ucDa >> 8);
}

//�Ӵ洢��������
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
