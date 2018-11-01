#include "AT24CXX.h"

#define macI2C_WR	0		/* 写控制bit */
#define macI2C_RD	1		/* 读控制bit */

bool AT24CXX::Write(uint32_t addr, void * buf, int len, int bufpos)
{
	uint32_t curAddr;
	uint32_t pageStart; //页内起始地址
	uint32_t bytesLeave; //还剩多少字节读取
	uint16_t bufaddr;

	pageStart = addr % this->Block;
	bytesLeave = len;
	curAddr = addr;
	bufaddr = 0;
	
	this->pinWP = 0;
	if (pageStart)
	{
		//读取不是页起始地址的内容		
		if ((pageStart + bytesLeave) < this->Block)
		{
			//一次能读完
			this->PageWrite(curAddr, buf, bytesLeave);
			return true;
		}
		else
		{
			//一次读取不完成
			this->PageWrite(curAddr, buf, this->Block - pageStart);
			bytesLeave -= (this->Block - pageStart);
			curAddr += (this->Block - pageStart);
			bufaddr += (this->Block - pageStart);
		}
	}

	while (bytesLeave > 0)
	{
		if (bytesLeave > this->Block)
		{
			//读取整页
			this->PageWrite(curAddr, ((uint8_t*)buf) + bufaddr, this->Block);
			bytesLeave -= this->Block;
			curAddr += this->Block;
			bufaddr += this->Block;
		}
		else
		{
			//读取剩余页
			this->PageWrite(curAddr, ((uint8_t*)buf) + bufaddr, bytesLeave);
			return true;
		}
	}
	this->pinWP = 1;
	return false;
}

bool AT24CXX::Read(uint32_t addr, void * buf, int len, int bufpos)
{
	uint32_t curAddr;
	uint32_t pageStart; //页内起始地址
	uint32_t bytesLeave; //还剩多少字节读取
	uint16_t bufaddr;

	pageStart = addr % this->Block;
	bytesLeave = len;
	curAddr = addr;
	bufaddr = 0;
	
	if (pageStart)
	{
		//读取不是页起始地址的内容		
		if ((pageStart + bytesLeave) < this->Block)
		{
			//一次能读完
			this->PageRead(curAddr, buf, bytesLeave);
			return true;
		}		
		else
		{
			//一次读取不完成
			this->PageRead(curAddr, buf, this->Block - pageStart);
			bytesLeave -= (this->Block - pageStart);
			curAddr += (this->Block - pageStart);
			bufaddr += (this->Block - pageStart);
		}
	}

	while (bytesLeave > 0)
	{
		if (bytesLeave > this->Block)
		{
			//读取整页
			this->PageRead(curAddr, ((uint8_t*)buf) + bufaddr, this->Block);
			bytesLeave -= this->Block;
			curAddr += this->Block;
			bufaddr += this->Block;
		}
		else
		{
			//读取剩余页
			this->PageRead(curAddr, ((uint8_t*)buf) + bufaddr, bytesLeave);
			return true;
		}
	}
	return false;
}
//写延时时间
AT24CXX::AT24CXX(EW24XXType devtype, uint8_t devaddr, uint32_t wnms)
{
	this->deviceType = devtype;
	this->Address = devaddr;
	this->Block = this->jsPageSize(devtype);
	this->writedelaynms = wnms;
}
void AT24CXX::SetPin(Pin pinscl, Pin pinsda, Pin pinwriteprotect)
{
	this->IIC.SetPin(pinscl, pinsda);
	if (pinwriteprotect != P0)
	{
		this->pinWP.Set(pinwriteprotect);
		this->pinWP.OpenDrain = false;
		this->pinWP.Invert = false;
		this->pinWP.Open();
		this->pinWP = 1;
	}
}

