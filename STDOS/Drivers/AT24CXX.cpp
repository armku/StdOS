#include "AT24CXX.h"
#include "stdio.h"
#include "Sys.h"

#define macI2C_WR	0		/* 写控制bit */
#define macI2C_RD	1		/* 读控制bit */


AT24CXX::AT24CXX(){}
AT24CXX::~AT24CXX(){}

void AT24CXX::Init(){

}

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
    this->IIC = new SoftI2C();
	this->IIC->SetPin(pinsck,pinsda);
    this->deviceType = devtype;
    this->Address = devaddr;
    this->pageSize = this->jsPageSize(devtype);
    this->writedelaynms = wnms;
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
        this->IIC->Stop();
        return 0;
    }
}

byte AT24CXX::checkDevice()
{
    byte ucAck;

    this->IIC->Start(); /* 发送启动信号 */
    /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
    this->IIC->WriteByte((this->Address) | macI2C_WR);
    ucAck = this->IIC->WaitAck(); /*检测设备的ACK应答 */

    this->IIC->Stop(); /* 发送停止信号 */

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
int AT24CXX::Read(uint addr, void *pBuffer, int size, ushort bufpos)
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
    this->IIC->Start();

    /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    this->IIC->WriteByte(this->Address | macI2C_WR); /* 此处是写指令 */

    /* 第3步：等待ACK */
    if (this->IIC->WaitAck() != 0)
    {
        goto cmd_Readbytefail; /* EEPROM器件无应答 */
    }
    if (this->deviceType > AT24C16)
    {
        /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
        this->IIC->WriteByte((byte)((address) >> 8));

        /* 第5步：等待ACK */
        if (this->IIC->WaitAck() != 0)
        {
            goto cmd_Readbytefail; /* EEPROM器件无应答 */
        }
    }
    /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
    this->IIC->WriteByte((byte)address);

    /* 第5步：等待ACK */
    if (this->IIC->WaitAck() != 0)
    {
        goto cmd_Readbytefail; /* EEPROM器件无应答 */
    }

    /* 第6步：重新启动I2C总线。前面的代码的目的向EEPROM传送地址，下面开始读取数据 */
    this->IIC->Start();

    /* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    this->IIC->WriteByte(this->Address | macI2C_RD); /* 此处是读指令 */

    /* 第8步：发送ACK */
    if (this->IIC->WaitAck() != 0)
    {
        goto cmd_Readbytefail; /* EEPROM器件无应答 */
    }

    /* 第9步：循环读取数据 */

    ret = this->IIC->ReadByte(); /* 读1个字节 */


    this->IIC->Ack(false); /* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */


    /* 发送I2C总线停止信号 */
    this->IIC->Stop();
    return ret; /* 执行成功 */

    cmd_Readbytefail:  /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    this->IIC->Stop();

    return ret;
}

bool AT24CXX::Write(ushort address, byte da)
{
    uint m;

    /*　第０步：发停止信号，启动内部写操作　*/
    this->IIC->Stop();

    /* 通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms 			
    CLK频率为200KHz时，查询次数为30次左右
     */
    for (m = 0; m < 1000; m++)
    {
        /* 第1步：发起I2C总线启动信号 */
        this->IIC->Start();

        /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
        this->IIC->WriteByte(this->Address | macI2C_WR); /* 此处是写指令 */

        /* 第3步：发送一个时钟，判断器件是否正确应答 */
        if (this->IIC->WaitAck() == 0)
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
        this->IIC->WriteByte((byte)((address >> 8)));

        /* 第5步：等待ACK */
        if (this->IIC->WaitAck() != 0)
        {
            goto cmd_Writebytefail; /* EEPROM器件无应答 */
        }
    }
    /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
    this->IIC->WriteByte((byte)address);

    /* 第5步：等待ACK */
    if (this->IIC->WaitAck() != 0)
    {
        goto cmd_Writebytefail; /* EEPROM器件无应答 */
    }



    /* 第6步：开始写入数据 */
    this->IIC->WriteByte(da);

    /* 第7步：发送ACK */
    if (this->IIC->WaitAck() != 0)
    {
        goto cmd_Writebytefail; /* EEPROM器件无应答 */
    }

    /* 命令执行成功，发送I2C总线停止信号 */
    this->IIC->Stop();
    Sys.Sleep(this->writedelaynms);
    return true;

    cmd_Writebytefail:  /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    this->IIC->Stop();
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
    return 0;
}

