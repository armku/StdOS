#include "W25QXXX.h"

/* Private define ------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////// 
//指令表
#define W25X_WriteEnable			0x06 
#define W25X_WriteDisable		    0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			    0x03 
#define W25X_FastReadData		    0x0B 
#define W25X_FastReadDual		    0x3B 
#define W25X_PageProgram		    0x02 
#define W25X_BlockErase			    0xD8 
#define W25X_SectorErase		    0x20 
#define W25X_ChipErase			    0xC7 
#define W25X_PowerDown			    0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			    0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 

#define WIP_Flag                  	0x01  /* Write In Progress (WIP) flag */

// 读取编号
uint W25Q64::ReadID()
{
    uint Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    this->_spi->Start();
    this->_spi->Write(W25X_JedecDeviceID);
    Temp0 = this->_spi->Write(0xFF);
    Temp1 = this->_spi->Write(0xFF);
    Temp2 = this->_spi->Write(0xFF);
    this->_spi->Stop();
    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
    return Temp;
}

#if W25QXXXUSESPISOFT
    W25Q64::W25Q64(SpiSoft *spi)
#else 
    W25Q64::W25Q64(Spi *spi)
#endif 
{
    this->_spi = spi;
    this->PageSize = 256;
}

uint W25Q64::ReadDeviceID(void)
{
    uint Temp = 0;

    this->_spi->Start();

    this->_spi->Write(W25X_DeviceID);
    this->_spi->Write(0XFF);
    this->_spi->Write(0XFF);
    this->_spi->Write(0XFF);

    Temp = this->_spi->Write(0XFF);

    this->_spi->Stop();

    this->ID = this->ReadID();
    return Temp;
}

// 擦除扇区
bool W25Q64::EraseSector(uint SectorAddr)
{
    this->WriteEnable();
    this->WaitForEnd();
    this->_spi->Start();
    this->_spi->Write(W25X_SectorErase);
    this->_spi->Write((SectorAddr &0xFF0000) >> 16);
    this->_spi->Write((SectorAddr &0xFF00) >> 8);
    this->_spi->Write(SectorAddr &0xFF);
    this->_spi->Stop();
    this->WaitForEnd();
    return true;
}

// 擦除页
bool W25Q64::ErasePage(uint pageAddr)
{
    return this->EraseSector(pageAddr);
}

// 等待操作完成
//读取W25QXX的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00

bool W25Q64::WaitForEnd()
{
    byte FLASH_Status = 0;
    this->_spi->Start();
    this->_spi->Write(W25X_ReadStatusReg);
    do
    {
        FLASH_Status = this->_spi->Write(0xFF);
    }
    while ((FLASH_Status &WIP_Flag));
    this->_spi->Stop();

    return true;
}

void W25Q64::WriteEnable(void)
{
    this->_spi->Start();
    this->_spi->Write(W25X_WriteEnable);
    this->_spi->Stop();
}

//写保护
void W25Q64::WriteDisable(void)
{
    this->_spi->Start();
    this->_spi->Write(W25X_WriteDisable);
    this->_spi->Stop();
}

void W25Q64::EraseChip(void)
{
    this->WriteEnable();
    this->WaitForEnd();
    this->_spi->Start();
    this->_spi->Write(W25X_ChipErase);
    this->_spi->Stop();
    this->WaitForEnd();
}

bool W25Q64::WritePage(uint addr, byte *pBuffer, uint NumByteToWrite)
{
    WriteEnable();
    this->_spi->Start();
    this->_spi->Write(W25X_PageProgram);
    this->_spi->Write((addr &0xFF0000) >> 16);
    this->_spi->Write((addr &0xFF00) >> 8);
    this->_spi->Write(addr &0xFF);

    if (NumByteToWrite > this->PageSize)
    {
        NumByteToWrite = this->PageSize;
        //printf("\n\r Err: this->PageWrite too large!");
    }

    while (NumByteToWrite--)
    {
        this->_spi->Write(*pBuffer);
        pBuffer++;
    }

    this->_spi->Stop();

    this->WaitForEnd();
    return true;
}

