#include "SSD1309.h"
#include "../../../User/ssd1309font.h"

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

void SSD1309::SetPinSpi(Pin sclk,Pin sdin,Pin dc,Pin res,Pin cs)
{
	this->_cs.Set(cs);
    this->_res.Set(res);
    this->_dc.Set(dc);
    this->_sclk.Set(sclk);
    this->_sdin.Set(sdin);

    this->_cs.OpenDrain = false;
    this->_res.OpenDrain = false;
    this->_dc.OpenDrain = false;
    this->_sclk.OpenDrain = false;
    this->_sdin.OpenDrain = false;

    this->_cs.Invert = 0;
    this->_res.Invert = 0;
    this->_dc.Invert = 0;
    this->_sclk.Invert = 0;
    this->_sdin.Invert = 0;

    this->_cs.Open();
    this->_res.Open();
    this->_dc.Open();
    this->_sclk.Open();
    this->_sdin.Open();
}

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
    void SSD1309::this->WRByte(uint8_t dat, uint8_t cmd)
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
    void SSD1309::WRByte(uint8_t dat, uint8_t cmd)
    {
        uint8_t i;
        if (cmd)
            this->_dc = 1;
        else
            this->_dc = 0;
        this->_cs = 0;
        for (i = 0; i < 8; i++)
        {
            this->_sclk = 0;
            if (dat &0x80)
                this->_sdin = 1;
            else
                this->_sdin = 0;
            this->_sclk = 1;
            dat <<= 1;
        }
        this->_cs = 1;
        this->_dc = 1;
    }	
#endif
//写命令
void SSD1309::_wrcmd(uint8_t cmd)
{
	this->WRByte(cmd,0);
}
//写数据
void SSD1309::_wrdata(uint8_t da)
{
	this->WRByte(da,1);
}	
void SSD1309::SetPos(uint8_t x, uint8_t y)
{
    this->_wrcmd(0xb0 + y);
    this->_wrcmd(((x &0xf0) >> 4) | 0x10);
    this->_wrcmd((x &0x0f) | 0x01);
}

//开启OLED显示    
void SSD1309::DisplayOn()
{
    this->_wrcmd(0X8D); //SET DCDC命令
    this->_wrcmd(0X14); //DCDC ON
    this->_wrcmd(0XAF); //DISPLAY ON
}

//关闭OLED显示     
void SSD1309::DisplayOff()
{
    this->_wrcmd(0X8D); //SET DCDC命令
    this->_wrcmd(0X10); //DCDC OFF
    this->_wrcmd(0XAE); //DISPLAY OFF
}

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void SSD1309::Clear(char ch)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        this->_wrcmd(0xb0 + i); //设置页地址（0~7）
        this->_wrcmd(0x00); //设置显示位置—列低地址
        this->_wrcmd(0x10); //设置显示位置—列高地址   
        for (n = 0; n < 128; n++)
            this->_wrdata(ch);
    } //更新显示
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void SSD1309::ShowChar(uint8_t x, uint8_t y, uint8_t chr)
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
        this->SetPos(x, y);
        for (i = 0; i < 8; i++)
            this->_wrdata(F8X16[c *16+i]);
        this->SetPos(x, y + 1);
        for (i = 0; i < 8; i++)
            this->_wrdata(F8X16[c *16+i + 8]);
    }
    else
    {
        this->SetPos(x, y + 1);
        for (i = 0; i < 6; i++)
            this->_wrdata(F6x8[c][i]);

    }
}

