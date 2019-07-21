#include "I2C.h"
#include "Sys.h"
// 使用端口和最大速度初始化，因为需要分频，实际速度小于等于该速度
I2CSoft::I2CSoft(uint speedHz)
{
	this->_delay=4;	
}

void I2CSoft::SetPin(Pin scl, Pin sda)
{
	this->SCL.SetPin(scl);
    this->SDA.SetPin(sda);

	this->SCL.pinMode(GPIO_Out_PP);
	this->SDA.pinMode(GPIO_Out_OD);//此处设置错误将导致IIC异常
	
	this->SDA = 1;
	this->SCL = 1;
}

void I2CSoft::Start()
{
    /* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
    this->SDA = 1;
	delayMicroseconds(1);
    this->SCL = 1;
	delayMicroseconds(1);
    this->SDA = 0;
    delayMicroseconds(1);
    this->SCL = 0;
    delayMicroseconds(1);
}

void I2CSoft::Stop()
{
    /* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
    this->SDA = 0;
    this->SCL = 1;
    delayMicroseconds(1);
    this->SDA = 1;
}

void I2CSoft::WriteByte(byte dat)
{
    byte i;

    /* 先发送字节的高位bit7 */
    for (i = 0; i < 8; i++)
    {
        if (dat &0x80)
        {
            this->SDA = 1;
        }
        else
        {
            this->SDA = 0;
        }
        delayMicroseconds(1);
        this->SCL = 1;
        delayMicroseconds(1);
        this->SCL = 0;
        if (i == 7)
        {
            this->SDA = 1; // 释放总线
        }
        dat <<= 1; /* 左移一个bit */
        delayMicroseconds(1);
    }
}

byte I2CSoft::ReadByte()
{
    byte i;
    byte value;

    /* 读到第1个bit为数据的bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        this->SCL = 1;
        delayMicroseconds(1);
        if (this->SDA.ReadInput())
        {
            value++;
        }
        this->SCL = 0;
        delayMicroseconds(1);
    }
    return value;
}
byte I2CSoft::ReadByte(bool ack)
{
	byte ret = this->ReadByte();
	this->Ack(ack);
	return ret;
}
void I2CSoft::Ack(bool ack)
{
    if (ack)
    {
        this->SDA = 0; /* CPU驱动SDA = 0 */
        delayMicroseconds(1);
        this->SCL = 1; /* CPU产生1个时钟 */
        delayMicroseconds(1);
        this->SCL = 0;
        delayMicroseconds(1);
        this->SDA = 1;
        /*CPU释放SDA总线 */
    }
    else
    {
        this->SDA = 1; /* CPU驱动SDA = 1 */
        delayMicroseconds(1);
        this->SCL = 1; /* CPU产生1个时钟 */
        delayMicroseconds(1);
        this->SCL = 0;
        delayMicroseconds(1);
    }
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
bool I2CSoft::WaitAck(int retry)
{
    byte re;

    this->SDA = 1; /* CPU释放SDA总线 */
    delayMicroseconds(1);
    this->SCL = 1; /* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    delayMicroseconds(1);

    if (this->SDA.ReadInput())
    /* CPU读取SDA口线状态 */
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    this->SCL = 0;
    delayMicroseconds(1);
    return re;
}