// 写入数据
bool W25Q64::Write(uint addr, byte *pBuffer, uint NumByteToWrite)
{
    byte NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = addr % this->PageSize;
    count = this->PageSize - Addr;
    NumOfPage = NumByteToWrite / this->PageSize;
    NumOfSingle = NumByteToWrite % this->PageSize;

    if (Addr == 0)
    /* addr is this->PageSize aligned  */
    {
        if (NumOfPage == 0)
        /* NumByteToWrite < this->PageSize */
        {
            this->WritePage(addr, pBuffer, NumByteToWrite);
        }
        else
        /* NumByteToWrite > this->PageSize */
        {
            while (NumOfPage--)
            {
                this->WritePage(addr, pBuffer, this->PageSize);
                addr += this->PageSize;
                pBuffer += this->PageSize;
            }

            this->WritePage(addr, pBuffer, NumOfSingle);
        }
    }
    else
    /* addr is not this->PageSize aligned  */
    {
        if (NumOfPage == 0)
        /* NumByteToWrite < this->PageSize */
        {
            if (NumOfSingle > count)
            /* (NumByteToWrite + addr) > this->PageSize */
            {
                temp = NumOfSingle - count;

                WritePage(addr, pBuffer, count);
                addr += count;
                pBuffer += count;

                this->WritePage(addr, pBuffer, temp);
            }
            else
            {
                WritePage(addr, pBuffer, NumByteToWrite);
            }
        }
        else
        /* NumByteToWrite > this->PageSize */
        {
            NumByteToWrite -= count;
            NumOfPage = NumByteToWrite / this->PageSize;
            NumOfSingle = NumByteToWrite % this->PageSize;

            WritePage(addr, pBuffer, count);
            addr += count;
            pBuffer += count;

            while (NumOfPage--)
            {
                this->WritePage(addr, pBuffer, this->PageSize);
                addr += this->PageSize;
                pBuffer += this->PageSize;
            }

            if (NumOfSingle != 0)
            {
                this->WritePage(addr, pBuffer, NumOfSingle);
            }
        }
    }
    return true;
}

//进入掉电模式
void W25Q64::PowerDown(void)
{
    this->_spi->Start();
    this->_spi->Write(W25X_PowerDown);
    this->_spi->Stop();
}

//唤醒
void W25Q64::WakeUp(void)
{
    this->_spi->Start();
    this->_spi->Write(W25X_ReleasePowerDown);
    this->_spi->Stop(); //等待TRES1
}

// 读取数据
bool W25Q64::Read(uint ReadAddr, byte *pBuffer, uint NumByteToRead)
{
    this->_spi->Start();
    this->_spi->Write(W25X_ReadData);
    this->_spi->Write((ReadAddr &0xFF0000) >> 16);
    this->_spi->Write((ReadAddr &0xFF00) >> 8);
    this->_spi->Write(ReadAddr &0xFF);

    for (int i = 0; i < NumByteToRead; i++)
    {
        pBuffer[i] = this->_spi->Write(0xFF);
    }

    this->_spi->Stop();
    return true;
}

