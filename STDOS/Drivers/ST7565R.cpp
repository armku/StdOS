/*
Bresenham算法画直线和圆，硬件条件stm32（fmsc）+ili9325
直线算法参照  http://www.cnblogs.com/soroman/archive/2008/09/30/509602.html  的算法
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
函 数 名: Init
功能说明: LCD模块初始化
形    参：无
返 回 值: 无
 ********************************************************************************************************/
void CLcd::Init()
{
    this->pPincs = 0;
    this->pPinres = 0; /*低电平复位*/
    Sys.Delay(500);
    this->pPinres = 1; /*复位完毕*/
    Sys.Delay(500);
    #if 0    
        this->writeCMD(0xe2); /*软复位*/
        Sys.Delay(500);
        this->writeCMD(0x2c); /*升压步聚1*/
        Sys.Delay(500);
        this->writeCMD(0x2e); /*升压步聚2*/
        Sys.Delay(500);
        this->writeCMD(0x2f); /*升压步聚3*/
        Sys.Delay(500);
        this->writeCMD(0x24); //0x24粗调对比度，可设置范围0x20～0x27
        this->writeCMD(0x81); /*微调对比度*/
        this->writeCMD(0x1a); //微调对比度的值，可设置范围0x00～0x3f
        this->writeCMD(0xa2); //1/9偏压比（bias）
        this->writeCMD(0xc0); //行扫描顺序：从上到下
        this->writeCMD(0xa1); //列扫描顺序：从左到右
        this->writeCMD(0x40); //起始行：第一行开始,原来那款“JLX12864G-202”型号的这里是“0x60”的
        this->writeCMD(0xaf); //开显示
    #else 
        this->writeCMD(0xe2); //用软件方式复位ST7565R 
        Sys.Delay(20);
        this->writeCMD(0xa2); //LCD偏压设置 （该寄存器的值请不要改动）
        this->writeCMD(0xa1); //横向刷屏方向设置  0xa0:从左向右  0xa1：从右向左
        this->writeCMD(0xc0); //纵向刷屏方向设置  0xc0:从下向上  0xc8:从上向下
        this->writeCMD(0x2f); //内部升压电路使能设置（该寄存器的值请不要改动）
        this->writeCMD(0x24); //粗调显示浓度（该寄存器值的范围为：0x20--0x27）
        this->writeCMD(0x81);
        this->writeCMD(0x28); //细调显示浓度（该寄存器值的范围为：0x01--0x3f）
        this->writeCMD(0xa8); //反白显示设置  0xa6:正常显示  0xa7:反白显示
        this->writeCMD(0xaf); //开启显示		
    #endif 
    this->pPincs = 1;

    this->Cls();
}

/********************************************************************************************************
函 数 名: writeCMD
功能说明: 写指令到LCD模块
形    参：指令 data1
返 回 值: 无
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
函 数 名: writeData
功能说明: 写数据到LCD模块
形    参：数据 data1
返 回 值: 无
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
函 数 名: Test
功能说明: 显示测试画面：例如全显示，隔行显示，隔列显示，雪花显示
形    参1：数据
形    参2：数据
返 回 值: 无
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
函 数 名: SetAddress
功能说明: 写入地址 页 列
形    参1：页
形    参2：列
返 回 值: 无
 ********************************************************************************************************/
void CLcd::SetAddress(byte page, byte column) //写入地址 页 列
{
    column = column + 3;
    page = page - 1;
    this->writeCMD(0xb0 + page); //设置页地址。每页是8行。一个画面的64行被分成8个页。我们平常所说的第1页，在LCD驱动IC里是第0页，所以在这里减去1*/
    this->writeCMD(((column >> 4) &0x0f) + 0x10); //设置列地址的高4位
    this->writeCMD(column &0x0f); //设置列地址的低4位
}