int AT24CXX::writePage(byte *buf, ushort bufpos, ushort addr, uint size) //页内写
{
    uint i, m;
    ushort usAddr;

    usAddr = addr;

    /*　第０步：发停止信号，启动内部写操作　*/
    this->IIC->Stop();

    /* 通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms 			
    CLK频率为200KHz时，查询次数为30次左右
     */
    for (m = 0; m < 1000; m++)
    {
        /* 第1步：发起I2C总线启动信号 */
        this->IIC->Start();

        /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
        this->IIC->WriteByte(this->Address | macI2C_WR); /* 此处是写指令 */

        /* 第3步：发送一个时钟，判断器件是否正确应答 */
        if (this->IIC->WaitAck() == 0)
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
        this->IIC->WriteByte((byte)((usAddr >> 8)));

        /* 第5步：等待ACK */
        if (this->IIC->WaitAck() != 0)
        {
            goto cmd_Writefail; /* EEPROM器件无应答 */
        }
    }
    /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
    this->IIC->WriteByte((byte)usAddr);

    /* 第5步：等待ACK */
    if (this->IIC->WaitAck() != 0)
    {
        goto cmd_Writefail; /* EEPROM器件无应答 */
    }

    for (i = 0; i < size; i++)
    {

        /* 第6步：开始写入数据 */
        this->IIC->WriteByte(buf[bufpos + i]);

        /* 第7步：发送ACK */
        if (this->IIC->WaitAck() != 0)
        {
            goto cmd_Writefail; /* EEPROM器件无应答 */
        }

        usAddr++; /* 地址增1 */
    }

    /* 命令执行成功，发送I2C总线停止信号 */
    this->IIC->Stop();
    return 0;

    cmd_Writefail:  /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    this->IIC->Stop();
    return 1;
}

