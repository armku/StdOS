#include "W25QXXX.h"
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */

// 设置操作地址
void W25Q64::SetAddr(uint addr){

}
// 等待操作完成
bool W25Q64::WaitForEnd()
{

    return true;
}

// 读取编号
uint W25Q64::ReadID()
{
    uint Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    /* Select the FLASH: Chip Select low */
    this->_spi->Start();
    /* Send "RDID " instruction */
    this->_spi->Write(W25X_JedecDeviceID);
    Temp0 = this->_spi->Write(0xFF);
    Temp1 = this->_spi->Write(0xFF);
    Temp2 = this->_spi->Write(0xFF);
    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();
    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
    return Temp;
}

W25Q64::W25Q64(Spi *spi)
{
    this->_spi = spi;
    this->PageSize = 256;
}

W25Q64::~W25Q64(){

}

// 擦除扇区
bool W25Q64::Erase(uint sector)
{
    return true;
}

// 擦除页
bool W25Q64::ErasePage(uint pageAddr)
{
    return true;
}

// 写入一页
bool W25Q64::WritePage(uint addr, byte *buf, uint count)
{
    return true;
}

// 读取一页
bool W25Q64::ReadPage(uint addr, byte *buf, uint count)
{
    return true;
}



/*******************************************************************************
 * Function Name  : SPI_FLASH_ReadID
 * Description    : Reads FLASH identification.
 * Input          : None
 * Output         : None
 * Return         : FLASH identification
 *******************************************************************************/
uint W25Q64::ReadDeviceID(void)
{
    uint Temp = 0;

    /* Select the FLASH: Chip Select low */
    this->_spi->Start();

    /* Send "RDID " instruction */
    this->_spi->Write(W25X_DeviceID);
    this->_spi->Write(0XFF);
    this->_spi->Write(0XFF);
    this->_spi->Write(0XFF);

    /* Read a byte from the FLASH */
    Temp = this->_spi->Write(0XFF);

    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();

    this->ID = this->ReadID();
    return Temp;
}

/*******************************************************************************
 * Function Name  : SectorErase
 * Description    : Erases the specified FLASH sector.
 * Input          : SectorAddr: address of the sector to erase.
 * Output         : None
 * Return         : None
 *******************************************************************************/
void W25Q64::SectorErase(uint SectorAddr)
{
    /* Send write enable instruction */
    WriteEnable();
    WaitForWriteEnd();
    /* Sector Erase */
    /* Select the FLASH: Chip Select low */
    this->_spi->Start();
    /* Send Sector Erase instruction */
    this->_spi->Write(W25X_SectorErase);
    /* Send SectorAddr high nibble address byte */
    this->_spi->Write((SectorAddr &0xFF0000) >> 16);
    /* Send SectorAddr medium nibble address byte */
    this->_spi->Write((SectorAddr &0xFF00) >> 8);
    /* Send SectorAddr low nibble address byte */
    this->_spi->Write(SectorAddr &0xFF);
    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();
    /* Wait the end of Flash writing */
    WaitForWriteEnd();
}

/*******************************************************************************
 * Function Name  : WaitForWriteEnd
 * Description    : Polls the status of the Write In Progress (WIP) flag in the
 *                  FLASH's status  register  and  loop  until write  opertaion
 *                  has completed.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void W25Q64::WaitForWriteEnd(void)
{
    byte FLASH_Status = 0;

    /* Select the FLASH: Chip Select low */
    this->_spi->Start();

    /* Send "Read Status Register" instruction */
    this->_spi->Write(W25X_ReadStatusReg);

    /* Loop as long as the memory is busy with a write cycle */
    do
    {
        /* Send a dummy byte to generate the clock needed by the FLASH
        and put the value of the status register in FLASH_Status variable */
        FLASH_Status = this->_spi->Write(0xFF);
    }
    while ((FLASH_Status &WIP_Flag) == SET); /* Write in progress */

    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();
}

/*******************************************************************************
 * Function Name  : WriteEnable
 * Description    : Enables the write access to the FLASH.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void W25Q64::WriteEnable(void)
{
    /* Select the FLASH: Chip Select low */
    this->_spi->Start();

    /* Send "Write Enable" instruction */
    this->_spi->Write(W25X_WriteEnable);

    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();
}