/*********************************************************************************************************
函 数 名: Point
功能说明: 画点
形    参1: x  X轴坐标
形    参2: y  X轴坐标
形    参3: reverse 反显
返 回 值: 无
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

//画图形
void CLcd::DisplayImage(byte *pbuf, ushort color, uint showtype)
{
    switch (showtype)
    {
        case 0:
            //纵向取模下位高
            for (int i = 0; i < 128 *64 / 8; i++)
            {
                ((byte*)(this->Interface_Table))[i] = color ? pbuf[i]: pbuf[i] ^ 0XFF;
            }
            break;
        case 1:
            //横向取模
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

//读取颜色
ushort CLcd::readPoint(ushort x, ushort y)
{
    return (this->Interface_Table[y / 8][x] &(1 << (y % 8)));
}

//求绝对值
//static ushort abs(int16_t x)
//{
//    if (x < 0)
//    {
//        x =  - x;
//    }
//    return x;
//}
//画线
void CLcd::Line(ushort x0, ushort y0, ushort x1, ushort y1)
{
    #if 0
        //Bresenham算法画直线
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
        //斜率小于等于1
        {
            if (x0 > x1)
            //互换
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
            num = (y1 - y0) > 0 ? 1 :  - 1; //判断加减
            sub = (dy << 1) - dx; //sub=dy*2-dx;
            this->Point(x0, y0, color); //起点
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
                this->Point(X, Y, color); //打点        
            }
        }
        else
        //斜率大于1
        {
            if (y0 > y1)
            //互换
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
            num = (x1 - x0) > 0 ? 1 :  - 1; //判断加减
            sub = (dx << 1) - dy; //sub=dy*2-dx;
            this->Point(x0, y0, color); //起点
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
                this->Point(X, Y, color); //打点        
            }
        }
    #else 
        //x0,y0起点坐标；x1,y1终点坐标；
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

//矩形
void CLcd::Rect(ushort x, ushort y, ushort width, ushort height)
{
    this->Line(x, y, x + width, y);
    this->Line(x, y + height, x + width, y + height);

    this->Line(x, y, x, y + height);
    this->Line(x + width, y, x + width, y + height);
}

//实心矩形
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

//Bresenham算法画直线和圆
void CLcd::Circle(ushort x0, ushort y0, ushort R)
{
    ushort a = 0, b = 0;
    int16_t di = 0;
    b = R;
    di = 1-R;
    while (a <= b)
    {
        //8块分别打点
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
函 数 名: Display4x8
功能说明: 显示ascall码
形    参1: x  X轴坐标
形    参2: y  Y轴坐标
形    参3: *text 显示内容
形    参4: color 颜色
返 回 值: 无
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
函 数 名: Display5x8
功能说明: 显示ascall码
形    参1: x  X轴坐标
形    参2: y  Y轴坐标
形    参3: *text 显示内容
形    参4: color 颜色
返 回 值: 无
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
函 数 名: Display6x12
功能说明: 显示6x12点阵图像、ASCII, 或8x16点阵的自造字符、其他图标
形    参1: x  X轴坐标
形    参2: y  Y轴坐标
形    参3: *dp 显示内容
形    参4: color 颜色
返 回 值: 无
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
函 数 名: Display6x16
功能说明: 显示6x16点阵图像、ASCII, 或8x16点阵的自造字符、其他图标
形    参1: x  X轴坐标
形    参2: y  Y轴坐标
形    参3: *dp 显示内容
形    参4: color 颜色
返 回 值: 无
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
函 数 名: Display8x16
功能说明: 显示8x16点阵图像、ASCII, 或8x16点阵的自造字符、其他图标
形    参1: x  X轴坐标
形    参2: y  Y轴坐标
形    参3: *dp 显示内容
形    参4: color 颜色
返 回 值: 无
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
函 数 名: Display12x12
功能说明: 显示12x12点阵图像、汉字、生僻字或16x16点阵的其他图标
形    参1: x  X轴坐标
形    参2: y  Y轴坐标
形    参3: *dp 显示内容
形    参4: color 颜色
返 回 值: 无
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
函 数 名: Display16x16
功能说明: 显示16x16点阵图像、汉字、生僻字或16x16点阵的其他图标
形    参1: x  X轴坐标
形    参2: y  Y轴坐标
形    参3: *dp 显示内容
形    参4: color 颜色
返 回 值: 无
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
函 数 名: Display16x32
功能说明: 显示16x32点阵图像、汉字、生僻字或32x32点阵的其他图标
形    参1: x  X轴坐标
形    参2: y  Y轴坐标
形    参3: *dp 显示内容
形    参4: color 颜色
返 回 值: 无
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
函 数 名: Display32x32
功能说明: 显示32x32点阵图像、汉字、生僻字或32x32点阵的其他图标
形    参1: x  X轴坐标
形    参2: y  Y轴坐标
形    参3: *dp 显示内容
返 回 值: 无
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
函 数 名: Flush
功能说明: 刷屏
形    参: 无
返 回 值: 无
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

//是否在阴影区域
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

//阴影部分复位
void CLcd::ShadowReset()
{
    for (int i = 0; i < 3; i++)
    {
        this->ShadowRect[i].enable = false;
    }
}

//阴影启用，默认启用通道0
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
