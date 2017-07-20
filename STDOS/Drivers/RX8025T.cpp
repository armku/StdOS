#include "RX8025T.h"

/* RX8025T 寄存器地址 */
#define SEC_ADDR			0x00
#define MIN_ADDR			0x01
#define HOUR_ADDR			0x02
#define WEEK_ADDR			0x03
#define DAY_ADDR			0x04
#define MONTH_ADDR		0x05
#define YEAR_ADDR			0x06
#define RAM_ADDR			0x07
#define MIN_ALM_ADDR	0x08
#define HOUR_ALM_ADDR	0x09
#define WEEK_ALM_ADDR	0x0A
#define DAY_ALM_ADDR	0x0A
#define TMR_CNT0_ADDR	0x0B
#define TMR_CNT1_ADDR	0x0C
#define EXT_REG_ADDR	0x0D
#define FLG_REG_ADDR	0x0E
#define CTL_REG_ADDR	0x0F


RX8025T::RX8025T(){}
RX8025T::RX8025T(Pin pinsck, Pin pinsda, byte devaddr, uint wnms)
{
    this->IIC.SetPin(pinsck, pinsda);
}

byte BCD2HEX(byte val)
{
    byte i;

    i = val &0x0f; //按位与，i得到低四位数。
    val >>= 4; //右移四位，将高四位移到低四位的位置，得到高四位码值。
    val &= 0x0f; //防止移位时高位补进1，只保留高四位码值
    val *= 10; //高位码值乘以10
    i += val; //然后与第四位码值相加。
    return i; //将得到的十进制数返回
}

/**
 * @brief RX8025_DEC2BCD  十进制转BCD码
 * @param val      BCD码
 * @return
 */
byte DEC2BCD(byte val)
{
    byte i;

    i = val / 10; //取得十位
    val %= 10; //取得个位
    i <<= 4; //左移4位
    i += val;

    return i;
}

void RX8025T::Init()
{
    this->devaddr = 0X64;
    //    this->IIC.Start();
    //    this->IIC.WriteByte(this->devaddr);
    //    this->IIC.WriteByte(0xe0);
    //    this->IIC.WriteByte(0x20);
    //    this->IIC.Stop();

    buf[0] = 0x00;
    RX8025_Write(0x07, buf, 1); //不使用精度调整

    buf[0] = 0x24; //0010 0100,bit5:1 24小时制，bit2:0 INTA输出1HZ电平，即秒脉冲，下降与秒计时同步
    RX8025_Write(0x0E, buf, 1); //设置24小时制

    RX8025_Read(0x0F, buf, 1); //先读寄存器
    buf[0] &= 0xFB; //CTFG设置为0
    RX8025_Write(0x0F, buf, 1); //只有这样电平模式才能使用

}


//读RX8025寄存器
void RX8025T::RX8025_Read(byte sadd, byte *buf, byte len)
{
    byte i;

    this->IIC.Start();
    this->IIC.WriteByte(this->devaddr);
    //this->IIC.WaitAck();

    this->IIC.WriteByte(sadd << 4 | 0x01);
    //this->IIC.WaitAck();

    this->IIC.Start();
    this->IIC.WriteByte(this->devaddr | 1);
    //this->IIC.WaitAck();

    for (i = 0; i < len - 1; i++)
    {
        buf[i] = this->IIC.Read(0xff);
        this->IIC.Ack(true);
    }
    buf[i] = this->IIC.Read(0xff);

    this->IIC.Ack(false);
    this->IIC.Stop();
}

//写RX8025寄存器
void RX8025T::RX8025_Write(byte sadd, byte *buf, byte len)
{
    byte i;

    this->IIC.Start();
    this->IIC.WriteByte(this->devaddr);
    this->IIC.WriteByte(sadd << 4);

    for (i = 0; i < len; i++)
    {
        this->IIC.WriteByte(buf[i]);
    }
    this->IIC.Stop();

}

void RX8025T::LoadTime(DateTime &dt)
{
    //	this->RX8025_Read(0,this->buf,10);
    //	dt.Second=this->buf[0];
    //	dt.Minute=this->buf[1];
    //	dt.Hour=this->buf[2];
    //	
    //	dt.Day=this->buf[3];
    //	dt.Month=this->buf[5];
    //	dt.Year=this->buf[6];

    RX8025_Read(0, buf, 16); //获取当前时间

    dt.Year = BCD2HEX(buf[6]) + 2000; //年
    dt.Month = BCD2HEX(buf[5]); //月
    dt.Day = BCD2HEX(buf[4]); //日
    dt.Hour = BCD2HEX(buf[2]); //时
    dt.Minute = BCD2HEX(buf[1]); //分
    dt.Second = BCD2HEX(buf[0]); //秒
}

void RX8025T::SaveTime(DateTime &dt)
{
    buf[SEC_ADDR] = DEC2BCD(dt.Second);
    buf[MIN_ADDR] = DEC2BCD(dt.Minute);
    buf[HOUR_ADDR] = DEC2BCD(dt.Hour);
    //	buf[WEEK_ADDR]=DEC2BCD(dt.DayOfWeek);
    buf[DAY_ADDR] = DEC2BCD(dt.Day);
    buf[MONTH_ADDR] = DEC2BCD(dt.Month);
    buf[YEAR_ADDR] = DEC2BCD(dt.Year - 2000);
    this->RX8025_Write(SEC_ADDR, buf, 7);
}

#if 0
    RX8025T rx8025(PB6, PB7);
    DateTime now;
    void Rx8025Refresh(void *param)
    {
        RX8025T *rx = (RX8025T*)param;
        rx->LoadTime(now);
        now.Show();
        debug_printf("\r\n");
    }
    void RX8025Test()
    {
        rx8025.Init();
        now.Year = 2017;
        now.Month = 7;
        now.Day = 20;
        //		now.DayOfWeek=4;
        now.Hour = 11;
        now.Minute = 11;
        now.Second = 2;
        rx8025.SaveTime(now);
        Sys.AddTask(Rx8025Refresh, &rx8025, 100, 1000, "TimeUp");
    }

#endif
