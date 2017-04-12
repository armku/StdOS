#include "W25QXXX.h"
#include "stm32f10x.h"

/* Private typedef -----------------------------------------------------------*/
//#define SPI_FLASH_PageSize      4096
#define SPI_FLASH_PageSize      256
#define SPI_FLASH_PerWritePageSize      256

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

#define Dummy_Byte                0xFF

W25QXXX::W25QXXX(Spi *spi, Pin pincs)
{
    this->pSpi = spi;
    if (pincs != P0)
    {
        this->pcs = new OutputPort(pincs, false);
    }
}

void W25QXXX::Init(void)
{
    if (this->pcs)
        *this->pcs = 1;
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_SectorErase
 * Description    : Erases the specified FLASH sector.
 * Input          : SectorAddr: address of the sector to erase.
 * Output         : None
 * Return         : None
 *******************************************************************************/
void W25QXXX::SectorErase(uint SectorAddr)
{
    /* Send write enable instruction */
    WriteEnable();
    WaitForWriteEnd();
    /* Sector Erase */
    /* Select the FLASH: Chip Select low */
    if (this->pcs)
        *this->pcs = 0;
    /* Send Sector Erase instruction */
    this->pSpi->Write(W25X_SectorErase);
    /* Send SectorAddr high nibble address byte */
    this->pSpi->Write((SectorAddr &0xFF0000) >> 16);
    /* Send SectorAddr medium nibble address byte */
    this->pSpi->Write((SectorAddr &0xFF00) >> 8);
    /* Send SectorAddr low nibble address byte */
    this->pSpi->Write(SectorAddr &0xFF);
    /* Deselect the FLASH: Chip Select high */
    *this->pcs = 1;
    /* Wait the end of Flash writing */
    WaitForWriteEnd();
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_BulkErase
 * Description    : Erases the entire FLASH.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void W25QXXX::BulkErase(void)
{
    /* Send write enable instruction */
    WriteEnable();

    /* Bulk Erase */
    /* Select the FLASH: Chip Select low */
    if (this->pcs)
        *this->pcs = 0;
    /* Send Bulk Erase instruction  */
    this->pSpi->Write(W25X_ChipErase);
    /* Deselect the FLASH: Chip Select high */
    if (this->pcs)
        *this->pcs = 1;

    /* Wait the end of Flash writing */
    WaitForWriteEnd();
}

void W25QXXX::WritePage(uint addr, byte *pBuffer, int size)
{
    /* Enable the write access to the FLASH */
    WriteEnable();

    /* Select the FLASH: Chip Select low */
    if (this->pcs)
        *this->pcs = 0;
    /* Send "Write to Memory " instruction */
    this->pSpi->Write(W25X_PageProgram);
    /* Send addr high nibble address byte to write to */
    this->pSpi->Write((addr &0xFF0000) >> 16);
    /* Send addr medium nibble address byte to write to */
    this->pSpi->Write((addr &0xFF00) >> 8);
    /* Send addr low nibble address byte to write to */
    this->pSpi->Write(addr &0xFF);

    if (size > SPI_FLASH_PerWritePageSize)
    {
        size = SPI_FLASH_PerWritePageSize;
        //printf("\n\r Err: SPI_FLASH_PageWrite too large!");
    }

    /* while there is data to be written on the FLASH */
    while (size--)
    {
        /* Send the current byte */
        this->pSpi->Write(*pBuffer);
        /* Point on the next byte to be written */
        pBuffer++;
    }

    /* Deselect the FLASH: Chip Select high */
    if (this->pcs)
        *this->pcs = 1;

    /* Wait the end of Flash writing */
    WaitForWriteEnd();
}

void W25QXXX::Write(uint addr, byte *pBuffer, int size)
{
    byte NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = addr % SPI_FLASH_PageSize;
    count = SPI_FLASH_PageSize - Addr;
    NumOfPage = size / SPI_FLASH_PageSize;
    NumOfSingle = size % SPI_FLASH_PageSize;

    if (Addr == 0)
    /* addr is SPI_FLASH_PageSize aligned  */
    {
        if (NumOfPage == 0)
        /* size < SPI_FLASH_PageSize */
        {
            WritePage(addr, pBuffer, size);
        }
        else
        /* size > SPI_FLASH_PageSize */
        {
            while (NumOfPage--)
            {
                WritePage(addr, pBuffer, SPI_FLASH_PageSize);
                addr += SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }

            WritePage(addr, pBuffer, NumOfSingle);
        }
    }
    else
    /* addr is not SPI_FLASH_PageSize aligned  */
    {
        if (NumOfPage == 0)
        /* size < SPI_FLASH_PageSize */
        {
            if (NumOfSingle > count)
            /* (size + addr) > SPI_FLASH_PageSize */
            {
                temp = NumOfSingle - count;

                WritePage(addr, pBuffer, count);
                addr += count;
                pBuffer += count;

                WritePage(addr, pBuffer, temp);
            }
            else
            {
                WritePage(addr, pBuffer, size);
            }
        }
        else
        /* size > SPI_FLASH_PageSize */
        {
            size -= count;
            NumOfPage = size / SPI_FLASH_PageSize;
            NumOfSingle = size % SPI_FLASH_PageSize;

            WritePage(addr, pBuffer, count);
            addr += count;
            pBuffer += count;

            while (NumOfPage--)
            {
                WritePage(addr, pBuffer, SPI_FLASH_PageSize);
                addr += SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }

            if (NumOfSingle != 0)
            {
                WritePage(addr, pBuffer, NumOfSingle);
            }
        }
    }
}

void W25QXXX::Read(uint addr, byte *pBuffer, int size)
{
    /* Select the FLASH: Chip Select low */
    if (this->pcs)
        *this->pcs = 0;

    /* Send "Read from Memory " instruction */
    this->pSpi->Write(W25X_ReadData);

    /* Send addr high nibble address byte to read from */
    this->pSpi->Write((addr &0xFF0000) >> 16);
    /* Send addr medium nibble address byte to read from */
    this->pSpi->Write((addr &0xFF00) >> 8);
    /* Send addr low nibble address byte to read from */
    this->pSpi->Write(addr &0xFF);

    while (size--)
    /* while there is data to be read */
    {
        /* Read a byte from the FLASH */
        *pBuffer = this->pSpi->Write(Dummy_Byte);
        /* Point to the next location where the byte read will be saved */
        pBuffer++;
    }

    /* Deselect the FLASH: Chip Select high */
    if (this->pcs)
        *this->pcs = 1;
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_ReadID
 * Description    : Reads FLASH identification.
 * Input          : None
 * Output         : None
 * Return         : FLASH identification
 *******************************************************************************/
uint W25QXXX::ReadID(void)
{
    uint Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    /* Select the FLASH: Chip Select low */
    if (this->pcs)
        *this->pcs = 0;

    /* Send "RDID " instruction */
    this->pSpi->Write(W25X_JedecDeviceID);

    /* Read a byte from the FLASH */
    Temp0 = this->pSpi->Write(Dummy_Byte);

    /* Read a byte from the FLASH */
    Temp1 = this->pSpi->Write(Dummy_Byte);

    /* Read a byte from the FLASH */
    Temp2 = this->pSpi->Write(Dummy_Byte);

    /* Deselect the FLASH: Chip Select high */
    if (this->pcs)
        *this->pcs = 1;

    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

    return Temp;
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_ReadID
 * Description    : Reads FLASH identification.
 * Input          : None
 * Output         : None
 * Return         : FLASH identification
 *******************************************************************************/
uint W25QXXX::ReadDeviceID(void)
{
    uint Temp = 0;

    /* Select the FLASH: Chip Select low */
    if (this->pcs)
        *this->pcs = 0;

    /* Send "RDID " instruction */
    this->pSpi->Write(W25X_DeviceID);
    this->pSpi->Write(Dummy_Byte);
    this->pSpi->Write(Dummy_Byte);
    this->pSpi->Write(Dummy_Byte);

    Temp = this->pSpi->Write(Dummy_Byte);

    /* Deselect the FLASH: Chip Select high */
    if (this->pcs)
        *this->pcs = 1;

    return Temp;
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_StartReadSequence
 * Description    : Initiates a read data byte (READ) sequence from the Flash.
 *                  This is done by driving the /CS line low to select the device,
 *                  then the READ instruction is transmitted followed by 3 bytes
 *                  address. This function exit and keep the /CS line low, so the
 *                  Flash still being selected. With this technique the whole
 *                  content of the Flash is read with a single READ instruction.
 * Input          : - addr : FLASH's internal address to read from.
 * Output         : None
 * Return         : None
 *******************************************************************************/
void W25QXXX::StartReadSequence(uint addr)
{
    /* Select the FLASH: Chip Select low */
    if (this->pcs)
        *this->pcs = 0;

    /* Send "Read from Memory " instruction */
    this->pSpi->Write(W25X_ReadData);

    /* Send the 24-bit address of the address to read from -----------------------*/
    /* Send addr high nibble address byte */
    this->pSpi->Write((addr &0xFF0000) >> 16);
    /* Send addr medium nibble address byte */
    this->pSpi->Write((addr &0xFF00) >> 8);
    /* Send addr low nibble address byte */
    this->pSpi->Write(addr &0xFF);
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_WriteEnable
 * Description    : Enables the write access to the FLASH.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void W25QXXX::WriteEnable(void)
{
    /* Select the FLASH: Chip Select low */
    if (this->pcs)
        *this->pcs = 0;

    /* Send "Write Enable" instruction */
    this->pSpi->Write(W25X_WriteEnable);

    /* Deselect the FLASH: Chip Select high */
    if (this->pcs)
        *this->pcs = 1;
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_WaitForWriteEnd
 * Description    : Polls the status of the Write In Progress (WIP) flag in the
 *                  FLASH's status  register  and  loop  until write  opertaion
 *                  has completed.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void W25QXXX::WaitForWriteEnd(void)
{
    byte FLASH_Status = 0;

    /* Select the FLASH: Chip Select low */
    if (this->pcs)
        *this->pcs = 0;

    /* Send "Read Status Register" instruction */
    this->pSpi->Write(W25X_ReadStatusReg);

    /* Loop as long as the memory is busy with a write cycle */
    do
    {
        /* Send a dummy byte to generate the clock needed by the FLASH
        and put the value of the status register in FLASH_Status variable */
        FLASH_Status = this->pSpi->Write(Dummy_Byte);
    }
    while ((FLASH_Status &WIP_Flag) == SET); /* Write in progress */

    /* Deselect the FLASH: Chip Select high */
    if (this->pcs)
        *this->pcs = 1;
}


//进入掉电模式
void W25QXXX::PowerDown(void)
{
    /* Select the FLASH: Chip Select low */
    if (this->pcs)
        *this->pcs = 0;

    /* Send "Power Down" instruction */
    this->pSpi->Write(W25X_PowerDown);

    /* Deselect the FLASH: Chip Select high */
    if (this->pcs)
        *this->pcs = 1;
}

//唤醒
void W25QXXX::WAKEUP(void)
{
    /* Select the FLASH: Chip Select low */
    if (this->pcs)
        *this->pcs = 0;

    /* Send "Power Down" instruction */
    this->pSpi->Write(W25X_ReleasePowerDown);

    /* Deselect the FLASH: Chip Select high */
    if (this->pcs)
        *this->pcs = 1;
    //等待TRES1
}

#ifdef DEBUG
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
    uint8_t Tx_Buffer[] = "感谢您选用stm32开发板\r\n";
    uint8_t Rx_Buffer[BufferSize];

    uint DeviceID = 0;
    uint FlashID = 0;
    TestStatus TransferStatus1 = FAILED;

    TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);
    void W25QXXX::Test()
    {
        printf("\r\n这是一个8Mbyte串行flash(W25Q64)实验 \r\n");

        Spi spi(Spi1, 9000000, false);
        W25QXXX w25q64(&spi, PA4);
        /* 8M串行flash W25Q64初始化 */
        w25q64.Init();

        /* Get SPI Flash Device ID */
        DeviceID = w25q64.ReadDeviceID();

        Sys.Delay(100);

        /* Get SPI Flash ID */
        FlashID = w25q64.ReadID();

        printf("\r\nFlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);

        /* Check the SPI Flash ID */
        if (FlashID == sFLASH_ID)
        /* #define  sFLASH_ID  0XEF4017 */
        {
            printf("检测到华邦串行flash W25Q64 !\r\n");

            /* Erase SPI FLASH Sector to write on */
            w25q64.SectorErase(FLASH_SectorToErase);

            /* 将发送缓冲区的数据写到flash中 */
            w25q64.Write(FLASH_WriteAddress, Tx_Buffer, BufferSize);
            w25q64.Write(252, Tx_Buffer, BufferSize);
            printf("写入的数据为：%s \r\n", Tx_Buffer);

            /* 将刚刚写入的数据读出来放到接收缓冲区中 */
            w25q64.Read(FLASH_ReadAddress, Rx_Buffer, BufferSize);
            printf("读出的数据为：%s \r\n", Rx_Buffer);

            /* 检查写入的数据与读出的数据是否相等 */
            TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

            if (PASSED == TransferStatus1)
            {
                printf("8M串行flash(W25Q64)测试成功!\r\n");
            }
            else
            {
                printf("8M串行flash(W25Q64)测试失败!\r\n");
            }
        } // if (FlashID == sFLASH_ID)
        else
        {
            printf("获取不到 W25Q64 ID!\r\n");
        }

        w25q64.PowerDown();
    }
    TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength)
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

#endif
