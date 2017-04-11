#include "stdio.h"
#include "W24xxx.h"
#include "Sys.h"

#define macI2C_WR	0		/* 写控制bit */
#define macI2C_RD	1		/* 读控制bit */

CW24xxx::CW24xxx(Pin pinsck, Pin pinsda, EW24XXType devtype, byte devaddr, uint wnms)
{
    this->pi2c = new CSoftI2C(pinsck, pinsda);
    this->deviceType = devtype;
    this->devAddr = devaddr;
    this->pageSize = this->jsPageSize(devtype);
    this->writedelaynms = wnms;
}

byte CW24xxx::CheckOk()
{
    if (this->checkDevice() == 0)
    {
        return 1;
    }
    else
    {
        /* 失败后，切记发送I2C总线停止信号 */
        this->pi2c->Stop();
        return 0;
    }
}

byte CW24xxx::checkDevice()
{
    byte ucAck;

    this->pi2c->Start(); /* 发送启动信号 */
    /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
    this->pi2c->WriteByte((this->devAddr) | macI2C_WR);
    ucAck = this->pi2c->WaitAck(); /*检测设备的ACK应答 */

    this->pi2c->Stop(); /* 发送停止信号 */

    return ucAck;
}

/*
 *********************************************************************************************************
 *	函 数 名: ee_ReadBytes
 *	功能说明: 从串行EEPROM指定地址处开始读取若干数据
 *	形    参：addr : 起始地址
 *			 size : 数据长度，单位为字节
 *			 pBuffer : 存放读到的数据的缓冲区指针
 *	返 回 值: 1 表示失败，0表示成功
 *********************************************************************************************************
 */
int CW24xxx::Read(uint addr, void *pBuffer, int size, ushort bufpos)
{
    return this->bufwr(addr, (byte*)pBuffer, size, bufpos, 0);
}

/*
 *********************************************************************************************************
 *	函 数 名: ee_WriteBytes
 *	功能说明: 向串行EEPROM指定地址写入若干数据，采用页写操作提高写入效率
 *	形    参：addr : 起始地址
 *			 size : 数据长度，单位为字节
 *			 pBuffer : 存放读到的数据的缓冲区指针
 *	返 回 值: 0 表示失败，1表示成功
 *********************************************************************************************************
 */
int CW24xxx::Write(uint addr, void *pBuffer, int size, ushort bufpos)
{
	int i=0;
	for(i=0;i<size;i++)
	{
		if(this->ReadByte(addr+i)!=((byte*)pBuffer)[bufpos+i])
		{
			break;
		}
	}
	if(i>=size)
	{
		//存储内容相同，不需要写入
		return size;
	}
    return this->bufwr(addr, (byte*)pBuffer, size, bufpos, 1);
}

byte CW24xxx::ReadByte(uint address)
{
    byte ret = 0;


    /* 第1步：发起I2C总线启动信号 */
    this->pi2c->Start();

    /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    this->pi2c->WriteByte(this->devAddr | macI2C_WR); /* 此处是写指令 */

    /* 第3步：等待ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Readbytefail; /* EEPROM器件无应答 */
    }
    if (this->deviceType > AT24C16)
    {
        /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
        this->pi2c->WriteByte((byte)((address) >> 8));

        /* 第5步：等待ACK */
        if (this->pi2c->WaitAck() != 0)
        {
            goto cmd_Readbytefail; /* EEPROM器件无应答 */
        }
    }
    /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
    this->pi2c->WriteByte((byte)address);

    /* 第5步：等待ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Readbytefail; /* EEPROM器件无应答 */
    }

    /* 第6步：重新启动I2C总线。前面的代码的目的向EEPROM传送地址，下面开始读取数据 */
    this->pi2c->Start();

    /* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    this->pi2c->WriteByte(this->devAddr | macI2C_RD); /* 此处是读指令 */

    /* 第8步：发送ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Readbytefail; /* EEPROM器件无应答 */
    }

    /* 第9步：循环读取数据 */

    ret = this->pi2c->ReadByte(); /* 读1个字节 */


    this->pi2c->Ack(false); /* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */


    /* 发送I2C总线停止信号 */
    this->pi2c->Stop();
    return ret; /* 执行成功 */

    cmd_Readbytefail:  /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    this->pi2c->Stop();

    return ret;
}

