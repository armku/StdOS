/*
Bresenham�㷨��ֱ�ߺ�Բ��Ӳ������stm32��fmsc��+ili9325
ֱ���㷨����  http://www.cnblogs.com/soroman/archive/2008/09/30/509602.html  ���㷨
http://www.amobbs.com/forum.php?mod=viewthread&tid=5296612&extra=&ordertype=1
 */
#include "ST7565R.h"

CLcd::CLcd()
{
    this->ShadowReset();
}

CLcd::CLcd(Pin pinrs, Pin pinsclk, Pin pinsid, Pin pinres, Pin pincs)
{
	this->pPinRS.Invert=false;
    this->pPinsclk.Invert=false;
    this->pPinsid.Invert=false;
    this->pPinres.Invert=false;
    this->pPincs.Invert=false;
	
    this->pPinRS.Set(pinrs);
    this->pPinsclk.Set(pinsclk);
    this->pPinsid.Set(pinsid);
    this->pPinres.Set(pinres);
    this->pPincs.Set(pincs);
    this->ShadowReset();
}
/********************************************************************************************************
�� �� ��: Init
����˵��: LCDģ���ʼ��
��    �Σ���
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd::Init()
{
    this->pPincs = 0;
    this->pPinres = 0; /*�͵�ƽ��λ*/
    Sys.Delay(500);
    this->pPinres = 1; /*��λ���*/
    Sys.Delay(500);
    #if 0    
        this->writeCMD(0xe2); /*��λ*/
        Sys.Delay(500);
        this->writeCMD(0x2c); /*��ѹ����1*/
        Sys.Delay(500);
        this->writeCMD(0x2e); /*��ѹ����2*/
        Sys.Delay(500);
        this->writeCMD(0x2f); /*��ѹ����3*/
        Sys.Delay(500);
        this->writeCMD(0x24); //0x24�ֵ��Աȶȣ������÷�Χ0x20��0x27
        this->writeCMD(0x81); /*΢���Աȶ�*/
        this->writeCMD(0x1a); //΢���Աȶȵ�ֵ�������÷�Χ0x00��0x3f
        this->writeCMD(0xa2); //1/9ƫѹ�ȣ�bias��
        this->writeCMD(0xc0); //��ɨ��˳�򣺴��ϵ���
        this->writeCMD(0xa1); //��ɨ��˳�򣺴�����
        this->writeCMD(0x40); //��ʼ�У���һ�п�ʼ,ԭ���ǿJLX12864G-202���ͺŵ������ǡ�0x60����
        this->writeCMD(0xaf); //����ʾ
    #else 
        this->writeCMD(0xe2); //�������ʽ��λST7565R 
        Sys.Delay(20);
        this->writeCMD(0xa2); //LCDƫѹ���� ���üĴ�����ֵ�벻Ҫ�Ķ���
        this->writeCMD(0xa1); //����ˢ����������  0xa0:��������  0xa1����������
        this->writeCMD(0xc0); //����ˢ����������  0xc0:��������  0xc8:��������
        this->writeCMD(0x2f); //�ڲ���ѹ��·ʹ�����ã��üĴ�����ֵ�벻Ҫ�Ķ���
        this->writeCMD(0x24); //�ֵ���ʾŨ�ȣ��üĴ���ֵ�ķ�ΧΪ��0x20--0x27��
        this->writeCMD(0x81);
        this->writeCMD(0x28); //ϸ����ʾŨ�ȣ��üĴ���ֵ�ķ�ΧΪ��0x01--0x3f��
        this->writeCMD(0xa8); //������ʾ����  0xa6:������ʾ  0xa7:������ʾ
        this->writeCMD(0xaf); //������ʾ		
    #endif 
    this->pPincs = 1;

    this->Cls();
}

