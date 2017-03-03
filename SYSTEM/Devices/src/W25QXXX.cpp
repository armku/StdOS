#include "W25QXXX.h"

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

W25QXXX::W25QXXX(Spi * spi,Pin pincs)
{
	this->pSpi=spi;
	this->pcs=new OutputPort(pincs,false); 
}

void W25QXXX::Init(void)
{        
	*this->pcs=1;
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
    *this->pcs=0;
    /* Send Sector Erase instruction */
    this->pSpi->Write(W25X_SectorErase);
    /* Send SectorAddr high nibble address byte */
    this->pSpi->Write((SectorAddr &0xFF0000) >> 16);
    /* Send SectorAddr medium nibble address byte */
    this->pSpi->Write((SectorAddr &0xFF00) >> 8);
    /* Send SectorAddr low nibble address byte */
    this->pSpi->Write(SectorAddr &0xFF);
    /* Deselect the FLASH: Chip Select high */
    *this->pcs=1;
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
    *this->pcs=0;
    /* Send Bulk Erase instruction  */
    this->pSpi->Write(W25X_ChipErase);
    /* Deselect the FLASH: Chip Select high */
    *this->pcs=1;

    /* Wait the end of Flash writing */
    WaitForWriteEnd();
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_PageWrite
 * Description    : Writes more than one byte to the FLASH with a single WRITE
 *                  cycle(Page WRITE sequence). The number of byte can't exceed
 *                  the FLASH page size.
 * Input          : - pBuffer : pointer to the buffer  containing the data to be
 *                    written to the FLASH.
 *                  - WriteAddr : FLASH's internal address to write to.
 *                  - NumByteToWrite : number of bytes to write to the FLASH,
 *                    must be equal or less than "SPI_FLASH_PageSize" value.
 * Output         : None
 * Return         : None
 *******************************************************************************/
void W25QXXX::PageWrite(byte *pBuffer, uint WriteAddr, ushort NumByteToWrite)
{
    /* Enable the write access to the FLASH */
    WriteEnable();

    /* Select the FLASH: Chip Select low */
    *this->pcs=0;
    /* Send "Write to Memory " instruction */
    this->pSpi->Write(W25X_PageProgram);
    /* Send WriteAddr high nibble address byte to write to */
    this->pSpi->Write((WriteAddr &0xFF0000) >> 16);
    /* Send WriteAddr medium nibble address byte to write to */
    this->pSpi->Write((WriteAddr &0xFF00) >> 8);
    /* Send WriteAddr low nibble address byte to write to */
    this->pSpi->Write(WriteAddr &0xFF);

    if (NumByteToWrite > SPI_FLASH_PerWritePageSize)
    {
        NumByteToWrite = SPI_FLASH_PerWritePageSize;
        //printf("\n\r Err: SPI_FLASH_PageWrite too large!");
    }

    /* while there is data to be written on the FLASH */
    while (NumByteToWrite--)
    {
        /* Send the current byte */
        this->pSpi->Write(*pBuffer);
        /* Point on the next byte to be written */
        pBuffer++;
    }

    /* Deselect the FLASH: Chip Select high */
    *this->pcs=1;

    /* Wait the end of Flash writing */
    WaitForWriteEnd();
}

/*******************************************************************************
 * Function Name  : SPI_FLASH_BufferWrite
 * Description    : Writes block of data to the FLASH. In this function, the
 *                  number of WRITE cycles are reduced, using Page WRITE sequence.
 * Input          : - pBuffer : pointer to the buffer  containing the data to be
 *                    written to the FLASH.
 *                  - WriteAddr : FLASH's internal address to write to.
 *                  - NumByteToWrite : number of bytes to write to the FLASH.
 * Output         : None
 * Return         : None
 *******************************************************************************/
void W25QXXX::BufferWrite(byte *pBuffer, uint WriteAddr, ushort NumByteToWrite)
{
    byte NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = WriteAddr % SPI_FLASH_PageSize;
    count = SPI_FLASH_PageSize - Addr;
    NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
    NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

    if (Addr == 0)
    /* WriteAddr is SPI_FLASH_PageSize aligned  */
    {
        if (NumOfPage == 0)
        /* NumByteToWrite < SPI_FLASH_PageSize */
        {
            PageWrite(pBuffer, WriteAddr, NumByteToWrite);
        }
        else
        /* NumByteToWrite > SPI_FLASH_PageSize */
        {
            while (NumOfPage--)
            {
                PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr += SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }

            PageWrite(pBuffer, WriteAddr, NumOfSingle);
        }
    }
    else
    /* WriteAddr is not SPI_FLASH_PageSize aligned  */
    {
        if (NumOfPage == 0)
        /* NumByteToWrite < SPI_FLASH_PageSize */
        {
            if (NumOfSingle > count)
            /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
            {
                temp = NumOfSingle - count;

                PageWrite(pBuffer, WriteAddr, count);
                WriteAddr += count;
                pBuffer += count;

                PageWrite(pBuffer, WriteAddr, temp);
            }
            else
            {
                PageWrite(pBuffer, WriteAddr, NumByteToWrite);
            }
        }
        else
        /* NumByteToWrite > SPI_FLASH_PageSize */
        {
            NumByteToWrite -= count;
            NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
            NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

            PageWrite(pBuffer, WriteAddr, count);
            WriteAddr += count;
            pBuffer += count;

            while (NumOfPage--)
            {
                PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr += SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }

            if (NumOfSingle != 0)
            {
                PageWrite(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

#if 0
    void SPI_FLASH_BufferWrite(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
    {
        u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, i;
        u32 CurrentAddr;


        CurrentAddr = WriteAddr;

        Addr = WriteAddr % SPI_FLASH_PageSize;
        count = SPI_FLASH_PageSize - Addr;

        NumOfPage = (NumByteToWrite - count) / SPI_FLASH_PageSize;
        NumOfSingle = (NumByteToWrite - count) % SPI_FLASH_PageSize;


        if (count)
        {
            SPI_FLASH_PageWrite(pBuffer, CurrentAddr, count);

            CurrentAddr += count;
            pBuffer += count;

        }

        for (i = 0; i < NumOfPage; i++)
        {
            SPI_FLASH_PageWrite(pBuffer, CurrentAddr, SPI_FLASH_PageSize);

            CurrentAddr += SPI_FLASH_PageSize;
            pBuffer += SPI_FLASH_PageSize;

        }

        if (NumOfSingle)
            SPI_FLASH_PageWrite(pBuffer, CurrentAddr, NumOfSingle);

    }
#endif 

/*******************************************************************************
 * Function Name  : SPI_FLASH_BufferRead
 * Description    : Reads a block of data from the FLASH.
 * Input          : - pBuffer : pointer to the buffer that receives the data read
 *                    from the FLASH.
 *                  - ReadAddr : FLASH's internal address to read from.
 *                  - NumByteToRead : number of bytes to read from the FLASH.
 * Output         : None
 * Return         : None
 *******************************************************************************/
void W25QXXX::BufferRead(byte *pBuffer, uint ReadAddr, ushort NumByteToRead)
{
    /* Select the FLASH: Chip Select low */
    *this->pcs=0;

    /* Send "Read from Memory " instruction */
    this->pSpi->Write(W25X_ReadData);

    /* Send ReadAddr high nibble address byte to read from */
    this->pSpi->Write((ReadAddr &0xFF0000) >> 16);
    /* Send ReadAddr medium nibble address byte to read from */
    this->pSpi->Write((ReadAddr &0xFF00) >> 8);
    /* Send ReadAddr low nibble address byte to read from */
    this->pSpi->Write(ReadAddr &0xFF);

    while (NumByteToRead--)
    /* while there is data to be read */
    {
        /* Read a byte from the FLASH */
        *pBuffer = this->pSpi->Write(Dummy_Byte);
        /* Point to the next location where the byte read will be saved */
        pBuffer++;
    }

    /* Deselect the FLASH: Chip Select high */
    *this->pcs=1;
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
    *this->pcs=0;

    /* Send "RDID " instruction */
    this->pSpi->Write(W25X_JedecDeviceID);

    /* Read a byte from the FLASH */
    Temp0 = this->pSpi->Write(Dummy_Byte);

    /* Read a byte from the FLASH */
    Temp1 = this->pSpi->Write(Dummy_Byte);

    /* Read a byte from the FLASH */
    Temp2 = this->pSpi->Write(Dummy_Byte);

    /* Deselect the FLASH: Chip Select high */
    *this->pcs=1;

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
    *this->pcs=0;

    /* Send "RDID " instruction */	
	this->pSpi->Write(W25X_DeviceID);
	this->pSpi->Write(Dummy_Byte);
	this->pSpi->Write(Dummy_Byte);
	this->pSpi->Write(Dummy_Byte);
	
	Temp = this->pSpi->Write(Dummy_Byte);	
	
    /* Deselect the FLASH: Chip Select high */
    *this->pcs=1;

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
 * Input          : - ReadAddr : FLASH's internal address to read from.
 * Output         : None
 * Return         : None
 *******************************************************************************/
void W25QXXX::StartReadSequence(uint ReadAddr)
{
    /* Select the FLASH: Chip Select low */
    *this->pcs=0;

    /* Send "Read from Memory " instruction */
    this->pSpi->Write(W25X_ReadData);

    /* Send the 24-bit address of the address to read from -----------------------*/
    /* Send ReadAddr high nibble address byte */
    this->pSpi->Write((ReadAddr &0xFF0000) >> 16);
    /* Send ReadAddr medium nibble address byte */
    this->pSpi->Write((ReadAddr &0xFF00) >> 8);
    /* Send ReadAddr low nibble address byte */
    this->pSpi->Write(ReadAddr &0xFF);
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
    *this->pcs=0;

    /* Send "Write Enable" instruction */
    this->pSpi->Write(W25X_WriteEnable);

    /* Deselect the FLASH: Chip Select high */
    *this->pcs=1;
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
    u8 FLASH_Status = 0;

    /* Select the FLASH: Chip Select low */
    *this->pcs=0;

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
    *this->pcs=1;
}


//进入掉电模式
void W25QXXX::PowerDown(void)
{
    /* Select the FLASH: Chip Select low */
    *this->pcs=0;

    /* Send "Power Down" instruction */
    this->pSpi->Write(W25X_PowerDown);

    /* Deselect the FLASH: Chip Select high */
    *this->pcs=1;
}

//唤醒
void W25QXXX::WAKEUP(void)
{
    /* Select the FLASH: Chip Select low */
    *this->pcs=0;

    /* Send "Power Down" instruction */
    this->pSpi->Write(W25X_ReleasePowerDown);

    /* Deselect the FLASH: Chip Select high */
    *this->pcs=1; //等待TRES1
}