int CW24xxx::WriteByte(uint address, byte da)
{
    uint m;

    /*　第０步：发停止信号，启动内部写操作　*/
    this->pi2c->Stop();

    /* 通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms 			
    CLK频率为200KHz时，查询次数为30次左右
     */
    for (m = 0; m < 1000; m++)
    {
        /* 第1步：发起I2C总线启动信号 */
        this->pi2c->Start();

        /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
        this->pi2c->WriteByte(this->devAddr | macI2C_WR); /* 此处是写指令 */

        /* 第3步：发送一个时钟，判断器件是否正确应答 */
        if (this->pi2c->WaitAck() == 0)
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
        this->pi2c->WriteByte((byte)((address >> 8)));

        /* 第5步：等待ACK */
        if (this->pi2c->WaitAck() != 0)
        {
            goto cmd_Writebytefail; /* EEPROM器件无应答 */
        }
    }
    /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
    this->pi2c->WriteByte((byte)address);

    /* 第5步：等待ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Writebytefail; /* EEPROM器件无应答 */
    }



    /* 第6步：开始写入数据 */
    this->pi2c->WriteByte(da);

    /* 第7步：发送ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Writebytefail; /* EEPROM器件无应答 */
    }

    /* 命令执行成功，发送I2C总线停止信号 */
    this->pi2c->Stop();
    Sys.Sleep(this->writedelaynms);
    return 0;

    cmd_Writebytefail:  /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    this->pi2c->Stop();
    return 0;
}

int CW24xxx::bufwr(ushort addr, byte *buf, uint size, ushort bufpos, byte wr) //读写集中操作1写 0读
{
    uint curAddr;
    uint pageStart; //页内起始地址
    uint bytesLeave; //还剩多少字节读取
    ushort bufaddr;

    pageStart = addr % this->pageSize;
    bytesLeave = size;
    curAddr = addr;
    bufaddr = bufpos;
    if (this->pinWP)
    {
        *this->pinWP = 0;
    }
    if (pageStart)
    {
        //读取不是页起始地址的内容
        //一次能读完
        if ((pageStart + bytesLeave) < this->pageSize)
        {
            if (wr)
            {
                this->writePage(buf, bufaddr, curAddr, bytesLeave);
            }
            else
            {
                this->readPage(buf, bufaddr, curAddr, bytesLeave);
            }
            bytesLeave -= bytesLeave;
            if (wr)
            {
                Sys.Sleep(this->writedelaynms);
            }
            return 0;
        }
        //一次读取不玩
        else
        {
            if (wr)
            {
                this->writePage(buf, bufaddr, curAddr, this->pageSize - pageStart);
            }
            else
            {
                this->readPage(buf, bufaddr, curAddr, this->pageSize - pageStart);
            }
            bytesLeave -= (this->pageSize - pageStart);
            curAddr += (this->pageSize - pageStart);
            bufaddr += (this->pageSize - pageStart);
            if (wr)
            {
                Sys.Sleep(this->writedelaynms);
            }
        }
    }

    while (bytesLeave > 0)
    {
        if (bytesLeave > this->pageSize)
        {
            if (wr)
            {
                this->writePage(buf, bufaddr, curAddr, this->pageSize);
            }
            else
            {
                this->readPage(buf, bufaddr, curAddr, this->pageSize);
            }
            bytesLeave -= this->pageSize;
            curAddr += this->pageSize;
            bufaddr += this->pageSize;
            if (wr)
            {
                Sys.Sleep(this->writedelaynms);
            }
        }
        else
        {
            if (wr)
            {
                this->writePage(buf, bufaddr, curAddr, bytesLeave);
            }
            else
            {
                this->readPage(buf, bufaddr, curAddr, bytesLeave);
            }

            curAddr += bytesLeave;
            bufaddr += bytesLeave;
            bytesLeave -= bytesLeave;
            if (wr)
            {
                Sys.Sleep(this->writedelaynms);
            }
        }
    }
    if (this->pinWP)
    {
        *this->pinWP = 1;
    }
    return 0;
}

int CW24xxx::writePage(byte *buf, ushort bufpos, ushort addr, uint size) //页内写
{
    uint i, m;
    ushort usAddr;

    usAddr = addr;
	
    /*　第０步：发停止信号，启动内部写操作　*/
    this->pi2c->Stop();

    /* 通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms 			
    CLK频率为200KHz时，查询次数为30次左右
     */
    for (m = 0; m < 1000; m++)
    {
        /* 第1步：发起I2C总线启动信号 */
        this->pi2c->Start();

        /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
        this->pi2c->WriteByte(this->devAddr | macI2C_WR); /* 此处是写指令 */

        /* 第3步：发送一个时钟，判断器件是否正确应答 */
        if (this->pi2c->WaitAck() == 0)
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
        this->pi2c->WriteByte((byte)((usAddr >> 8)));

        /* 第5步：等待ACK */
        if (this->pi2c->WaitAck() != 0)
        {
            goto cmd_Writefail; /* EEPROM器件无应答 */
        }
    }
    /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
    this->pi2c->WriteByte((byte)usAddr);

    /* 第5步：等待ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Writefail; /* EEPROM器件无应答 */
    }

    for (i = 0; i < size; i++)
    {

        /* 第6步：开始写入数据 */
        this->pi2c->WriteByte(buf[bufpos + i]);

        /* 第7步：发送ACK */
        if (this->pi2c->WaitAck() != 0)
        {
            goto cmd_Writefail; /* EEPROM器件无应答 */
        }

        usAddr++; /* 地址增1 */
    }

    /* 命令执行成功，发送I2C总线停止信号 */
    this->pi2c->Stop();
    return 0;

    cmd_Writefail:  /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    this->pi2c->Stop();
    return 1;
}

