#include "SSD1309.h"
#include "../../../User/ssd1309font.h"

//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
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

//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   

#if OLED_MODE==1
    //��SSD1106д��һ���ֽڡ�
    //dat:Ҫд�������/����
    //cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
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
    //��SSD1106д��һ���ֽڡ�
    //dat:Ҫд�������/����
    //cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
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
//д����
void SSD1309::_wrcmd(uint8_t cmd)
{
	this->WRByte(cmd,0);
}
//д����
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

//����OLED��ʾ    
void SSD1309::DisplayOn()
{
    this->_wrcmd(0X8D); //SET DCDC����
    this->_wrcmd(0X14); //DCDC ON
    this->_wrcmd(0XAF); //DISPLAY ON
}

//�ر�OLED��ʾ     
void SSD1309::DisplayOff()
{
    this->_wrcmd(0X8D); //SET DCDC����
    this->_wrcmd(0X10); //DCDC OFF
    this->_wrcmd(0XAE); //DISPLAY OFF
}

//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void SSD1309::Clear(char ch)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        this->_wrcmd(0xb0 + i); //����ҳ��ַ��0~7��
        this->_wrcmd(0x00); //������ʾλ�á��е͵�ַ
        this->_wrcmd(0x10); //������ʾλ�á��иߵ�ַ   
        for (n = 0; n < 128; n++)
            this->_wrdata(ch);
    } //������ʾ
}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void SSD1309::ShowChar(uint8_t x, uint8_t y, uint8_t chr)
{
    unsigned char c = 0, i = 0;
    c = chr - ' '; //�õ�ƫ�ƺ��ֵ			
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

//m^n����
uint32_t SSD1309::oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while (n--)
        result *= m;
    return result;
}

//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
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
//��ʾһ���ַ��Ŵ�
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
//��ʾ����
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
//���Һ��ֱ���λ��
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

/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
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

//��ʼ��SSD1306					    
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
    this->_wrcmd(0x00); //--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    this->_wrcmd(0x40); //Set COM/Row Scan Direction   0xc0���·��� 0xc8����
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
