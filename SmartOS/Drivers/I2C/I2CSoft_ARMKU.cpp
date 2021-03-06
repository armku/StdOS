#include "Kernel\Sys.h"
#include "I2CSoft_ARMKU.h"

//延时-微妙
void I2CBase_ARMKU::Delay(int us) const
{
    Sys.Delay(us);
}
//延时-毫秒
void I2CBase_ARMKU::Sleep(int ms) const
{
    Sys.Sleep(ms);
}

/******************************** SoftI2C ********************************/
// 使用端口和最大速度初始化，因为需要分频，实际速度小于等于该速度
I2CSoft_ARMKU::I2CSoft_ARMKU(uint speedHz)
{
	this->_delay=4;	
}

void I2CSoft_ARMKU::SetPin(Pin scl, Pin sda)
{
	this->SCL.Set(scl);
    this->SDA.Set(sda);

    this->SCL.OpenDrain = false;
    this->SDA.OpenDrain = true;//此处设置错误将导致IIC异常

    this->SCL.Invert = false;
    this->SDA.Invert = false;

    this->SCL.Open();
    this->SDA.Open();
	
	this->SDA = 1;
	this->SCL = 1;
}

void I2CSoft_ARMKU::Start()
{
    /* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
    this->SDA = 1;
    this->Delay(1);
    this->SCL = 1;
    this->Delay(1);
    this->SDA = 0;
    this->Delay(1);
    this->SCL = 0;
    this->Delay(1);
}

void I2CSoft_ARMKU::Stop()
{
    /* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
    this->SDA = 0;
    this->SCL = 1;
    this->Delay(1);
    this->SDA = 1;
}

void I2CSoft_ARMKU::WriteByte(byte dat)
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
        this->Delay(1);
        this->SCL = 1;
        this->Delay(1);
        this->SCL = 0;
        if (i == 7)
        {
            this->SDA = 1; // 释放总线
        }
        dat <<= 1; /* 左移一个bit */
        this->Delay(1);
    }
}

byte I2CSoft_ARMKU::ReadByte()
{
    byte i;
    byte value;

    /* 读到第1个bit为数据的bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        this->SCL = 1;
        this->Delay(1);
        if (this->SDA.ReadInput())
        {
            value++;
        }
        this->SCL = 0;
        this->Delay(1);
    }
    return value;
}
byte I2CSoft_ARMKU::ReadByte(bool ack)
{
	byte ret = this->ReadByte();
	this->Ack(ack);
	return ret;
}
void I2CSoft_ARMKU::Ack(bool ack)
{
    if (ack)
    {
        this->SDA = 0; /* CPU驱动SDA = 0 */
        this->Delay(1);
        this->SCL = 1; /* CPU产生1个时钟 */
        this->Delay(1);
        this->SCL = 0;
        this->Delay(1);
        this->SDA = 1;
        /*CPU释放SDA总线 */
    }
    else
    {
        this->SDA = 1; /* CPU驱动SDA = 1 */
        this->Delay(1);
        this->SCL = 1; /* CPU产生1个时钟 */
        this->Delay(1);
        this->SCL = 0;
        this->Delay(1);
    }
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
bool I2CSoft_ARMKU::WaitAck(int retry)
{
    byte re;

    this->SDA = 1; /* CPU释放SDA总线 */
    this->Delay(1);
    this->SCL = 1; /* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    this->Delay(1);

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
    this->Delay(1);
    return re;
}