//m^n函数
uint32_t SSD1309::oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
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
void SSD1309::ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                this->ShowChar(x + (size / 2) *t, y, ' ');
                continue;
            }
            else
                enshow = 1;

        }
        this->ShowChar(x + (size / 2) *t, y, temp + '0');
    }
}
//显示一个字符号串
void SSD1309::ShowString(uint8_t x, uint8_t y, char *chr, uint8_t interval)
{
    unsigned char j = 0;
    while (chr[j] != '\0')
    {
        if (chr[j] < 0x80)
        {
            this->ShowChar(x, y, chr[j]);
            x += 8;
            if (x > 120)
            {
                x = 0;
                y += 2;
            }
            j++;
        }
        else
        {
			this->ShowCHinese(x,y,chr+j);
			j++;
			j++;
			x+=16+interval;
        }
    }
}
//显示汉字
void SSD1309::ShowCHinese11(uint8_t x, uint8_t y, uint8_t no)
{
	uint8_t t, adder = 0;
    this->SetPos(x, y);
    for (t = 0; t < 16; t++)
    {
        this->_wrdata(gb16ssd1309[no].Msk[t]);
        adder += 1;
    }
    this->SetPos(x, y + 1);
    for (t = 0; t < 16; t++)
    {
        this->_wrdata(gb16ssd1309[no].Msk[16+t]);
        adder += 1;
    }
}
void SSD1309::ShowCHinese(uint8_t x, uint8_t y, char *hz)
{
	uint8_t lo=hz[0];
	uint8_t hi=hz[1];
	this->ShowCHinese11(x,y,this->SearchhzIndex(lo,hi));
}
//查找汉字编码位置
uint16_t SSD1309::SearchhzIndex(uint8_t lo, uint8_t hi)
{
	uint16_t i = 0;
    uint16_t maxcount = 251;
	for (i = 0; i < maxcount; i++)
	{
		if ((gb16ssd1309[i].Index[0] == lo) && (gb16ssd1309[i].Index[1] == hi))
		{
			return i;
		}
	}
    return 0;
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void SSD1309::DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[])
{
    unsigned int j = 0;
    unsigned char x, y;

    if (y1 % 8 == 0)
        y = y1 / 8;
    else
        y = y1 / 8+1;
    for (y = y0; y < y1; y++)
    {
        this->SetPos(x0, y);
        for (x = x0; x < x1; x++)
        {
            this->_wrdata(BMP[j++]);
        }
    }
}

//初始化SSD1306					    
void SSD1309::Init()
{
    this->_res = 1;
    delay_ms(100);
    this->_res = 0;
    delay_ms(100);
    this->_res = 1;
	this->_wrcmd(0xFD); //--turn off oled panel
    this->_wrcmd(0x12); //--turn off oled panel	
    this->_wrcmd(0xAE); //--turn off oled panel
    this->_wrcmd(0xD5); //---set low column address
    this->_wrcmd(0xA0); //---set high column address
    this->_wrcmd(0xA8); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    this->_wrcmd(0x3F); //--set contrast control register
    this->_wrcmd(0xD3); // Set SEG Output Current Brightness
    this->_wrcmd(0x00); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    this->_wrcmd(0x40); //Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    this->_wrcmd(0xA1); //--set normal display
    this->_wrcmd(0xC8); //--set multiplex ratio(1 to 64)
    this->_wrcmd(0xDA); //--1/64 duty
    this->_wrcmd(0x2); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    this->_wrcmd(0x00); //-not offset
    this->_wrcmd(0xd5); //--set display clock divide ratio/oscillator frequency
    this->_wrcmd(0x80); //--set divide ratio, Set Clock as 100 Frames/Sec
    this->_wrcmd(0xD9); //--set pre-charge period
    this->_wrcmd(0xF1); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    this->_wrcmd(0xDA); //--set com pins hardware configuration
    this->_wrcmd(0x12);
    this->_wrcmd(0x81); //--set vcomh
    this->_wrcmd(0xBF); //Set VCOM Deselect Level
    this->_wrcmd(0xD9); //-Set Page Addressing Mode (0x00/0x01/0x02)
    this->_wrcmd(0x25); //
    this->_wrcmd(0xDB); //--set Charge Pump enable/disable
    this->_wrcmd(0x34); //--set(0x10) disable
    this->_wrcmd(0xA4); // Disable Entire Display On (0xa4/0xa5)
    this->_wrcmd(0xA6); // Disable Inverse Display On (0xa6/a7) 
    this->_wrcmd(0xAF); //--turn on oled panel
    this->_wrcmd(0xAF); /*display ON*/
    this->Clear();
    this->SetPos(0, 0);
}
