#include "PCF8563.h"
#define macI2C_WR	0		/* 写控制bit */
#define macI2C_RD	1		/* 读控制bit */
PCF8563::PCF8563()
{
	this->Address = 0XA0;
}

PCF8563::~PCF8563()
{
}

void PCF8563::SetDateTime(DateTime & dt)
{

}
void PCF8563::GetDateTime(DateTime& dt)
{
	
}
byte PCF8563::readaddr(byte add)
{
	byte ret = 0;
	/* 第1步：发起I2C总线启动信号 */
	this->IIC.Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	this->IIC.WriteByte(this->Address | macI2C_WR); /* 此处是写指令 */

													/* 第3步：等待ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Readbytefail; /* EEPROM器件无应答 */
	}
	
	/* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
	this->IIC.WriteByte((byte)this->Address);

	/* 第5步：等待ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Readbytefail; /* EEPROM器件无应答 */
	}

	/* 第6步：重新启动I2C总线。前面的代码的目的向EEPROM传送地址，下面开始读取数据 */
	this->IIC.Start();

	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	this->IIC.WriteByte(this->Address | macI2C_RD); /* 此处是读指令 */

													/* 第8步：发送ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Readbytefail; /* EEPROM器件无应答 */
	}

	/* 第9步：循环读取数据 */

	ret = this->IIC.ReadByte(false); /* 读1个字节 */
	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */

	/* 发送I2C总线停止信号 */
	this->IIC.Stop();
	return ret; /* 执行成功 */

cmd_Readbytefail:  /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
				   /* 发送I2C总线停止信号 */
	this->IIC.Stop();

	return ret;
}


