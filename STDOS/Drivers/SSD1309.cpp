#include "SSD1309.h"
#include "Fonts\ssd1309font.h"  

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

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

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
    void SSD1309::this->WRByte(byte dat, byte cmd)
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
    void SSD1309::WRByte(byte dat, byte cmd)
    {
        byte i;
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
void SSD1309::SetPos(byte x, byte y)
{
    this->WRByte(0xb0 + y, OLED_CMD);
    this->WRByte(((x &0xf0) >> 4) | 0x10, OLED_CMD);
    this->WRByte((x &0x0f) | 0x01, OLED_CMD);
}

//����OLED��ʾ    
void SSD1309::DisplayOn()
{
    this->WRByte(0X8D, OLED_CMD); //SET DCDC����
    this->WRByte(0X14, OLED_CMD); //DCDC ON
    this->WRByte(0XAF, OLED_CMD); //DISPLAY ON
}

//�ر�OLED��ʾ     
void SSD1309::DisplayOff()
{
    this->WRByte(0X8D, OLED_CMD); //SET DCDC����
    this->WRByte(0X10, OLED_CMD); //DCDC OFF
    this->WRByte(0XAE, OLED_CMD); //DISPLAY OFF
}

//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void SSD1309::Clear(char ch)
{
    byte i, n;
    for (i = 0; i < 8; i++)
    {
        this->WRByte(0xb0 + i, OLED_CMD); //����ҳ��ַ��0~7��
        this->WRByte(0x00, OLED_CMD); //������ʾλ�á��е͵�ַ
        this->WRByte(0x10, OLED_CMD); //������ʾλ�á��иߵ�ַ   
        for (n = 0; n < 128; n++)
            this->WRByte(ch, OLED_DATA);
    } //������ʾ
}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void SSD1309::ShowChar(byte x, byte y, byte chr)
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
            this->WRByte(F8X16[c *16+i], OLED_DATA);
        this->SetPos(x, y + 1);
        for (i = 0; i < 8; i++)
            this->WRByte(F8X16[c *16+i + 8], OLED_DATA);
    }
    else
    {
        this->SetPos(x, y + 1);
        for (i = 0; i < 6; i++)
            this->WRByte(F6x8[c][i], OLED_DATA);

    }
}

//m^n����
uint SSD1309::oled_pow(byte m, byte n)
{
    uint result = 1;
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
void SSD1309::ShowNum(byte x, byte y, uint num, byte len, byte size)
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
void SSD1309::ShowString(byte x, byte y, char *chr, byte interval)
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
void SSD1309::ShowCHinese11(byte x, byte y, byte no)
{
	byte t, adder = 0;
    this->SetPos(x, y);
    for (t = 0; t < 16; t++)
    {
        this->WRByte(gb16ssd1309[no].Msk[t], OLED_DATA);
        adder += 1;
    }
    this->SetPos(x, y + 1);
    for (t = 0; t < 16; t++)
    {
        this->WRByte(gb16ssd1309[no].Msk[16+t], OLED_DATA);
        adder += 1;
    }
}
void SSD1309::ShowCHinese(byte x, byte y, char *hz)
{
	byte lo=hz[0];
	byte hi=hz[1];
	this->ShowCHinese11(x,y,this->SearchhzIndex(lo,hi));
}
//���Һ��ֱ���λ��
ushort SSD1309::SearchhzIndex(byte lo, byte hi)
{
	ushort i = 0;
    ushort maxcount = 251;
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
void SSD1309::DrawBMP(byte x0, byte y0, byte x1, byte y1, byte BMP[])
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
            this->WRByte(BMP[j++], OLED_DATA);
        }
    }
}

//��ʼ��SSD1306					    
void SSD1309::Init()
{
    this->_res = 1;
    Sys.Sleep(100);
    //delay_ms(100);
    this->_res = 0;
    Sys.Sleep(100);
    //delay_ms(100);
    this->_res = 1;
    this->WRByte(0xFD, OLED_CMD); //--turn off oled panel
    this->WRByte(0x12, OLED_CMD); //--turn off oled panel	
    this->WRByte(0xAE, OLED_CMD); //--turn off oled panel
    this->WRByte(0xD5, OLED_CMD); //---set low column address
    this->WRByte(0xA0, OLED_CMD); //---set high column address
    this->WRByte(0xA8, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    this->WRByte(0x3F, OLED_CMD); //--set contrast control register
    this->WRByte(0xD3, OLED_CMD); // Set SEG Output Current Brightness
    this->WRByte(0x00, OLED_CMD); //--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    this->WRByte(0x40, OLED_CMD); //Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    this->WRByte(0xA1, OLED_CMD); //--set normal display
    this->WRByte(0xC8, OLED_CMD); //--set multiplex ratio(1 to 64)
    this->WRByte(0xDA, OLED_CMD); //--1/64 duty
    this->WRByte(0x2, OLED_CMD); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    this->WRByte(0x00, OLED_CMD); //-not offset
    this->WRByte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
    this->WRByte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
    this->WRByte(0xD9, OLED_CMD); //--set pre-charge period
    this->WRByte(0xF1, OLED_CMD); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    this->WRByte(0xDA, OLED_CMD); //--set com pins hardware configuration
    this->WRByte(0x12, OLED_CMD);
    this->WRByte(0x81, OLED_CMD); //--set vcomh
    this->WRByte(0xBF, OLED_CMD); //Set VCOM Deselect Level
    this->WRByte(0xD9, OLED_CMD); //-Set Page Addressing Mode (0x00/0x01/0x02)
    this->WRByte(0x25, OLED_CMD); //
    this->WRByte(0xDB, OLED_CMD); //--set Charge Pump enable/disable
    this->WRByte(0x34, OLED_CMD); //--set(0x10) disable
    this->WRByte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
    this->WRByte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7) 
    this->WRByte(0xAF, OLED_CMD); //--turn on oled panel

    this->WRByte(0xAF, OLED_CMD); /*display ON*/
    this->Clear();
    this->SetPos(0, 0);
}
