#include "stm32f10x.h"
#include "Sys.h"
#include "oledfont.h"  
#include "Drivers\SSD1309.h"

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
//-----------------OLED�˿ڶ���----------------  					   
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOD,GPIO_Pin_3)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOD,GPIO_Pin_3)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_4)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOD,GPIO_Pin_4)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_5)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOD,GPIO_Pin_5)

#define OLED_WR_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_14)
#define OLED_WR_Set() GPIO_SetBits(GPIOG,GPIO_Pin_14)

#define OLED_RD_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_13)
#define OLED_RD_Set() GPIO_SetBits(GPIOG,GPIO_Pin_13)



//PC0~7,��Ϊ������
#define DATAOUT(x) GPIO_Write(GPIOC,x);//���  
//ʹ��4�ߴ��нӿ�ʱʹ�� 

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_6)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOD,GPIO_Pin_6)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_7)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOD,GPIO_Pin_7)


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
    //��SSD1106д��һ���ֽڡ�
    //dat:Ҫд�������/����
    //cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
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

//����OLED��ʾ    
void SSD1309::OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC����
    OLED_WR_Byte(0X14, OLED_CMD); //DCDC ON
    OLED_WR_Byte(0XAF, OLED_CMD); //DISPLAY ON
}

//�ر�OLED��ʾ     
void SSD1309::OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC����
    OLED_WR_Byte(0X10, OLED_CMD); //DCDC OFF
    OLED_WR_Byte(0XAE, OLED_CMD); //DISPLAY OFF
}

//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void SSD1309::OLED_Clear(void)
{
    byte i, n;
    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); //����ҳ��ַ��0~7��
        OLED_WR_Byte(0x00, OLED_CMD); //������ʾλ�á��е͵�ַ
        OLED_WR_Byte(0x10, OLED_CMD); //������ʾλ�á��иߵ�ַ   
        for (n = 0; n < 128; n++)
            OLED_WR_Byte(0, OLED_DATA);
    } //������ʾ
}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void SSD1309::OLED_ShowChar(byte x, byte y, byte chr)
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

//��ʾһ���ַ��Ŵ�
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

//��ʾ����
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

/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
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
//��ʼ��SSD1306					    
void SSD1309::OLED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG, ENABLE); //ʹ��PC,D,G�˿�ʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_3 | GPIO_Pin_8; //PD3,PD6�������  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz
    GPIO_Init(GPIOD, &GPIO_InitStructure); //��ʼ��GPIOD3,6
    GPIO_SetBits(GPIOD, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_3 | GPIO_Pin_8); //PD3,PD6 �����

    #if OLED_MODE==1

        GPIO_InitStructure.GPIO_Pin = 0xFF; //PC0~7 OUT�������
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        GPIO_SetBits(GPIOC, 0xFF); //PC0~7�����
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //PG13,14,15 OUT�������
        GPIO_Init(GPIOG, &GPIO_InitStructure);
        GPIO_SetBits(GPIOG, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15); //PG13,14,15 OUT  �����
    #else 
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //PC0,1 OUT�������
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1); //PC0,1 OUT  �����
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //PG15 OUT�������	  RST
        GPIO_Init(GPIOG, &GPIO_InitStructure);
        GPIO_SetBits(GPIOG, GPIO_Pin_15); //PG15 OUT  �����
    #endif 
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
    OLED_WR_Byte(0x00, OLED_CMD); //--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    OLED_WR_Byte(0x40, OLED_CMD); //Set COM/Row Scan Direction   0xc0���·��� 0xc8����
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