#if 0    

    typedef enum
    {
        FAILED = 0, PASSED = !FAILED
    } TestStatus;


    /* 获取缓冲区的长度 */
    #define TxBufferSize1   (countof(TxBuffer1) - 1)
    #define RxBufferSize1   (countof(TxBuffer1) - 1)
    #define countof(a)      (sizeof(a) / sizeof(*(a)))
    #define BufferSize (countof(Tx_Buffer)-1)

    #define FLASH_WriteAddress     0x00000
    #define FLASH_ReadAddress      FLASH_WriteAddress
    #define FLASH_SectorToErase    FLASH_WriteAddress
    //#define  sFLASH_ID              0xEF3015     //W25X16
    //#define  sFLASH_ID              0xEF4015	 //W25Q16
    #define sFLASH_ID              0XEF4017    //W25Q64


    /* 发送缓冲区初始化 */
    byte Tx_Buffer[] = " 感谢您选用野火stm32开发板\r\n                http://firestm32.taobao.com";
    byte Rx_Buffer[BufferSize];

    /*
     * 函数名：Buffercmp
     * 描述  ：比较两个缓冲区中的数据是否相等
     * 输入  ：-pBuffer1     src缓冲区指针
     *         -pBuffer2     dst缓冲区指针
     *         -BufferLength 缓冲区长度
     * 输出  ：无
     * 返回  ：-PASSED pBuffer1 等于   pBuffer2
     *         -FAILED pBuffer1 不同于 pBuffer2
     */
    TestStatus Buffercmp(byte *pBuffer1, byte *pBuffer2, ushort BufferLength)
    {
        while (BufferLength--)
        {
            if (*pBuffer1 !=  *pBuffer2)
            {
                return FAILED;
            }

            pBuffer1++;
            pBuffer2++;
        }
        return PASSED;
    }
    #if W25QXXXUSESPISOFT
        SpiSoft spi64;
    #else 
        Spi spi64(Spi1);
    #endif 
    W25Q64 w25q64(&spi64);
    void W25Q64Test()
    {
        TestStatus TransferStatus1 = FAILED;

        printf("\r\n 这是一个8Mbyte串行flash(W25Q64)实验 \r\n");
        #if W25QXXXUSESPISOFT
            spi64.SetPin(PA5, PA6, PA7);
            spi64.CPOL = CPOL_High;
            spi64.CPHA = CPHA_2Edge;
        #else 
        #endif 
		spi64.SetNss(PA4);
        spi64.Open();

        /* Get SPI Flash Device ID */
        w25q64.DeviceID = w25q64.ReadDeviceID();
        //		return;
        Sys.Delay(10);

        printf("\r\n FlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", w25q64.ID, w25q64.DeviceID);

        /* Check the SPI Flash ID */
        if (w25q64.ID == sFLASH_ID)
        /* #define  sFLASH_ID  0XEF4017 */
        {
            printf("\r\n 检测到华邦串行flash W25Q64 !\r\n");

            /* Erase FLASH Sector to write on */
            w25q64.EraseSector(FLASH_SectorToErase);

            /* 将发送缓冲区的数据写到flash中 */
            w25q64.Write(FLASH_WriteAddress, Tx_Buffer, BufferSize);
            w25q64.Write(252, Tx_Buffer, BufferSize);
            printf("\r\n 写入的数据为：%s \r\t", Tx_Buffer);

            /* 将刚刚写入的数据读出来放到接收缓冲区中 */
            w25q64.Read(FLASH_ReadAddress, Rx_Buffer, BufferSize);
            printf("\r\n 读出的数据为：%s \r\n", Rx_Buffer);

            /* 检查写入的数据与读出的数据是否相等 */
            TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

            if (PASSED == TransferStatus1)
            {
                printf("\r\n 8M串行flash(W25Q64)测试成功!\n\r");
            }
            else
            {
                printf("\r\n 8M串行flash(W25Q64)测试失败!\n\r");
            }
        } // if (FlashID == sFLASH_ID)
        else
        {
            printf("\r\n 获取不到 W25Q64 ID!\n\r");
        }

        w25q64.PowerDown();
        printf("\r\n\n\r");
    }
#endif 

//读取芯片ID
//返回值如下:				   
//0XEF13,表示芯片型号为W25Q80  
//0XEF14,表示芯片型号为W25Q16    
//0XEF15,表示芯片型号为W25Q32  
//0XEF16,表示芯片型号为W25Q64 
//0XEF17,表示芯片型号为W25Q128 	  
uint W25Q128::ReadID()
{
    ushort Temp = 0;
    //使能器件   
    this->_spi->Start();
    this->_spi->Write(W25X_ManufactDeviceID); //发送读取ID命令	    
    this->_spi->Write(0x00);
    this->_spi->Write(0x00);
    this->_spi->Write(0x00);
    Temp |= this->_spi->Write(0xFF) << 8;
    Temp |= this->_spi->Write(0xFF);
    //取消片选     
    this->_spi->Stop();

    return Temp;
}