int AT24CXX::readPage(byte *buf, ushort bufpos, ushort addr, uint size) //页内读
{
    uint i;

    /* 第1步：发起I2C总线启动信号 */
    this->IIC->Start();

    /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    this->IIC->WriteByte(this->Address | macI2C_WR); /* 此处是写指令 */

    /* 第3步：等待ACK */
    if (this->IIC->WaitAck() != 0)
    {
        goto cmd_Readfail; /* EEPROM器件无应答 */
    }
    if (this->deviceType > AT24C16)
    {
        /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
        this->IIC->WriteByte((byte)((addr) >> 8));

        /* 第5步：等待ACK */
        if (this->IIC->WaitAck() != 0)
        {
            goto cmd_Readfail; /* EEPROM器件无应答 */
        }
    }
    /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
    this->IIC->WriteByte((byte)addr);

    /* 第5步：等待ACK */
    if (this->IIC->WaitAck() != 0)
    {
        goto cmd_Readfail; /* EEPROM器件无应答 */
    }

    /* 第6步：重新启动I2C总线。前面的代码的目的向EEPROM传送地址，下面开始读取数据 */
    this->IIC->Start();

    /* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    this->IIC->WriteByte(this->Address | macI2C_RD); /* 此处是读指令 */

    /* 第8步：发送ACK */
    if (this->IIC->WaitAck() != 0)
    {
        goto cmd_Readfail; /* EEPROM器件无应答 */
    }

    /* 第9步：循环读取数据 */
    for (i = 0; i < size; i++)
    {
        buf[bufpos + i] = IIC->ReadByte(); /* 读1个字节 */

        /* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
        if (i != size - 1)
        {
            this->IIC->Ack(true); /* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
        }
        else
        {
            this->IIC->Ack(false); /* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
        }
    }
    /* 发送I2C总线停止信号 */
    this->IIC->Stop();
    return 0; /* 执行成功 */

    cmd_Readfail:  /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    this->IIC->Stop();
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

#ifdef DEBUG
    /*
     * 读写测试
     */
    void AT24CXX::Test()
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
#if 1
	#include "stm32f10x.h"
	#define EE_DEV_ADDR			0xA0		/* 24xx02的设备地址 */
#define EE_PAGE_SIZE		8			  /* 24xx02的页面大小 */
#define EE_SIZE				256			  /* 24xx02总容量 */
#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */
/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define GPIO_PORT_I2C	GPIOB			/* GPIO端口 */
#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define I2C_SCL_PIN		GPIO_Pin_6			/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN		GPIO_Pin_7			/* 连接到SDA数据线的GPIO */

/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 0	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
	#define I2C_SCL_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 1 */
	#define I2C_SCL_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 1 */
	#define I2C_SDA_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 0 */
	
	#define I2C_SDA_READ()  GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)	/* 读SDA口线状态 */
#else	/* 这个分支选择直接寄存器操作实现IO读写 */
    /*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
	#define I2C_SCL_1()  GPIO_PORT_I2C->BSRR = I2C_SCL_PIN				/* SCL = 1 */
	#define I2C_SCL_0()  GPIO_PORT_I2C->BRR = I2C_SCL_PIN				/* SCL = 0 */
	
	#define I2C_SDA_1()  GPIO_PORT_I2C->BSRR = I2C_SDA_PIN				/* SDA = 1 */
	#define I2C_SDA_0()  GPIO_PORT_I2C->BRR = I2C_SDA_PIN				/* SDA = 0 */
	
	#define I2C_SDA_READ()  ((GPIO_PORT_I2C->IDR & I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
#endif

static void i2c_CfgGpio(void);

/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过安富莱AX-Pro逻辑分析仪测试得到的。
		CPU主频72MHz时，在内部Flash运行, MDK工程不优化
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
        
    IAR工程编译效率高，不能设置为7
	*/
	for (i = 0; i < 10; i++);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		i2c_Delay();
		I2C_SCL_1();
		i2c_Delay();	
		I2C_SCL_0();
		if (i == 7)
		{
			 I2C_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1();
		i2c_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_0();
		i2c_Delay();
	}
	return value;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	I2C_SDA_1();	/* CPU释放SDA总线 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_0();
	i2c_Delay();
	return re;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	I2C_SDA_0();	/* CPU驱动SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	I2C_SDA_1();	/* CPU驱动SDA = 1 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();	
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CfgGpio
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_CfgGpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_I2C_PORT, ENABLE);	/* 打开GPIO时钟 */

	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* 开漏输出 */
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);

	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	i2c_Stop();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	i2c_CfgGpio();		/* 配置GPIO */

	
	i2c_Start();		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(_Address | I2C_WR);
	ucAck = i2c_WaitAck();	/* 检测设备的ACK应答 */

	i2c_Stop();			/* 发送停止信号 */

	return ucAck;
}
/*
*********************************************************************************************************
*	函 数 名: ee_CheckOk
*	功能说明: 判断串行EERPOM是否正常
*	形    参：无
*	返 回 值: 1 表示正常， 0 表示不正常
*********************************************************************************************************
*/
uint8_t ee_CheckOk(void)
{
	if (i2c_CheckDevice(EE_DEV_ADDR) == 0)
	{
		return 1;
	}
	else
	{
		/* 失败后，切记发送I2C总线停止信号 */
		i2c_Stop();		
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: ee_ReadBytes
*	功能说明: 从串行EEPROM指定地址处开始读取若干数据
*	形    参：_usAddress : 起始地址
*			 _usSize : 数据长度，单位为字节
*			 _pReadBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;
	
	/* 采用串行EEPROM随即读取指令序列，连续读取若干字节 */
	
	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();
	
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(EE_DEV_ADDR | I2C_WR);	/* 此处是写指令 */
	
	/* 第3步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}

	/* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
	i2c_SendByte((uint8_t)_usAddress);
	
	/* 第5步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	
	/* 第6步：重新启动I2C总线。前面的代码的目的向EEPROM传送地址，下面开始读取数据 */
	i2c_Start();
	
	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(EE_DEV_ADDR | I2C_RD);	/* 此处是读指令 */
	
	/* 第8步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}	
	
	/* 第9步：循环读取数据 */
	for (i = 0; i < _usSize; i++)
	{
		_pReadBuf[i] = i2c_ReadByte();	/* 读1个字节 */
		
		/* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
		if (i != _usSize - 1)
		{
			i2c_Ack();	/* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
		}
		else
		{
			i2c_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		}
	}
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 1;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
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
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;
	
	/* 
		写串行EEPROM不像读操作可以连续读取很多字节，每次写操作只能在同一个page。
		对于24xx02，page size = 8
		简单的处理方法为：按字节写操作模式，没写1个字节，都发送地址
		为了提高连续写的效率: 本函数采用page wirte操作。
	*/

	usAddr = _usAddress;	
	for (i = 0; i < _usSize; i++)
	{
		/* 当发送第1个字节或是页面首地址时，需要重新发起启动信号和地址 */
		if ((i == 0) || (usAddr & (EE_PAGE_SIZE - 1)) == 0)
		{
			/*　第０步：发停止信号，启动内部写操作　*/
			i2c_Stop();
			
			/* 通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms 			
				CLK频率为200KHz时，查询次数为30次左右
			*/
			for (m = 0; m < 100; m++)
			{				
				/* 第1步：发起I2C总线启动信号 */
				i2c_Start();
				
				/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
				i2c_SendByte(EE_DEV_ADDR | I2C_WR);	/* 此处是写指令 */
				
				/* 第3步：发送一个时钟，判断器件是否正确应答 */
				if (i2c_WaitAck() == 0)
				{
					break;
				}
			}
			if (m  == 1000)
			{
				goto cmd_fail;	/* EEPROM器件写超时 */
			}
		
			/* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
			i2c_SendByte((uint8_t)usAddr);
			
			/* 第5步：发送ACK */
			if (i2c_WaitAck() != 0)
			{
				goto cmd_fail;	/* EEPROM器件无应答 */
			}
		}
	
		/* 第6步：开始写入数据 */
		i2c_SendByte(_pWriteBuf[i]);
	
		/* 第7步：发送ACK */
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* EEPROM器件无应答 */
		}

		usAddr++;	/* 地址增1 */		
	}
	
	/* 命令执行成功，发送I2C总线停止信号 */
	i2c_Stop();
	return 1;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
}

void ee_Erase(void)
{
	uint16_t i;
	uint8_t buf[EE_SIZE];
	
	/* 填充缓冲区 */
	for (i = 0; i < EE_SIZE; i++)
	{
		buf[i] = 0xFF;
	}
	
	/* 写EEPROM, 起始地址 = 0，数据长度为 256 */
	if (ee_WriteBytes(buf, 0, EE_SIZE) == 0)
	{
		printf("擦除eeprom出错！\r\n");
		return;
	}
	else
	{
		printf("擦除eeprom成功！\r\n");
	}
}


/*--------------------------------------------------------------------------------------------------*/

static void ee_Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
void AT24C02Test()
{
//	AT24CXX at2402(PB6,PB7,AT24C02);
//	i2c_CfgGpio();
//	at2402.Init();	
//	printf("\r\n");
//	at2402.Test();
	uint16_t i;
	uint8_t write_buf[EE_SIZE];
  uint8_t read_buf[EE_SIZE];
  
/*-----------------------------------------------------------------------------------*/  
  if (ee_CheckOk() == 0)
	{
		/* 没有检测到EEPROM */
		printf("没有检测到串行EEPROM!\r\n");
				
		while (1);	/* 停机 */
	}
/*------------------------------------------------------------------------------------*/  
  /* 填充测试缓冲区 */
	for (i = 0; i < EE_SIZE; i++)
	{		
		write_buf[i] = i;
	}
/*------------------------------------------------------------------------------------*/  
  if (ee_WriteBytes(write_buf, 0, EE_SIZE) == 0)
	{
		printf("写eeprom出错！\r\n");
		return;
	}
	else
	{		
		printf("写eeprom成功！\r\n");
	}
  
  /*写完之后需要适当的延时再去读，不然会出错*/
  ee_Delay(0x0FFFFF);
	//Sys.Sleep(999);
/*-----------------------------------------------------------------------------------*/
  if (ee_ReadBytes(read_buf, 0, EE_SIZE) == 0)
	{
		printf("读eeprom出错！\r\n");
		return;
	}
	else
	{		
		printf("读eeprom成功，数据如下：\r\n");
	}
/*-----------------------------------------------------------------------------------*/  
  for (i = 0; i < EE_SIZE; i++)
	{
		if(read_buf[i] != write_buf[i])
		{
			printf("0x%02X ", read_buf[i]);
			printf("错误:EEPROM读出与写入的数据不一致");
			return;
		}
    printf(" %02X", read_buf[i]);
		
		if ((i & 15) == 15)
		{
			printf("\r\n");	
		}		
	}
  printf("eeprom读写测试成功\r\n");
}
#endif
