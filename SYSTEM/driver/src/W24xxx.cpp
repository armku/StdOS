#include "stdio.h"
#include "W24xxx.h"
#include "delay.h"

#define macI2C_WR	0		/* 写控制bit */
#define macI2C_RD	1		/* 读控制bit */

CW24xxx::CW24xxx(PinPort pinsck, PinPort pinsda, EW24XXType devtype, uint8_t devaddr, uint32_t wnms)
{
    this->pi2c = new CI2CSoft(pinsck, pinsda);
    this->deviceType = devtype;
    this->devAddr = devaddr;
    this->pageSize = this->jsPageSize(devtype);
    this->writedelaynms = wnms;
}

CW24xxx::~CW24xxx(){

}
uint8_t CW24xxx::CheckOk()
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

uint8_t CW24xxx::checkDevice()
{
    uint8_t ucAck;

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
 *	形    参：_usAddress : 起始地址
 *			 _usSize : 数据长度，单位为字节
 *			 _pReadBuf : 存放读到的数据的缓冲区指针
 *	返 回 值: 1 表示失败，0表示成功
 *********************************************************************************************************
 */
uint8_t CW24xxx::ReadBytes(uint8_t *_pReadBuf, uint16_t bufpos, uint16_t _usAddress, uint32_t _usSize)
{
    return this->bufwr(_pReadBuf, bufpos, _usAddress, _usSize, 0);
}

/*
 *********************************************************************************************************
 *	函 数 名: ee_WriteBytes
 *	功能说明: 向串行EEPROM指定地址写入若干数据，采用页写操作提高写入效率
 *	形    参：_usAddress : 起始地址
 *			 _usSize : 数据长度，单位为字节
 *			 _pWriteBuf : 存放读到的数据的缓冲区指针
 *	返 回 值: 0 表示失败，1表示成功
 *********************************************************************************************************
 */
uint8_t CW24xxx::WriteBytes(uint8_t *_pWriteBuf, uint16_t bufpos, uint16_t _usAddress, uint32_t _usSize)
{
    return this->bufwr(_pWriteBuf, bufpos, _usAddress, _usSize, 1);
}

uint8_t CW24xxx::ReadByte(uint16_t address)
{
    uint8_t ret = 0;


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
        this->pi2c->WriteByte((uint8_t)((address) >> 8));

        /* 第5步：等待ACK */
        if (this->pi2c->WaitAck() != 0)
        {
            goto cmd_Readbytefail; /* EEPROM器件无应答 */
        }
    }
    /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
    this->pi2c->WriteByte((uint8_t)address);

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


    this->pi2c->NAck(); /* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */


    /* 发送I2C总线停止信号 */
    this->pi2c->Stop();
    return ret; /* 执行成功 */

    cmd_Readbytefail:  /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    this->pi2c->Stop();

    return ret;
}

uint8_t CW24xxx::WriteByte(uint16_t address, uint8_t da)
{
    uint32_t m;

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
        this->pi2c->WriteByte((uint8_t)((address >> 8)));

        /* 第5步：等待ACK */
        if (this->pi2c->WaitAck() != 0)
        {
            goto cmd_Writebytefail; /* EEPROM器件无应答 */
        }
    }
    /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
    this->pi2c->WriteByte((uint8_t)address);

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
    delay_ms(this->writedelaynms);
    return 0;

    cmd_Writebytefail:  /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    this->pi2c->Stop();
    return 0;
}

uint8_t CW24xxx::bufwr(uint8_t *buf, uint16_t bufpos, uint16_t addr, uint32_t len, uint8_t wr) //读写集中操作1写 0读
{
    uint32_t curAddr;
    uint32_t pageStart; //页内起始地址
    uint32_t bytesLeave; //还剩多少字节读取
    uint16_t bufaddr;

    pageStart = addr % this->pageSize;
    bytesLeave = len;
    curAddr = addr;
    bufaddr = bufpos;

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
                delay_ms(this->writedelaynms);
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
                delay_ms(this->writedelaynms);
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
                delay_ms(this->writedelaynms);
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
                delay_ms(this->writedelaynms);
            }
        }
    }
    return 0;
}

uint8_t CW24xxx::writePage(uint8_t *buf, uint16_t bufpos, uint16_t addr, uint32_t len) //页内写
{
    uint32_t i, m;
    uint16_t usAddr;

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
        this->pi2c->WriteByte((uint8_t)((usAddr >> 8)));

        /* 第5步：等待ACK */
        if (this->pi2c->WaitAck() != 0)
        {
            goto cmd_Writefail; /* EEPROM器件无应答 */
        }
    }
    /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
    this->pi2c->WriteByte((uint8_t)usAddr);

    /* 第5步：等待ACK */
    if (this->pi2c->WaitAck() != 0)
    {
        goto cmd_Writefail; /* EEPROM器件无应答 */
    }

    for (i = 0; i < len; i++)
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

uint8_t CW24xxx::readPage(uint8_t *buf, uint16_t bufpos, uint16_t addr, uint32_t len) //页内读
{
    uint32_t i;

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
        this->pi2c->WriteByte((uint8_t)((addr) >> 8));

        /* 第5步：等待ACK */
        if (this->pi2c->WaitAck() != 0)
        {
            goto cmd_Readfail; /* EEPROM器件无应答 */
        }
    }
    /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
    this->pi2c->WriteByte((uint8_t)addr);

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
    for (i = 0; i < len; i++)
    {
        buf[bufpos + i] = pi2c->ReadByte(); /* 读1个字节 */

        /* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
        if (i != len - 1)
        {
            this->pi2c->Ack(); /* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
        }
        else
        {
            this->pi2c->NAck(); /* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
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

uint16_t CW24xxx::jsPageSize(uint32_t type) //计算存储页大小
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

#if EEPTEST
    /*
     * 读写测试
     */
    void CW24xxx::Test(void)
    {
        uint32_t i;
        const uint32_t testsize = 8;
        const uint32_t testaddress = 0;
        uint8_t write_buf[testsize + testaddress];
        uint8_t read_buf[testsize + testaddress];

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
        if (this->WriteBytes(write_buf, 0, testaddress, testsize))
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
        if (this->ReadBytes(read_buf, 0, testaddress, testsize))
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