#if W25QXXXUSESPISOFT
    W25Q128::W25Q128(SpiSoft *spi): W25Q64(spi)
#else 
    W25Q128::W25Q128(Spi *spi): W25Q64(spi)
#endif 
{
    this->PageSize = 4096;
}

// 擦除扇区
//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个山区的最少时间:150ms
bool W25Q128::EraseSector(uint sectorAddr)
{
    //监视falsh擦除情况,测试用   
    printf("fe:%x\r\n", sectorAddr);
    sectorAddr *= 4096;
    this->WriteEnable(); //SET WEL 	 
    this->WaitForEnd();
    //使能器件   
    this->_spi->Start();
    this->_spi->Write(W25X_SectorErase); //发送扇区擦除指令 
    this->_spi->Write((byte)((sectorAddr) >> 16)); //发送24bit地址    
    this->_spi->Write((byte)((sectorAddr) >> 8));
    this->_spi->Write((byte)sectorAddr);
    //取消片选     
    this->_spi->Stop();
    this->WaitForEnd(); //等待擦除完成

    return true;
}

// 写入一页
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 

bool W25Q128::WritePage(uint addr, byte *buf, uint count)
{
    ushort i;
    this->WriteEnable(); //SET WEL 
    //使能器件   
    this->_spi->Start();
    this->_spi->Write(W25X_PageProgram); //发送写页命令   
    this->_spi->Write((byte)((addr) >> 16)); //发送24bit地址    
    this->_spi->Write((byte)((addr) >> 8));
    this->_spi->Write((byte)addr);
    for (i = 0; i < count; i++)
    {
        this->_spi->Write(buf[i]);
    }
    //循环写数  
    //取消片选     
    this->_spi->Stop();
    this->WaitForEnd(); //等待写入结束
    return true;
}

//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)						
//NumByteToWrite:要写入的字节数(最大65535)
// 写入数据
bool W25Q128::Write(uint WriteAddr, byte *pBuffer, uint NumByteToWrite)
{
    uint secpos;
    ushort secoff;
    ushort secremain;
    ushort i;
    byte *W25QXX_BUF;
    W25QXX_BUF = W25QXX_BUFFER;
    secpos = WriteAddr / 4096; //扇区地址  
    secoff = WriteAddr % 4096; //在扇区内的偏移
    secremain = 4096-secoff; //扇区剩余空间大小   
    //printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
    if (NumByteToWrite <= secremain)
        secremain = NumByteToWrite;
    //不大于4096个字节
    while (1)
    {
        this->Read(secpos *4096, W25QXX_BUF, 4096); //读出整个扇区的内容
        for (i = 0; i < secremain; i++)
        //校验数据
        {
            if (W25QXX_BUF[secoff + i] != 0XFF)
                break;
            //需要擦除  	  
        }
        if (i < secremain)
        //需要擦除
        {
            this->EraseSector(secpos); //擦除这个扇区
            for (i = 0; i < secremain; i++)
            //复制
            {
                W25QXX_BUF[i + secoff] = pBuffer[i];
            }
            this->W25QXX_Write_NoCheck(W25QXX_BUF, secpos *4096, 4096); //写入整个扇区  

        }
        else
            this->W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain);
        //写已经擦除了的,直接写入扇区剩余区间. 				   
        if (NumByteToWrite == secremain)
            break;
        //写入结束了
        else
        //写入未结束
        {
            secpos++; //扇区地址增1
            secoff = 0; //偏移位置为0 	 

            pBuffer += secremain; //指针偏移
            WriteAddr += secremain; //写地址偏移	   
            NumByteToWrite -= secremain; //字节数递减
            if (NumByteToWrite > 4096)
                secremain = 4096;
            //下一个扇区还是写不完
            else
                secremain = NumByteToWrite;
            //下一个扇区可以写完了
        }
    };
    return true;
}