/*******************************************************************************
 * Function Name  : BulkErase
 * Description    : Erases the entire FLASH.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void W25Q64::BulkErase(void)
{
    /* Send write enable instruction */
    WriteEnable();

    /* Bulk Erase */
    /* Select the FLASH: Chip Select low */
    this->_spi->Start();
    /* Send Bulk Erase instruction  */
    this->_spi->Write(W25X_ChipErase);
    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();

    /* Wait the end of Flash writing */
    WaitForWriteEnd();
}

/*******************************************************************************
 * Function Name  : PageWrite
 * Description    : Writes more than one byte to the FLASH with a single WRITE
 *                  cycle(Page WRITE sequence). The number of byte can't exceed
 *                  the FLASH page size.
 * Input          : - pBuffer : pointer to the buffer  containing the data to be
 *                    written to the FLASH.
 *                  - addr : FLASH's internal address to write to.
 *                  - NumByteToWrite : number of bytes to write to the FLASH,
 *                    must be equal or less than "SPI_FLASH_PageSize" value.
 * Output         : None
 * Return         : None
 *******************************************************************************/
void W25Q64::PageWrite(byte *pBuffer, uint addr, uint NumByteToWrite)
{
    /* Enable the write access to the FLASH */
    WriteEnable();

    /* Select the FLASH: Chip Select low */
    this->_spi->Start();
    /* Send "Write to Memory " instruction */
    this->_spi->Write(W25X_PageProgram);
    /* Send addr high nibble address byte to write to */
    this->_spi->Write((addr &0xFF0000) >> 16);
    /* Send addr medium nibble address byte to write to */
    this->_spi->Write((addr &0xFF00) >> 8);
    /* Send addr low nibble address byte to write to */
    this->_spi->Write(addr &0xFF);

    if (NumByteToWrite > this->PageSize)
    {
        NumByteToWrite = this->PageSize;
        //printf("\n\r Err: this->PageWrite too large!");
    }

    /* while there is data to be written on the FLASH */
    while (NumByteToWrite--)
    {
        /* Send the current byte */
        this->_spi->Write(*pBuffer);
        /* Point on the next byte to be written */
        pBuffer++;
    }

    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();

    /* Wait the end of Flash writing */
    WaitForWriteEnd();
}

/*******************************************************************************
 * Function Name  : Write
 * Description    : Writes block of data to the FLASH. In this function, the
 *                  number of WRITE cycles are reduced, using Page WRITE sequence.
 * Input          : - pBuffer : pointer to the buffer  containing the data to be
 *                    written to the FLASH.
 *                  - addr : FLASH's internal address to write to.
 *                  - NumByteToWrite : number of bytes to write to the FLASH.
 * Output         : None
 * Return         : None
 *******************************************************************************/
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
            this->PageWrite(pBuffer, addr, NumByteToWrite);
        }
        else
         /* NumByteToWrite > this->PageSize */
        {
            while (NumOfPage--)
            {
                this->PageWrite(pBuffer, addr, this->PageSize);
                addr += this->PageSize;
                pBuffer += this->PageSize;
            }

            this->PageWrite(pBuffer, addr, NumOfSingle);
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

                PageWrite(pBuffer, addr, count);
                addr += count;
                pBuffer += count;

                this->PageWrite(pBuffer, addr, temp);
            }
            else
            {
                PageWrite(pBuffer, addr, NumByteToWrite);
            }
        }
        else
         /* NumByteToWrite > this->PageSize */
        {
            NumByteToWrite -= count;
            NumOfPage = NumByteToWrite / this->PageSize;
            NumOfSingle = NumByteToWrite % this->PageSize;

            PageWrite(pBuffer, addr, count);
            addr += count;
            pBuffer += count;

            while (NumOfPage--)
            {
                this->PageWrite(pBuffer, addr, this->PageSize);
                addr += this->PageSize;
                pBuffer += this->PageSize;
            }

            if (NumOfSingle != 0)
            {
                this->PageWrite(pBuffer, addr, NumOfSingle);
            }
        }
    }
	return true;
}

//进入掉电模式
void W25Q64::PowerDown(void)
{
    /* Select the FLASH: Chip Select low */
    this->_spi->Start();

    /* Send "Power Down" instruction */
    this->_spi->Write(W25X_PowerDown);

    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();
}