/********************************************************************************************************
�� �� ��: writeCMD
����˵��: дָ�LCDģ��
��    �Σ�ָ�� data1
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd::writeCMD(byte data1)
{
    this->pPincs = 0;
    this->pPinRS = 0;
    for (byte i = 0; i < 8; i++)
    {
        this->pPinsclk = 0;
        //	Sys.Delay(10);
        if (data1 &0x80)
        {
            this->pPinsid = 1;
        }
        else
        {
            this->pPinsid = 0;
        }
        this->pPinsclk = 1;
        //	Sys.Delay(10);
        data1 = data1 <<= 1;
    }
    this->pPincs = 1;
}

/*********************************************************************************************************
�� �� ��: writeData
����˵��: д���ݵ�LCDģ��
��    �Σ����� data1
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd::writeData(byte data1)
{
    this->pPincs = 0;
    this->pPinRS = 1;
    for (byte i = 0; i < 8; i++)
    {
        this->pPinsclk = 0;
        if (data1 &0x80)
        {
            this->pPinsid = 1;
        }
        else
        {
            this->pPinsid = 0;
        }
        this->pPinsclk = 1;
        data1 = data1 <<= 1;
    }
    this->pPincs = 1;
}

/********************************************************************************************************
�� �� ��: Test
����˵��: ��ʾ���Ի��棺����ȫ��ʾ��������ʾ��������ʾ��ѩ����ʾ
��    ��1������
��    ��2������
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd::Test(byte data1, byte data2)
{
    for (byte j = 0; j < 8; j++)
    {
        this->pPincs = 0;
        this->SetAddress(j + 1, 0);
        for (byte i = 0; i < 128; i++)
        {
            this->writeData(data1);
            this->writeData(data2);
        }
    }
}

/*********************************************************************************************************
�� �� ��: SetAddress
����˵��: д���ַ ҳ ��
��    ��1��ҳ
��    ��2����
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd::SetAddress(byte page, byte column) //д���ַ ҳ ��
{
    column = column + 3;
    page = page - 1;
    this->writeCMD(0xb0 + page); //����ҳ��ַ��ÿҳ��8�С�һ�������64�б��ֳ�8��ҳ������ƽ����˵�ĵ�1ҳ����LCD����IC���ǵ�0ҳ�������������ȥ1*/
    this->writeCMD(((column >> 4) &0x0f) + 0x10); //�����е�ַ�ĸ�4λ
    this->writeCMD(column &0x0f); //�����е�ַ�ĵ�4λ
}


