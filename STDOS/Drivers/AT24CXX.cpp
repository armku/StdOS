#include "AT24CXX.h"

#define macI2C_WR	0		/* 写控制bit */
#define macI2C_RD	1		/* 读控制bit */


AT24CXX::AT24CXX(){}
AT24CXX::~AT24CXX(){}

bool AT24CXX::Write(uint addr, const Buffer &bs)const
{
    return false;
}

bool AT24CXX::Read(uint addr, Buffer &bs)const
{
    return false;
}

AT24CXX::AT24CXX(Pin pinsck, Pin pinsda, EW24XXType devtype, byte devaddr, uint wnms)
{
    this->IIC.SetPin(pinsck, pinsda);
    this->deviceType = devtype;
    this->Address = devaddr;
    this->pageSize = this->jsPageSize(devtype);
    this->writedelaynms = wnms;
}

/*
 *********************************************************************************************************
 *	函 数 名: Read
 *	功能说明: 从串行EEPROM指定地址处开始读取若干数据
 *	形    参：addr : 起始地址
 *			 size : 数据长度，单位为字节
 *			 pBuffer : 存放读到的数据的缓冲区指针
 *	返 回 值: 1 表示失败，0表示成功
 *********************************************************************************************************
 */
int AT24CXX::Read(uint addr, void *pBuffer, int size, ushort bufpos)
{
    return this->bufwr(addr, (byte*)pBuffer, size, bufpos, 0);
}

/*
 *********************************************************************************************************
 *	函 数 名: Write
 *	功能说明: 向串行EEPROM指定地址写入若干数据，采用页写操作提高写入效率
 *	形    参：addr : 起始地址
 *			 size : 数据长度，单位为字节
 *			 pBuffer : 存放读到的数据的缓冲区指针
 *	返 回 值: 0 表示失败，1表示成功
 *********************************************************************************************************
 */
int AT24CXX::Write(uint addr, void *pBuffer, int size, ushort bufpos)
{
    int i = 0;
    for (i = 0; i < size; i++)
    {
        if (this->Read(addr + i) != ((byte*)pBuffer)[bufpos + i])
        {
            break;
        }
    }
    if (i >= size)
    {
        //存储内容相同，不需要写入
        return size;
    }
    return this->bufwr(addr, (byte*)pBuffer, size, bufpos, 1);
}

byte AT24CXX::Read(ushort address)
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
    if (this->deviceType > AT24C16)
    {
        /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
        this->IIC.WriteByte((byte)((address) >> 8));

        /* 第5步：等待ACK */
        if (this->IIC.WaitAck() != 0)
        {
            goto cmd_Readbytefail; /* EEPROM器件无应答 */
        }
    }
    /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
    this->IIC.WriteByte((byte)address);

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

    ret = this->IIC.ReadByte(); /* 读1个字节 */


    this->IIC.Ack(false); /* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */


    /* 发送I2C总线停止信号 */
    this->IIC.Stop();
    return ret; /* 执行成功 */

    cmd_Readbytefail:  /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    this->IIC.Stop();

    return ret;
}

bool AT24CXX::Write(ushort address, byte da)
{
    uint m;

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
        this->IIC.WriteByte((byte)((address >> 8)));

        /* 第5步：等待ACK */
        if (this->IIC.WaitAck() != 0)
        {
            goto cmd_Writebytefail; /* EEPROM器件无应答 */
        }
    }
    /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
    this->IIC.WriteByte((byte)address);

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

int AT24CXX::bufwr(ushort addr, byte *buf, uint size, ushort bufpos, byte wr) //读写集中操作1写 0读
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
    return size;
}