//唤醒
void W25Q64::WakeUp(void)
{
    /* Select the FLASH: Chip Select low */
    this->_spi->Start();

    /* Send "Power Down" instruction */
    this->_spi->Write(W25X_ReleasePowerDown);

    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop(); //等待TRES1
}

/*******************************************************************************
 * Function Name  : Read
 * Description    : Reads a block of data from the FLASH.
 * Input          : - pBuffer : pointer to the buffer that receives the data read
 *                    from the FLASH.
 *                  - ReadAddr : FLASH's internal address to read from.
 *                  - NumByteToRead : number of bytes to read from the FLASH.
 * Output         : None
 * Return         : None
 *******************************************************************************/
// 读取数据
bool W25Q64::Read(uint ReadAddr, byte *pBuffer, uint NumByteToRead)
{	
    /* Select the FLASH: Chip Select low */
    this->_spi->Start();

    /* Send "Read from Memory " instruction */
    this->_spi->Write(W25X_ReadData);

    /* Send ReadAddr high nibble address byte to read from */
    this->_spi->Write((ReadAddr &0xFF0000) >> 16);
    /* Send ReadAddr medium nibble address byte to read from */
    this->_spi->Write((ReadAddr &0xFF00) >> 8);
    /* Send ReadAddr low nibble address byte to read from */
    this->_spi->Write(ReadAddr &0xFF);

    while (NumByteToRead--)
     /* while there is data to be read */
    {
        /* Read a byte from the FLASH */
        *pBuffer = this->_spi->Write(0xFF);
        /* Point to the next location where the byte read will be saved */
        pBuffer++;
    }

    /* Deselect the FLASH: Chip Select high */
    this->_spi->Stop();
	return true;
}



/*******************************************************************************
 * Function Name  : StartReadSequence
 * Description    : Initiates a read data byte (READ) sequence from the Flash.
 *                  This is done by driving the /CS line low to select the device,
 *                  then the READ instruction is transmitted followed by 3 bytes
 *                  address. This function exit and keep the /CS line low, so the
 *                  Flash still being selected. With this technique the whole
 *                  content of the Flash is read with a single READ instruction.
 * Input          : - ReadAddr : FLASH's internal address to read from.
 * Output         : None
 * Return         : None
 *******************************************************************************/
void W25Q64::StartReadSequence(uint ReadAddr)
{
    /* Select the FLASH: Chip Select low */
    this->_spi->Start();

    /* Send "Read from Memory " instruction */
    this->_spi->Write(W25X_ReadData);

    /* Send the 24-bit address of the address to read from -----------------------*/
    /* Send ReadAddr high nibble address byte */
    this->_spi->Write((ReadAddr &0xFF0000) >> 16);
    /* Send ReadAddr medium nibble address byte */
    this->_spi->Write((ReadAddr &0xFF00) >> 8);
    /* Send ReadAddr low nibble address byte */
    this->_spi->Write(ReadAddr &0xFF);
}

Spi spi(Spi1);
W25Q64 w25q64(&spi);

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


void W25Q64Test()
{
    __IO TestStatus TransferStatus1 = FAILED;

    printf("\r\n 这是一个8Mbyte串行flash(W25Q64)实验 \r\n");

    spi.Open();
    /* Get SPI Flash Device ID */
    w25q64.DeviceID = w25q64.ReadDeviceID();

    Sys.Delay(10);

    printf("\r\n FlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", w25q64.ID, w25q64.DeviceID);

    /* Check the SPI Flash ID */
    if (w25q64.ID == sFLASH_ID)
     /* #define  sFLASH_ID  0XEF4017 */
    {
        printf("\r\n 检测到华邦串行flash W25Q64 !\r\n");

        /* Erase FLASH Sector to write on */
        w25q64.SectorErase(FLASH_SectorToErase);

        /* 将发送缓冲区的数据写到flash中 */
        w25q64.Write(FLASH_WriteAddress,Tx_Buffer,  BufferSize);
        w25q64.Write( 252,Tx_Buffer, BufferSize);
        printf("\r\n 写入的数据为：%s \r\t", Tx_Buffer);

        /* 将刚刚写入的数据读出来放到接收缓冲区中 */
        w25q64.Read(FLASH_ReadAddress,Rx_Buffer,  BufferSize);
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