/*********************************************************************************************************
�� �� ��: Point
����˵��: ����
��    ��1: x  X������
��    ��2: y  X������
��    ��3: reverse ����
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd::Point(ushort x, ushort y, ushort color)
{
    if (((color) && (!this->inShadow(x, y))) || ((!color) && (this->inShadow(x, y))))
    {
        this->Interface_Table[y / 8][x] |= (1 << (y % 8));
    }
    else
    {
        this->Interface_Table[(y) / 8][x] &= 0xff ^ (1 << (y % 8));
    }
}

//��ͼ��
void CLcd::DisplayImage(byte *pbuf, ushort color, uint showtype)
{
    switch (showtype)
    {
        case 0:
            //����ȡģ��λ��
            for (int i = 0; i < 128 *64 / 8; i++)
            {
                ((byte*)(this->Interface_Table))[i] = color ? pbuf[i]: pbuf[i] ^ 0XFF;
            }
            break;
        case 1:
            //����ȡģ
            for (int i = 0; i < 128 *8; i++)
            {
                for (int temp = 0x80, j = 0; j < 8; j++)
                {
                    if ((temp &pbuf[i]) == 0x00)
                    {
                        Point((i % 16) *8+j, i / 16, 1);
                    }
                    else
                    {
                        Point((i % 16) *8+j, i / 16, 0);
                    }
                    temp >>= 1;
                }
            }
            break;
        default:
            break;
    }

    this->Flush();
}

//��ȡ��ɫ
ushort CLcd::readPoint(ushort x, ushort y)
{
    return (this->Interface_Table[y / 8][x] &(1 << (y % 8)));
}

//�����ֵ
//static ushort abs(int16_t x)
//{
//    if (x < 0)
//    {
//        x =  - x;
//    }
//    return x;
//}
//����
void CLcd::Line(ushort x0, ushort y0, ushort x1, ushort y1)
{
    #if 0
        //Bresenham�㷨��ֱ��
        ushort X = 0, Y = 0;
        int16_t dx = 0, dy = 0;

        int16_t sub = 0;
        int16_t num = 0;
        ushort temp = 0;
        if ((x0 == x1) && (y0 == y1))
        {
            return ;
        }
        dx = abs(x1 - x0);
        dy = abs(y1 - y0);
        if (dx >= dy)
        //б��С�ڵ���1
        {
            if (x0 > x1)
            //����
            {
                temp = x1;
                x1 = x0;
                x0 = temp;
                temp = y1;
                y1 = y0;
                y0 = temp;
            }
            dx = abs(x1 - x0);
            dy = abs(y1 - y0);
            num = (y1 - y0) > 0 ? 1 :  - 1; //�жϼӼ�
            sub = (dy << 1) - dx; //sub=dy*2-dx;
            this->Point(x0, y0, color); //���
            for (X = x0, Y = y0; X <= x1; X++)
            {
                if (sub > 0)
                {
                    Y += num;
                    sub += (dy << 1) - (dx << 1); //sub=2*dy-2*dx        
                }
                else
                {
                    sub += dy << 1;
                }
                this->Point(X, Y, color); //���        
            }
        }
        else
        //б�ʴ���1
        {
            if (y0 > y1)
            //����
            {
                temp = x1;
                x1 = x0;
                x0 = temp;
                temp = y1;
                y1 = y0;
                y0 = temp;
            }
            dx = abs(x1 - x0);
            dy = abs(y1 - y0);
            num = (x1 - x0) > 0 ? 1 :  - 1; //�жϼӼ�
            sub = (dx << 1) - dy; //sub=dy*2-dx;
            this->Point(x0, y0, color); //���
            for (X = x0, Y = y0; Y <= y1; Y++)
            {
                if (sub > 0)
                {
                    X += num;
                    sub += (dx << 1) - (dy << 1); //sub=2*dy-2*dx        
                }
                else
                {
                    sub += dx << 1;
                }
                this->Point(X, Y, color); //���        
            }
        }
    #else 
        //x0,y0������ꣻx1,y1�յ����ꣻ
        ushort x, y;
        ushort d_x, d_y;
        int16_t err = 0;
        ushort temp = 0;

        if (y1 < y0)
        {
            x = x0;
            y = y0;
            x0 = x1;
            y0 = y1;
            x1 = x;
            y1 = y;
        }
        d_y = y1 - y0;
        if (d_y == 0)
        {
            if (x0 > x1)
            {
                x = x0;
                x0 = x1;
                x1 = x;
            }
            for (x = x0; x <= x1; x++)
            {
                this->Point(x, y0);
            }
        }
        else
        {
            if (x1 >= x0)
            {
                temp = 1;
                d_x = x1 - x0;
            }
            else
                d_x = x0 - x1;
            x = x0;
            y = y0;
            this->Point(x, y);
            if (temp && (d_y <= d_x))
            while (x != x1)
            {
                if (err < 0)
                {
                    x = x + 1;
                    err = err + (y1 - y);
                }
                else
                {
                    x = x + 1;
                    y = y + 1;
                    err = err + (y1 - y) - (x1 - x);
                }
                this->Point(x, y);
            }
            else if (temp && (d_y > d_x))
            while (y != y1)
            {
                d_x = x1 - x;
                d_y = y1 - y;
                if (err < 0)
                {
                    x = x + 1;
                    y = y + 1;
                    err = err + d_y - d_x;
                }
                else
                {
                    y = y + 1;
                    err = err - d_x;
                }
                this->Point(x, y);
            }
            else if (!temp && (d_y <= d_x))
            while (x != x1)
            {
                d_x = x - x1;
                d_y = y1 - y;
                if (err < 0)
                {
                    x = x - 1;
                    err = err + d_y;
                }
                else
                {
                    x = x - 1;
                    y = y + 1;
                    err = err + d_y - d_x;
                }
                this->Point(x, y);
            }
            else if (!temp && (d_y > d_x))
            while (y != y1)
            {
                d_x = x - x1;
                d_y = y1 - y;
                if (err < 0)
                {
                    x = x - 1;
                    y = y + 1;
                    err = err + d_y - d_x;
                }
                else
                {
                    y = y + 1;
                    err = err - d_x;
                }
                this->Point(x, y);
            }
        }
    #endif 
}

//����
void CLcd::Rect(ushort x, ushort y, ushort width, ushort height)
{
    this->Line(x, y, x + width, y);
    this->Line(x, y + height, x + width, y + height);

    this->Line(x, y, x, y + height);
    this->Line(x + width, y, x + width, y + height);
}

//ʵ�ľ���
void CLcd::RectSolid(ushort x0, ushort y0, ushort width, ushort height)
{
    for (ushort x = x0; x < x0 + width; x++)
    {
        for (ushort y = y0; y < y0 + height; y++)
        {
            this->Point(x, y);
        }
    }
}

//Bresenham�㷨��ֱ�ߺ�Բ
void CLcd::Circle(ushort x0, ushort y0, ushort R)
{
    ushort a = 0, b = 0;
    int16_t di = 0;
    b = R;
    di = 1-R;
    while (a <= b)
    {
        //8��ֱ���
        this->Point(x0 + b, y0 - a); //0
        this->Point(x0 - a, y0 + b); //1
        this->Point(x0 - a, y0 - b); //2
        this->Point(x0 - b, y0 - a); //3
        this->Point(x0 + b, y0 + a); //4
        this->Point(x0 + a, y0 - b); //5
        this->Point(x0 + a, y0 + b); //6
        this->Point(x0 - b, y0 + a); //7
        a++;
        if (di < 0)
        {
            di += (a << 1) + 3;
        }
        else
        {
            di += ((a - b) << 1) + 5;
            b--;
        }
    }
}

/*********************************************************************************************************
�� �� ��: Display4x8
����˵��: ��ʾascall��
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: *text ��ʾ����
��    ��4: color ��ɫ
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd::Display4x8(ushort x, ushort y, byte *dp)
{
    for (byte i = 0; i < 4; i++)
    {
        byte bitMap = 0x01;
        for (byte mi = 0; mi < 8; mi++)
        {
            this->Point(x + i, y + mi, bitMap &dp[i] ? 1 : 0);
            bitMap <<= 1;
        }
    }

}

/*********************************************************************************************************
�� �� ��: Display5x8
����˵��: ��ʾascall��
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: *text ��ʾ����
��    ��4: color ��ɫ
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd::Display5x8(ushort x, ushort y, byte *dp)
{
    for (byte i = 0; i < 5; i++)
    {
        byte bitMap = 0x01;
        for (byte mi = 0; mi < 8; mi++)
        {
            this->Point(x + i, y + mi, bitMap &dp[i] ? 1 : 0);
            bitMap <<= 1;
        }
    }

}

/*********************************************************************************************************
�� �� ��: Display6x12
����˵��: ��ʾ6x12����ͼ��ASCII, ��8x16����������ַ�������ͼ��
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: *dp ��ʾ����
��    ��4: color ��ɫ
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd::Display6x12(ushort x, ushort y, byte *dp)
{
    for (byte j = 0; j < 2; j++)
    {
        for (byte i = 0; i < 6; i++)
        {
            byte bitMap = 0x01;
            for (byte mi = 0; mi < 6; mi++)
            {
                this->Point(x + i, y + mi + 6 * j, bitMap &dp[i + 6 * j] ? 1 : 0);
                bitMap <<= 1;
            }
        }
    }
}
/*********************************************************************************************************
�� �� ��: Display6x16
����˵��: ��ʾ6x16����ͼ��ASCII, ��8x16����������ַ�������ͼ��
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: *dp ��ʾ����
��    ��4: color ��ɫ
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd::Display6x16(ushort x, ushort y, byte *dp)
{
    for (byte j = 0; j < 2; j++)
    {
        for (byte i = 0; i < 8; i++)
        {
            byte bitMap = 0x01;
            for (byte mi = 0; mi < 8; mi++)
            {
                this->Point(x + i, y + mi + 8 * j, bitMap &dp[i + 8 * j] ? 1 : 0);
                bitMap <<= 1;
            }
        }
    }
}
/*********************************************************************************************************
�� �� ��: Display8x16
����˵��: ��ʾ8x16����ͼ��ASCII, ��8x16����������ַ�������ͼ��
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: *dp ��ʾ����
��    ��4: color ��ɫ
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd::Display8x16(ushort x, ushort y, byte *dp)
{
    for (byte j = 0; j < 2; j++)
    {
        for (byte i = 0; i < 8; i++)
        {
            byte bitMap = 0x01;
            for (byte mi = 0; mi < 8; mi++)
            {
                this->Point(x + i, y + mi + 8 * j, bitMap &dp[i + 8 * j] ? 1 : 0);
                bitMap <<= 1;
            }
        }
    }
}

/*********************************************************************************************************
�� �� ��: Display12x12
����˵��: ��ʾ12x12����ͼ�񡢺��֡���Ƨ�ֻ�16x16���������ͼ��
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: *dp ��ʾ����
��    ��4: color ��ɫ
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd::Display12x12(ushort x, ushort y, byte *dp)
{
	for (byte j = 0; j < 2; j++)
    {
        for (byte i = 0; i < 16; i++)
        {
            byte bitMap = 1;
            for (byte mi = 0; mi < 8; mi++)
            {
                this->Point(x + i, y + mi + 8 * j, 0);
                bitMap <<= 1;
            }
        }
    }
	
    for (byte i = 0; i < 12; i++)
    {
        byte bitMap = 0x01;
        for (byte mi = 0; mi < 8; mi++)
        {
            this->Point(x + i, y + mi+2, bitMap &dp[i] ? 1 : 0);
            bitMap <<= 1;
        }
    }
    for (byte i = 0; i < 12; i++)
    {
        byte bitMap = 0x01;
        for (byte mi = 0; mi < 4; mi++)
        {
            this->Point(x + i, y + mi + 8+2, bitMap &dp[i + 12] ? 1 : 0);
            bitMap <<= 1;
        }
    }	 
}

/*********************************************************************************************************
�� �� ��: Display16x16
����˵��: ��ʾ16x16����ͼ�񡢺��֡���Ƨ�ֻ�16x16���������ͼ��
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: *dp ��ʾ����
��    ��4: color ��ɫ
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd::Display16x16(ushort x, ushort y, byte *dp)
{
    for (byte j = 0; j < 2; j++)
    {
        for (byte i = 0; i < 16; i++)
        {
            byte bitMap = 1;
            for (byte mi = 0; mi < 8; mi++)
            {
                this->Point(x + i, y + mi + 8 * j, bitMap &dp[i + 16 * j] ? 1 : 0);
                bitMap <<= 1;
            }
        }
    }
}

/*********************************************************************************************************
�� �� ��: Display16x32
����˵��: ��ʾ16x32����ͼ�񡢺��֡���Ƨ�ֻ�32x32���������ͼ��
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: *dp ��ʾ����
��    ��4: color ��ɫ
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd::Display16x32(ushort x, ushort y, byte *dp)
{
    for (byte j = 0; j < 2; j++)
    {
        for (byte i = 0; i < 31; i++)
        {
            byte bitMap = 0x01;
            for (byte mi = 0; mi < 8; mi++)
            {
                this->Point(x + i, y + mi + 8 * j, bitMap &dp[i + 32 * j] ? 1 : 0);
                bitMap <<= 1;
            }
        }
    }
}

/*********************************************************************************************************
�� �� ��: Display32x32
����˵��: ��ʾ32x32����ͼ�񡢺��֡���Ƨ�ֻ�32x32���������ͼ��
��    ��1: x  X������
��    ��2: y  Y������
��    ��3: *dp ��ʾ����
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd::Display32x32(ushort x, ushort y, byte *dp)
{
    for (byte j = 0; j < 4; j++)
    {
        for (byte i = 0; i < 31; i++)
        {
            byte bitMap = 0x01;
            for (byte mi = 0; mi < 8; mi++)
            {
                this->Point(x + i, y + mi + 8 * j, bitMap &dp[i + 32 * j] ? 1 : 0);
                bitMap <<= 1;
            }
        }
    }
}

void CLcd::Cls(ushort x, ushort y, ushort width, ushort height)
{
    this->ShadowReset();
    for (ushort i = x; i < x + width; i++)
    {
        for (ushort j = y; j < y + height; j++)
        {
            this->Point(i, j, 0);
        }
    }
}

/*********************************************************************************************************
�� �� ��: Flush
����˵��: ˢ��
��    ��: ��
�� �� ֵ: ��
 ********************************************************************************************************/