int AT24CXX::writePage(byte *buf, ushort bufpos, ushort addr, uint size) //页内写
{
    uint i, m;
    ushort usAddr;

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
        this->IIC.WriteByte((byte)((usAddr >> 8)));

        /* 第5步：等待ACK */
        if (this->IIC.WaitAck() != 0)
        {
            goto cmd_Writefail; /* EEPROM器件无应答 */
        }
    }
    /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
    this->IIC.WriteByte((byte)usAddr);

    /* 第5步：等待ACK */
    if (this->IIC.WaitAck() != 0)
    {
        goto cmd_Writefail; /* EEPROM器件无应答 */
    }

    for (i = 0; i < size; i++)
    {

        /* 第6步：开始写入数据 */
        this->IIC.WriteByte(buf[bufpos + i]);

        /* 第7步：发送ACK */
        if (this->IIC.WaitAck() != 0)
        {
            goto cmd_Writefail; /* EEPROM器件无应答 */
        }

        usAddr++; /* 地址增1 */
    }

    /* 命令执行成功，发送I2C总线停止信号 */
    this->IIC.Stop();
    return 0;

    cmd_Writefail:  /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    this->IIC.Stop();
    return 1;
}

int AT24CXX::readPage(byte *buf, ushort bufpos, ushort addr, uint size) //页内读
{
    uint i;

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
        this->IIC.WriteByte((byte)((addr) >> 8));

        /* 第5步：等待ACK */
        if (this->IIC.WaitAck() != 0)
        {
            goto cmd_Readfail; /* EEPROM器件无应答 */
        }
    }
    /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
    this->IIC.WriteByte((byte)addr);

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
    for (i = 0; i < size; i++)
    {
        buf[bufpos + i] = this->IIC.ReadByte(); /* 读1个字节 */

        /* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
        if (i != size - 1)
        {
            this->IIC.Ack(true); /* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
        }
        else
        {
            this->IIC.Ack(false); /* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
        }
    }
    /* 发送I2C总线停止信号 */
    this->IIC.Stop();
    return 0; /* 执行成功 */

    cmd_Readfail:  /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    this->IIC.Stop();
    return 1;
}

ushort AT24CXX::jsPageSize(uint type) //计算存储页大小
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

byte AT24CXX::checkDevice()
{
    byte ucAck;

    this->IIC.Start(); /* 发送启动信号 */
    /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
    this->IIC.WriteByte((this->Address) | macI2C_WR);
    ucAck = this->IIC.WaitAck(); /*检测设备的ACK应答 */

    this->IIC.Stop(); /* 发送停止信号 */

    return ucAck;
}

byte AT24CXX::CheckOk()
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

#if 0
    #define EE_SIZE				256			  /* 24xx02总容量 */    
       
    void AT24C02Test()
    {
		AT24CXX at2402(PB6, PB7, AT24C02);
        printf("\r\n");

        ushort i;
        byte write_buf[EE_SIZE];
        byte read_buf[EE_SIZE];

        /*-----------------------------------------------------------------------------------*/
        if (at2402.CheckOk() == 0)
        {
            /* 没有检测到EEPROM */
            printf("没有检测到串行EEPROM!\r\n");

            while (1)
                ;
            /* 停机 */
        }
        /*------------------------------------------------------------------------------------*/
        /* 填充测试缓冲区 */
        for (i = 0; i < EE_SIZE; i++)
        {
            write_buf[i] = i;
        }
        /*------------------------------------------------------------------------------------*/
        if (at2402.Write(0,write_buf, EE_SIZE) == 0)
        {
            printf("写eeprom出错！\r\n");
            return ;
        }
        else
        {
            printf("写eeprom成功！\r\n");
        }

        /*写完之后需要适当的延时再去读，不然会出错*/
        Sys.Sleep(10);
        /*-----------------------------------------------------------------------------------*/
        if (at2402.Read(0,read_buf, EE_SIZE) == 0)
        {
            printf("读eeprom出错！\r\n");
            return ;
        }
        else
        {
            printf("读eeprom成功，数据如下：\r\n");
        }
        /*-----------------------------------------------------------------------------------*/
        for (i = 0; i < EE_SIZE; i++)
        {
            if (read_buf[i] != write_buf[i])
            {
                printf("0x%02X ", read_buf[i]);
                printf("错误:EEPROM读出与写入的数据不一致");
                return ;
            }
            printf(" %02X", read_buf[i]);

            if ((i &15) == 15)
            {
                printf("\r\n");
            }
        }
        printf("eeprom读写测试成功\r\n");
    }
#endif
