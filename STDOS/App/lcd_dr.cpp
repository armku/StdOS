#include <stdio.h>
#include <string.h>
#include <math.h>

#include "lcd_dr.h"
#include "Bsp\font.h"

CLcd_DR::CLcd_DR()
{}
/*********************************************************************************************************
�� �� ��: DisplayChar4x8
����˵��: ��ʾascall��
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: ch 
��    ��4: color ��ɫ
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd_DR::DisplayChar4x8(uint16_t x, uint16_t y, uint8_t ch)
{       
	this->Display4x8(x,y,(uint8_t*)&ascii_table_4x8[ch - 0x20][0]);
}
/*********************************************************************************************************
�� �� ��: DisplayString4x8
����˵��: ��ʾ�ַ���
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: *text ��ʾ����
��    ��4: color ��ɫ
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd_DR::DisplayString4x8(uint16_t x, uint16_t y, char *text)
{
	while(*text)
	{
		this->DisplayChar4x8(x,y,*text);
		text++;
		x+=5;
	}
}
/*********************************************************************************************************
�� �� ��: DisplayChar5x8
����˵��: ��ʾascall��
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: ch 
��    ��4: color ��ɫ
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd_DR::DisplayChar5x8(uint16_t x, uint16_t y, uint8_t ch)
{    
    this->Display5x8(x,y,(uint8_t*)&ascii_table_5x8[ch - 0x20][0]);	
}
/*********************************************************************************************************
�� �� ��: DisplayString5x8
����˵��: ��ʾ�ַ���
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: *text ��ʾ����
��    ��4: color ��ɫ
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd_DR::DisplayString5x8(uint16_t x, uint16_t y, char *text)
{
	while(*text)
	{
		this->DisplayChar5x8(x,y,*text);
		text++;
		x+=6;
	}
}

/*********************************************************************************************************
�� �� ��: DisplayChar6x12
����˵��: ��ʾascall��
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: ch ��ʾ����
��    ��4: color ��ɫ
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd_DR::DisplayChar6x12(uint16_t x, uint16_t y, uint8_t ch)
{    
	this->Display6x12(x,y,(uint8_t*)&ascii_table_6x12[ch - 0x20][0]);
}
/*********************************************************************************************************
�� �� ��: DisplayChar8x16
����˵��: ��ʾascall��
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: ch ��ʾ����
��    ��4: color ��ɫ
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd_DR::DisplayChar8x16(uint16_t x, uint16_t y, uint8_t ch)
{    
	this->Display8x16(x,y,(uint8_t*)&ascii_table_8x16[ch - 0x20][0]);
}

/*********************************************************************************************************
�� �� ��: DisplayChar6x16
����˵��: ��ʾascall��
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: ch ��ʾ����
��    ��4: color ��ɫ
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd_DR::DisplayChar6x16(uint16_t x, uint16_t y, uint8_t ch)
{    
	this->Display6x16(x,y,(uint8_t*)&nAsciiDot6X16[ch - 0x20][0]);
}
/*********************************************************************************************************
�� �� ��: DisplayChar8x16Bolt
����˵��: ��ʾascall��
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: ch ��ʾ����
��    ��4: color ��ɫ
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd_DR::DisplayChar8x16Bolt(uint16_t x, uint16_t y, uint8_t ch)
{    
	this->Display8x16(x,y,(uint8_t*)&ascBolt[ch - 0x20][0]);
}
/*********************************************************************************************************
�� �� ��: DisplayString8x16
����˵��: ��ʾ�ַ���
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: *text ��ʾ����
��    ��4: color ��ɫ
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd_DR::DisplayString8x16(uint16_t x, uint16_t y, char *text)
{
	while(*text)
	{
		this->DisplayChar8x16(x,y,*text);
		text++;
		x+=9;
	}
}

/*********************************************************************************************************
�� �� ��: DisplayLattice
����˵��: ��ʾ����
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: *text ��ʾ����
��    ��4: font ����ѡ�� 8  12  16  20 32
��    ��5: real �Ƿ�ʵ��
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd_DR::DisplayLattice(uint16_t x, uint16_t y, uint8_t *dp, Font font)
{
    switch (font)
    {
        case FONT_4X8:
            this->Display4x8(x, y, dp);
            break;
        case FONT_8X16:
            this->Display8x16(x, y, dp);
            break;
        case FONT_12X12:
            this->Display12x12(x, y, dp);
            break;
        case FONT_16X16:
            this->Display16x16(x, y, dp);
            break;
        case FONT_16X32:
            this->Display16x32(x, y, dp);
            break;
        case FONT_32X32:
            this->Display32x32(x, y, dp);
            break;
        default:
            this->Display32x32(x, y, dp);
            break;
    }
}

void CLcd_DR::DisplayHEX(uint16_t x,uint16_t y,uint8_t da,Font font)
{
	uint8_t hi=da>>4;
	uint8_t lo=da&0x0f;
	
	if(hi>9)
	{
		hi=hi-10+'A';
	}
	else
	{
		hi=hi+'0';
	}
	if(lo>9)
	{
		lo=lo-10+'A';
	}
	else
	{
		lo+='0';
	}
	switch(font)
	{
		case FONT_8X16:
			this->DisplayChar8x16(x,y,hi);
			this->DisplayChar8x16(x+8,y,lo);
			break;
		default:
			break;
	}
}
/*********************************************************************************************************
�� �� ��: DisplayStr
����˵��: �ַ���ʾ
��    �Σ����� ��ʾ���� ����ѡ�� �Ƿ�ʵ��
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd_DR::DisplayStr(uint16_t x, uint16_t y, char *text, Font font)
{
    while (*text)
    {
        if ((*text) < 0x80)
        {
            switch (font)
            {
                case FONT_4X8:                    
                        this->DisplayChar4x8(x, y, *text);
                        x += 5;                    
                    break;
                case FONT_6X12:
					this->DisplayChar6x12(x, y, *text);
					x+=6;
					break;
				case FONT_12X12:
				case FONT_5X8:                    
                        this->DisplayString5x8(x, y, text);
                        x += 6;                    
                    break;
				case FONT_6X16:
					 this->DisplayChar6x16(x, y, *text);
					if(*text=='.')
					{
                        x += 5; 
					}
					else
					{
						x+=7;
					}
					break;
                case FONT_8X16BOLT:
					 this->DisplayChar8x16Bolt(x, y, *text);
					if(*text=='.')
					{
						x+=6;
					}
					else
					{
                        x += 8; 
					}
					break;
                case FONT_16X16:
				case FONT_8X16:                    
                        this->DisplayChar8x16(x, y, *text);
                        x += 8;                    
                    break;
                default:                    
                        this->DisplayChar8x16(x, y, *text);
                        x += 8;                   
                    break;
            }
            text++;
        }
        else if (((*text) &0xf0) && ((*(text + 1)) &0xf0))
        {
            switch (font)
            {
                case FONT_12X12:                    
                        this->Display12x12(x, y, (uint8_t*)GB_12[SearchhzIndex(*text, *(text + 1), FONT_12X12)].Msk);
                        x += 12;                    
                    break;
                case FONT_16X16:                    
                        this->Display16x16(x, y, (uint8_t *)GB_16[SearchhzIndex(*text, *(text + 1), FONT_16X16)].Msk);
                        x += 16;                   
                    break;
                default:                    
                        this->Display16x16(x, y, (uint8_t *)GB_16[SearchhzIndex(*text, *(text + 1), FONT_16X16)].Msk);
                        x += 16;                    
                    break;
            }
            text++;
            text++;
        }
    }
}

//��ʾ��ֵ float����
void CLcd_DR::DispVal(uint16_t x, uint16_t y, float val, int8_t wei, Font font) 
{	
	char dat[20];//����ѡ�� ����sprintfת��
    switch (wei)
    {
        case 0:
            snprintf(dat, 8, "%6.0f", val);
            this->DisplayStr(x, y, dat, font);
            break;
        case 1:
            snprintf(dat, 8, "%6.1f", val);
            this->DisplayStr(x, y, dat, font);
            break;
        case 2:
            snprintf(dat, 8, "%6.2f", val);
            this->DisplayStr(x, y, dat, font);
            break;
        case 3:
            snprintf(dat, 8, "%6.3f", val);
            this->DisplayStr(x, y, dat, font);
            break;
        case 4:
            snprintf(dat, 8, "%.*f", 4, val);
            this->DisplayStr(x, y, dat, font);
            break;
        default:
            break;
    }
}
//��ʾ��ֵ �̶�λ�÷���
void CLcd_DR::DispValu(uint16_t x, uint16_t y, uint16_t val, int wei, uint8_t reversebit, Font font)
{
    char dat[20]; //����ѡ�� ����sprintfת��
    uint8_t pos = 0;
    if (wei > 3)
    {
        dat[pos] = val / 1000 % 10+'0';
        pos++;
    }
    if (wei > 2)
    {
        dat[pos] = val / 100 % 10+'0';
        pos++;
    }
    if (wei > 1)
    {
        dat[pos] = val / 10 % 10+'0';
        pos++;
    }
    if (wei > 0)
    {
        dat[pos] = val % 10+'0';
        pos++;
    }
    switch (wei)
    {
        case 4:
            switch (font)
            {
            case FONT_16X16:
            case FONT_8X16:
                this->DisplayChar8x16(x, y, dat[0]);
                this->DisplayChar8x16(x + 8, y, dat[1]);
                this->DisplayChar8x16(x + 16, y, dat[2]);
                this->DisplayChar8x16(x + 24, y, dat[3]);
                break;
            }
            break;
        default:
            break;
    }
}
/*********************************************************************************************************
�� �� ��: SearchhzIndex
����˵��:�����ѯ
��    �Σ���
�� �� ֵ: ����ֵ
 ********************************************************************************************************/
uint16_t CLcd_DR::SearchhzIndex(uint8_t lo, uint8_t hi, Font font) //���Һ��ֱ���λ��
{
    uint16_t i = 0;
    uint16_t maxcount = 251;
    switch (font)
    {
        case FONT_12X12:
            maxcount = 251;
            for (i = 0; i < maxcount; i++)
            {
                if ((GB_12[i].Index[0] == lo) && (GB_12[i].Index[1] == hi))
                {
                    return i;
                }
            }
            break;
        case FONT_16X16:
            maxcount = 251;
            for (i = 0; i < maxcount; i++)
            {
                if ((GB_16[i].Index[0] == lo) && (GB_16[i].Index[1] == hi))
                {
                    return i;
                }
            }
            break;
        default:
            maxcount = 251;
            for (i = 0; i < maxcount; i++)
            {
                if ((GB_16[i].Index[0] == lo) && (GB_16[i].Index[1] == hi))
                {
                    return i;
                }
            }
            break;
    }
    return 0;
}