uint8_t AT24CXX::Read(uint16_t address)
{
	uint8_t ret = 0;
	
	/* 第1步：发起I2C总线启动信号 */
	this->IIC.Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	this->IIC.WriteByte(this->Address | macI2C_WR); /* 此处是写指令 */

	/* 第3步：等待ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Readbytefail; /* EEPROM器件无应答 */
	}
	if (this->deviceType > AT24C16)
	{
		/* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
		this->IIC.WriteByte((uint8_t)((address) >> 8));

		/* 第5步：等待ACK */
		if (this->IIC.WaitAck() != 0)
		{
			goto cmd_Readbytefail; /* EEPROM器件无应答 */
		}
	}
	/* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
	this->IIC.WriteByte((uint8_t)address);

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

bool AT24CXX::Write(uint16_t address, uint8_t da)
{
	uint32_t m;

	/*　第０步：发停止信号，启动内部写操作　*/
	this->IIC.Stop();

	/* 通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms
	CLK频率为200KHz时，查询次数为30次左右
	 */
	for (m = 0; m < 1000; m++)
	{
		/* 第1步：发起I2C总线启动信号 */
		this->IIC.Start();

		/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
		this->IIC.WriteByte(this->Address | macI2C_WR); /* 此处是写指令 */

		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (this->IIC.WaitAck() == 0)
		{
			break;
		}
	}
	if (m == 1000)
	{
		goto cmd_Writebytefail; /* EEPROM器件写超时 */
	}
	if (this->deviceType > AT24C16)
	{
		/* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
		this->IIC.WriteByte((uint8_t)((address >> 8)));

		/* 第5步：等待ACK */
		if (this->IIC.WaitAck() != 0)
		{
			goto cmd_Writebytefail; /* EEPROM器件无应答 */
		}
	}
	/* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
	this->IIC.WriteByte((uint8_t)address);

	/* 第5步：等待ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Writebytefail; /* EEPROM器件无应答 */
	}
	
	/* 第6步：开始写入数据 */
	this->IIC.WriteByte(da);

	/* 第7步：发送ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Writebytefail; /* EEPROM器件无应答 */
	}

	/* 命令执行成功，发送I2C总线停止信号 */
	this->IIC.Stop();
	Sys.Sleep(this->writedelaynms);
	return true;

cmd_Writebytefail:  /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
/* 发送I2C总线停止信号 */
	this->IIC.Stop();
	return true;
}

//页内读，最多一页
int AT24CXX::PageRead(uint16_t addr, void * buf, int len)
{
	if (len > this->Block)
		return 1;

	/* 第1步：发起I2C总线启动信号 */
	this->IIC.Start();

	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	this->IIC.WriteByte(this->Address | macI2C_WR); /* 此处是写指令 */

													/* 第3步：等待ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Readfail; /* EEPROM器件无应答 */
	}
	if (this->deviceType > AT24C16)
	{
		/* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
		this->IIC.WriteByte((uint8_t)((addr) >> 8));

		/* 第5步：等待ACK */
		if (this->IIC.WaitAck() != 0)
		{
			goto cmd_Readfail; /* EEPROM器件无应答 */
		}
	}
	/* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
	this->IIC.WriteByte((uint8_t)addr);

	/* 第5步：等待ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Readfail; /* EEPROM器件无应答 */
	}

	/* 第6步：重新启动I2C总线。前面的代码的目的向EEPROM传送地址，下面开始读取数据 */
	this->IIC.Start();

	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	this->IIC.WriteByte(this->Address | macI2C_RD); /* 此处是读指令 */

													/* 第8步：发送ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Readfail; /* EEPROM器件无应答 */
	}

	/* 第9步：循环读取数据 */
	for (int i = 0; i < len; i++)
	{
		((uint8_t*)buf)[i] = this->IIC.ReadByte((i != len - 1) ? true : false); /* 读1个字节 */
		/* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */		
	}
	/* 发送I2C总线停止信号 */
	this->IIC.Stop();
	return 0; /* 执行成功 */