void CLcd::Flush()
{
    for (ushort j = 0; j < 8; j++)
    {
        this->SetAddress(j + 1, 1);
        for (ushort i = 0; i < 128; i++)
        {
            this->writeData(this->Interface_Table[j][i]);
        }
    }
}

//�Ƿ�����Ӱ����
bool CLcd::inShadow(ushort x, ushort y)
{
    bool ret = false;

    for (int i = 0; i < 3; i++)
    {
        if (this->ShadowRect[i].enable)
        {
            if ((x >= this->ShadowRect[i].x) && (x <= (this->ShadowRect[i].x + this->ShadowRect[i].width)) && (y >= this->ShadowRect[i].y) && (y <= (this->ShadowRect[i].y + this
                ->ShadowRect[i].height)))
            {
                ret = true;
            }
        }
    }

    return ret;
}

//��Ӱ���ָ�λ
void CLcd::ShadowReset()
{
    for (int i = 0; i < 3; i++)
    {
        this->ShadowRect[i].enable = false;
    }
}

//��Ӱ���ã�Ĭ������ͨ��0
void CLcd::ShadowOn(ushort x, ushort y, ushort width, ushort height, byte id)
{
    if (id >= 3)
    {
        return ;
    }
    this->ShadowRect[id].enable = true;
    this->ShadowRect[id].x = x;
    this->ShadowRect[id].y = y;
    this->ShadowRect[id].width = width;
    this->ShadowRect[id].height = height;
}
#if 0
	CLcd lcddr1(PD3, PD6, PD7, PB3, PB4);
	void ST7565Rtest()
	{
		
		lcddr1.Init();
		for(int i=0;i<10;i++)
		{
			lcddr1.Test(0x55,0xAA);
			Sys.Sleep(500);
			lcddr1.Test(0x00,0x00);
			Sys.Sleep(500);
		}
		lcddr1.Test(0x55,0xAA);
	}
#endif
