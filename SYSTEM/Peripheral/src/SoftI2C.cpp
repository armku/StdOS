#include "SoftI2C.h"
//#include "delay.h"

CSoftI2C::CSoftI2C(Pin pinscl, Pin pinsda, uint nus)
{
	this->psck.OpenDrain=true;
    this->psck.Set(pinscl);
    this->psda = new InputPort(pinsda);
    this->delayus = nus;

    //this->psck->SetModeOut_OD();
    this->psda->SetModeOut_OD();

    this->psck=0;
    this->psda->Set();
    this->psck=1;
}

void CSoftI2C::Init(){

}
void CSoftI2C::Start()
{
    /* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
    this->psda->Set();
    this->psck=1;
    this->delay();
    this->psda->Reset();
    this->delay();
    this->psck=0;
    this->delay();
}

void CSoftI2C::Stop()
{
    /* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
    this->psda->Reset();
    this->psck=1;
    this->delay();
    this->psda->Set();
}

void CSoftI2C::WriteByte(byte _ucByte)
{
    byte i;

    /* 先发送字节的高位bit7 */
    for (i = 0; i < 8; i++)
    {
        if (_ucByte &0x80)
        {
            this->psda->Set();
        }
        else
        {
            this->psda->Reset();
        }
        this->delay();
        this->psck=1;
        this->delay();
        this->psck=0;
        if (i == 7)
        {
            this->psda->Set(); // 释放总线
        }
        _ucByte <<= 1; /* 左移一个bit */
        this->delay();
    }
}

byte CSoftI2C::ReadByte()
{
    byte i;
    byte value;

    /* 读到第1个bit为数据的bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        this->psck=1;
        this->delay();
        if (this->psda->Read())
        {
            value++;
        }
        this->psck=0;
        this->delay();
    }
    return value;
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
byte CSoftI2C::WaitAck()
{
    byte re;

    this->psda->Set(); /* CPU释放SDA总线 */
    this->delay();
    this->psck=1; /* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    this->delay();

    if (this->psda->Read())
    /* CPU读取SDA口线状态 */
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    this->psck=0;
    this->delay();
    return re;
}

void CSoftI2C::Ack()
{
    this->psda->Reset(); /* CPU驱动SDA = 0 */
    this->delay();
    this->psck=1; /* CPU产生1个时钟 */
    this->delay();
    this->psck=0;
    this->delay();
    this->psda->Set();
    /*CPU释放SDA总线 */
}

void CSoftI2C::NAck()
{
    this->psda->Set(); /* CPU驱动SDA = 1 */
    this->delay();
    this->psck=1; /* CPU产生1个时钟 */
    this->delay();
    this->psck=0;
    this->delay();
}

void CSoftI2C::delay(void)
{
    byte i;

    /*　
    下面的时间是通过安富莱AX-Pro逻辑分析仪测试得到的。
    CPU主频72MHz时，在内部Flash运行, MDK工程不优化
    循环次数为10时，SCL频率 = 205KHz 
    循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
    循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 

    IAR工程编译效率高，不能设置为7
     */
    for (i = 0; i < 10; i++)
        ;
}