cmd_Readfail:  /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
			   /* 发送I2C总线停止信号 */
	this->IIC.Stop();
	return 1;
}
//页内写，最多一页
int AT24CXX::PageWrite(uint16_t addr, void * buf, int len)
{
	if (len > this->Block)
		return 1;
	uint32_t m;
	uint16_t usAddr;
	uint8_t buftmp[256];

	//非法长度，直接返回
	if (len > 256)
		return 0;
	this->PageRead(addr, buftmp, len);
	bool flagchgs = false;
	for (int i = 0; i < len; i++)
	{
		if (((uint8_t*)buf)[i] != buftmp[i])
		{
			flagchgs = true;
			break;
		}
	}
	//存储值与之前相等，不需要更新
	if (!flagchgs)
		return 0;

	usAddr = addr;
	/*　第０步：发停止信号，启动内部写操作　*/
	this->IIC.Stop();

	/* 通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms
	CLK频率为200KHz时，查询次数为30次左右
	*/
	for (m = 0; m < 1000; m++)
	{
		/* 第1步：发起I2C总线启动信号 */
		this->IIC.Start();

		/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
		this->IIC.WriteByte(this->Address | macI2C_WR); /* 此处是写指令 */

														/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (this->IIC.WaitAck() == 0)
		{
			break;
		}
	}
	if (m == 1000)
	{
		goto cmd_Writefail; /* EEPROM器件写超时 */
	}
	if (this->deviceType > AT24C16)
	{
		/* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
		this->IIC.WriteByte((uint8_t)((usAddr >> 8)));

		/* 第5步：等待ACK */
		if (this->IIC.WaitAck() != 0)
		{
			goto cmd_Writefail; /* EEPROM器件无应答 */
		}
	}
	/* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
	this->IIC.WriteByte((uint8_t)usAddr);

	/* 第5步：等待ACK */
	if (this->IIC.WaitAck() != 0)
	{
		goto cmd_Writefail; /* EEPROM器件无应答 */
	}

	for (int i = 0; i < len; i++)
	{

		/* 第6步：开始写入数据 */
		this->IIC.WriteByte(((uint8_t*)buf)[i]);

		/* 第7步：发送ACK */
		if (this->IIC.WaitAck() != 0)
		{
			goto cmd_Writefail; /* EEPROM器件无应答 */
		}

		usAddr++; /* 地址增1 */
	}

	/* 命令执行成功，发送I2C总线停止信号 */
	this->IIC.Stop();
	Sys.Sleep(this->writedelaynms);
	return 0;

cmd_Writefail:  /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
				/* 发送I2C总线停止信号 */
	this->IIC.Stop();
	return 1;
}

uint16_t AT24CXX::jsPageSize(uint32_t type) //计算存储页大小
{
	uint16_t ret = 8;
	switch (type)
	{
	case AT24C02:
	case AT24C04:
		ret = 8;
		break;
	case AT24C08:
	case AT24C16:
		ret = 16;
		break;
	case AT24C32:
	case AT24C64:
		ret = 32;
		break;
	case AT24C128:
	case AT24C256:
		ret = 64;
		break;
	case AT24C512:
	case AT24C1024:
		ret = 128;
		break;
	case AT24C2048:
		ret = 256;
		break;
	default:
		break;
	}
	return ret;
}

uint8_t AT24CXX::checkDevice()
{
	uint8_t ucAck;

	this->IIC.Start(); /* 发送启动信号 */
	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	this->IIC.WriteByte((this->Address) | macI2C_WR);
	ucAck = this->IIC.WaitAck(); /*检测设备的ACK应答 */

	this->IIC.Stop(); /* 发送停止信号 */

	return ucAck;
}

uint8_t AT24CXX::CheckOk()
{
	if (this->checkDevice() == 0)
	{
		return 1;
	}
	else
	{
		/* 失败后，切记发送I2C总线停止信号 */
		this->IIC.Stop();
		return 0;
	}
}