//4Kbytes为一个Sector
//16个扇区为1个Block
//W25Q128
//容量为16M字节,共有128个Block,4096个Sector 
void W25Q128::W25QXX_Init(void)
{

    this->_spi->Stop();

    //	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
    //    spi.Write(0xff); //启动传输
    W25QXX_TYPE = this->ReadID(); //读取FLASH ID.
}

//写W25QXX状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void W25Q128::W25QXX_Write_SR(byte sr)
{
    //使能器件   
    this->_spi->Start();
    this->_spi->Write(W25X_WriteStatusReg); //发送写取状态寄存器命令    
    this->_spi->Write(sr); //写入一个字节  
    //取消片选     
    this->_spi->Stop();

}

//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void W25Q128::W25QXX_Write_NoCheck(byte *pBuffer, uint WriteAddr, ushort NumByteToWrite)
{
    ushort pageremain;
    pageremain = 256-WriteAddr % 256; //单页剩余的字节数		 	    
    if (NumByteToWrite <= pageremain)
        pageremain = NumByteToWrite;
    //不大于256个字节
    while (1)
    {
        this->WritePage(WriteAddr, pBuffer, pageremain);
        if (NumByteToWrite == pageremain)
            break;
        //写入结束了
        else
        //NumByteToWrite>pageremain
        {
            pBuffer += pageremain;
            WriteAddr += pageremain;

            NumByteToWrite -= pageremain; //减去已经写入了的字节数
            if (NumByteToWrite > 256)
                pageremain = 256;
            //一次可以写入256个字节
            else
                pageremain = NumByteToWrite;
            //不够256个字节了
        }
    };
}

#if 0
	//正点原子 探索者开发板
	#if W25QXXXUSESPISOFT
        SpiSoft spi128;
    #else 
        Spi spi128(Spi1);
    #endif     
    W25Q128 w25q128(&spi128);
    OutputPort nsspp;

    //要写入到W25Q16的字符串数组
    const byte TEXT_Buffer[] = 
    {
        "Explorer STM32F4 SPI TEST"
    };

    void w25q128test()
    {
        nsspp.Invert = false;
        nsspp.OpenDrain = false;
        nsspp.Set(PG7);
        nsspp = 1; //PG7输出1,防止NRF干扰SPI FLASH的通信 

		#if W25QXXXUSESPISOFT
			spi128.SetPin(PG6,PG7,PG8);//PB3, PB4, PB5);
            spi128.CPOL = CPOL_High;
            spi128.CPHA = CPHA_2Edge;
		#endif
		spi128.SetNss(PB14);
        byte datatemp[sizeof(TEXT_Buffer)];
        uint FLASH_SIZE;
        w25q128.W25QXX_Init(); //W25QXX初始化
        printf("\r\nSPI TEST\r\n");
        while (w25q128.ReadID() != W25QXXX128)
        //检测不到W25Q128
        {
            printf("W25Q128 Check Failed!\r\n");
			return;
        }
        printf("W25Q128 Ready!\r\n");
        FLASH_SIZE = 16 * 1024 * 1024; //FLASH 大小为16字节
        printf("\r\n");

        printf("Start Write W25Q128....\r\n");
        w25q128.Write(FLASH_SIZE - 100, (byte*)TEXT_Buffer, sizeof(TEXT_Buffer)); //从倒数第100个地址处开始,写入SIZE长度的数据
        printf("W25Q128 Write Finished!\r\n"); //提示传送完成
        printf("\r\n");

        printf("Start Read W25Q128....\r\n");
        w25q128.Read(FLASH_SIZE - 100, datatemp, sizeof(TEXT_Buffer)); //从倒数第100个地址处开始,读出SIZE个字节
        printf("The Data Readed Is:\r\n"); //提示传送完成
        printf((const char*)datatemp);

        printf("\r\n\r\n");
    }
#endif