int CW24xxx::readPage(byte *buf, ushort bufpos, ushort addr, uint size) //页内读
{
    uint i;

    /* 第1步：发起I2C总线启动信号 */
    this->pi2c->Start();

    /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    this->pi2c->WriteByte(this->devAddr | macI2C_WR); /* 此处是写指令 */

    /* 第3步：等待ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Readfail; /* EEPROM器件无应答 */
    }
    if (this->deviceType > AT24C16)
    {
        /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
        this->pi2c->WriteByte((byte)((addr) >> 8));

        /* 第5步：等待ACK */
        if (this->pi2c->WaitAck() != 0)
        {
            goto cmd_Readfail; /* EEPROM器件无应答 */
        }
    }
    /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
    this->pi2c->WriteByte((byte)addr);

    /* 第5步：等待ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Readfail; /* EEPROM器件无应答 */
    }

    /* 第6步：重新启动I2C总线。前面的代码的目的向EEPROM传送地址，下面开始读取数据 */
    this->pi2c->Start();

    /* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    this->pi2c->WriteByte(this->devAddr | macI2C_RD); /* 此处是读指令 */

    /* 第8步：发送ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Readfail; /* EEPROM器件无应答 */
    }

    /* 第9步：循环读取数据 */
    for (i = 0; i < size; i++)
    {
        buf[bufpos + i] = pi2c->ReadByte(); /* 读1个字节 */

        /* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
        if (i != size - 1)
        {
            this->pi2c->Ack(true); /* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
        }
        else
        {
            this->pi2c->Ack(false); /* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
        }
    }
    /* 发送I2C总线停止信号 */
    this->pi2c->Stop();
    return 0; /* 执行成功 */

    cmd_Readfail:  /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    this->pi2c->Stop();
    return 1;
}

ushort CW24xxx::jsPageSize(uint type) //计算存储页大小
{
    ushort ret = 8;
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

#ifdef DEBUG
    /*
     * 读写测试
     */
    void CW24xxx::Test()
    {
        uint i;
        const uint testsize = 8;
        const uint testaddress = 0;
        byte write_buf[testsize + testaddress];
        byte read_buf[testsize + testaddress];

        /*-----------------------------------------------------------------------------------*/
        if (this->CheckOk() == 0)
        {
            /* 没有检测到EEPROM */
            printf("没有检测到串行EEPROM!\r\n");

            return ;
        }
        /*------------------------------------------------------------------------------------*/
        /* 填充测试缓冲区 */
        for (i = 0; i < testsize; i++)
        {
            write_buf[i] = i;
            read_buf[i] = 0;
        }
        printf("原始数据\r\n");
        for (i = 0; i < testsize; i++)
        {
            printf("%02X ", write_buf[i]);

            if ((i &15) == 15)
            {
                printf("\r\n");
            }
        }
        printf("\r\n");
        /*------------------------------------------------------------------------------------*/
        if (this->Write(testaddress, write_buf, testsize, 0))
        {
            printf("写eeprom出错！\r\n");
            return ;
        }
        else
        {
            printf("写eeprom成功！\r\n");
        }
        /*写完之后需要适当的延时再去读，不然会出错*/

        /*-----------------------------------------------------------------------------------*/
        if (this->Read(testaddress, read_buf, testsize, 0))
        {
            printf("读eeprom出错！\r\n");
            return ;
        }
        else
        {
            printf("读eeprom成功，数据如下：\r\n");
        }

        /*-----------------------------------------------------------------------------------*/
        for (i = 0; i < testsize; i++)
        {
            if (read_buf[i] != write_buf[i])
            {
                //printf("0X%02X ", read_buf[i]);
                //printf("错误:EEPROM读出与写入的数据不一致");
                //return ;
            }
            printf("%02X ", read_buf[i]);

            if ((i &15) == 15)
            {
                printf("\r\n");
            }
        }
        printf("eeprom读写测试成功\r\n");
    }
#endif
