#include "SSD1309.h"
#include "oledfont.h"  

//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  

void SSD1309::SetPin(Pin cs,Pin res, Pin dc,Pin wr,Pin rd,Pin sclk,Pin sdin)
{
	this->_cs.Set(cs);
	this->_res.Set(res);
	this->_dc.Set(dc);
	this->_wr.Set(wr);
	this->_rd.Set(rd);
	this->_sclk.Set(sclk);
	this->_sdin.Set(sdin);
	
	this->_cs.OpenDrain=false;
	this->_res.OpenDrain=false;
	this->_dc.OpenDrain=false;
	this->_wr.OpenDrain=false;
	this->_rd.OpenDrain=false;
	this->_sclk.OpenDrain=false;
	this->_sdin.OpenDrain=false;
	
	this->_cs.Invert=0;
	this->_res.Invert=0;
	this->_dc.Invert=0;
	this->_wr.Invert=0;
	this->_rd.Invert=0;
	this->_sclk.Invert=0;
	this->_sdin.Invert=0;
	
	this->_cs.Open();
	this->_res.Open();
	this->_dc.Open();
	this->_wr.Open();
	this->_rd.Open();
	this->_sclk.Open();
	this->_sdin.Open();
}
void  SSD1309::OLED_CS_Clr()  
{
	_cs=0;
}
void  SSD1309::OLED_CS_Set()  
{
	_cs=1;
}

void  SSD1309::OLED_RST_Clr() 
{
	_res=0;
}
void  SSD1309::OLED_RST_Set() 
{
	_res=1;
}

void  SSD1309::OLED_DC_Clr() 
{
	_dc=0;
}
void  SSD1309::OLED_DC_Set() 
{
	_dc=1;
}

void  SSD1309::OLED_WR_Clr() 
{
	_wr=0;
}
void  SSD1309::OLED_WR_Set() 
{
	_wr=1;
}

void  SSD1309::OLED_RD_Clr() 
{
	_rd=0;
}
void  SSD1309::OLED_RD_Set() 
{
	_rd=1;
}

void SSD1309::OLED_SCLK_Clr()
{
	_sclk=0;
}
void SSD1309::OLED_SCLK_Set()
{
	_sclk=1;
}
void  SSD1309::OLED_SDIN_Clr()
{
	_sdin=0;
}
void SSD1309::OLED_SDIN_Set()
{	
	_sdin=1;
}


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   

#if OLED_MODE==1
    //向SSD1106写入一个字节。
    //dat:要写入的数据/命令
    //cmd:数据/命令标志 0,表示命令;1,表示数据;
    void SSD1309::OLED_WR_Byte(byte dat, byte cmd)
    {
        DATAOUT(dat);
        if (cmd)
            OLED_DC_Set();
        else
            OLED_DC_Clr();
        OLED_CS_Clr();
        OLED_WR_Clr();
        OLED_WR_Set();
        OLED_CS_Set();
        OLED_DC_Set();
    }
#else 
    //向SSD1106写入一个字节。
    //dat:要写入的数据/命令
    //cmd:数据/命令标志 0,表示命令;1,表示数据;
    void SSD1309::OLED_WR_Byte(byte dat, byte cmd)
    {
        byte i;
        if (cmd)
            OLED_DC_Set();
        else
            OLED_DC_Clr();
        OLED_CS_Clr();
        for (i = 0; i < 8; i++)
        {
            OLED_SCLK_Clr();
            if (dat &0x80)
                OLED_SDIN_Set();
            else
                OLED_SDIN_Clr();
            OLED_SCLK_Set();
            dat <<= 1;
        }
        OLED_CS_Set();
        OLED_DC_Set();
    }
#endif 
void SSD1309::OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WR_Byte(0xb0 + y, OLED_CMD);
    OLED_WR_Byte(((x &0xf0) >> 4) | 0x10, OLED_CMD);
    OLED_WR_Byte((x &0x0f) | 0x01, OLED_CMD);
}

//开启OLED显示    
void SSD1309::OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC命令
    OLED_WR_Byte(0X14, OLED_CMD); //DCDC ON
    OLED_WR_Byte(0XAF, OLED_CMD); //DISPLAY ON
}

//关闭OLED显示     
void SSD1309::OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC命令
    OLED_WR_Byte(0X10, OLED_CMD); //DCDC OFF
    OLED_WR_Byte(0XAE, OLED_CMD); //DISPLAY OFF
}

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void SSD1309::OLED_Clear(void)
{
    byte i, n;
    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); //设置页地址（0~7）
        OLED_WR_Byte(0x00, OLED_CMD); //设置显示位置—列低地址
        OLED_WR_Byte(0x10, OLED_CMD); //设置显示位置—列高地址   
        for (n = 0; n < 128; n++)
            OLED_WR_Byte(0, OLED_DATA);
    } //更新显示
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void SSD1309::OLED_ShowChar(byte x, byte y, byte chr)
{
    unsigned char c = 0, i = 0;
    c = chr - ' '; //得到偏移后的值			
    if (x > Max_Column - 1)
    {
        x = 0;
        y = y + 2;
    }
    if (SIZE == 16)
    {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 8; i++)
            OLED_WR_Byte(F8X16[c *16+i], OLED_DATA);
        OLED_Set_Pos(x, y + 1);
        for (i = 0; i < 8; i++)
            OLED_WR_Byte(F8X16[c *16+i + 8], OLED_DATA);
    }
    else
    {
        OLED_Set_Pos(x, y + 1);
        for (i = 0; i < 6; i++)
            OLED_WR_Byte(F6x8[c][i], OLED_DATA);

    }
}

//m^n函数
uint SSD1309::oled_pow(byte m, byte n)
{
    uint result = 1;
    while (n--)
        result *= m;
    return result;
}

//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void SSD1309::OLED_ShowNum(byte x, byte y, uint num, byte len, byte size)
{
    byte t, temp;
    byte enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                OLED_ShowChar(x + (size / 2) *t, y, ' ');
                continue;
            }
            else
                enshow = 1;

        }
        OLED_ShowChar(x + (size / 2) *t, y, temp + '0');
    }
}

//显示一个字符号串
void SSD1309::OLED_ShowString(byte x, byte y, byte *chr)
{
    unsigned char j = 0;
    while (chr[j] != '\0')
    {
        OLED_ShowChar(x, y, chr[j]);
        x += 8;
        if (x > 120)
        {
            x = 0;
            y += 2;
        }
        j++;
    }
}

//显示汉字
void SSD1309::OLED_ShowCHinese(byte x, byte y, byte no)
{
    byte t, adder = 0;
    OLED_Set_Pos(x, y);
    for (t = 0; t < 16; t++)
    {
        OLED_WR_Byte(Hzk[2 *no][t], OLED_DATA);
        adder += 1;
    }
    OLED_Set_Pos(x, y + 1);
    for (t = 0; t < 16; t++)
    {
        OLED_WR_Byte(Hzk[2 *no + 1][t], OLED_DATA);
        adder += 1;
    }
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void SSD1309::OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
    unsigned int j = 0;
    unsigned char x, y;

    if (y1 % 8 == 0)
        y = y1 / 8;
    else
        y = y1 / 8+1;
    for (y = y0; y < y1; y++)
    {
        OLED_Set_Pos(x0, y);
        for (x = x0; x < x1; x++)
        {
            OLED_WR_Byte(BMP[j++], OLED_DATA);
        }
    }
}
//初始化SSD1306					    
void SSD1309::OLED_Init(void)
{	
    OLED_RST_Set();
	Sys.Sleep(100);
    //delay_ms(100);
    OLED_RST_Clr();
    Sys.Sleep(100);
    //delay_ms(100);
    OLED_RST_Set();
    OLED_WR_Byte(0xFD, OLED_CMD); //--turn off oled panel
    OLED_WR_Byte(0x12, OLED_CMD); //--turn off oled panel	
    OLED_WR_Byte(0xAE, OLED_CMD); //--turn off oled panel
    OLED_WR_Byte(0xD5, OLED_CMD); //---set low column address
    OLED_WR_Byte(0xA0, OLED_CMD); //---set high column address
    OLED_WR_Byte(0xA8, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x3F, OLED_CMD); //--set contrast control register
    OLED_WR_Byte(0xD3, OLED_CMD); // Set SEG Output Current Brightness
    OLED_WR_Byte(0x00, OLED_CMD); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WR_Byte(0x40, OLED_CMD); //Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WR_Byte(0xA1, OLED_CMD); //--set normal display
    OLED_WR_Byte(0xC8, OLED_CMD); //--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0xDA, OLED_CMD); //--1/64 duty
    OLED_WR_Byte(0x2, OLED_CMD); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00, OLED_CMD); //-not offset
    OLED_WR_Byte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9, OLED_CMD); //--set pre-charge period
    OLED_WR_Byte(0xF1, OLED_CMD); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA, OLED_CMD); //--set com pins hardware configuration
    OLED_WR_Byte(0x12, OLED_CMD);
    OLED_WR_Byte(0x81, OLED_CMD); //--set vcomh
    OLED_WR_Byte(0xBF, OLED_CMD); //Set VCOM Deselect Level
    OLED_WR_Byte(0xD9, OLED_CMD); //-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x25, OLED_CMD); //
    OLED_WR_Byte(0xDB, OLED_CMD); //--set Charge Pump enable/disable
    OLED_WR_Byte(0x34, OLED_CMD); //--set(0x10) disable
    OLED_WR_Byte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7) 
    OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel

    OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/
    OLED_Clear();
    OLED_Set_Pos(0, 0);
}

