#include "w25qxx.h"

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

CW25Qxxx::CW25Qxxx(PinPort pinscs,PinPort pinsclk,PinPort pinsmiso,PinPort pinsmosi,ESpiChannel spichannel)
{
	this->pincs=new BasePort(pinscs);
	this->pinclk=new BasePort(pinsclk);
	this->pinmiso=new BasePort(pinsmiso);
	this->pinmosi=new BasePort(pinsmosi);
	this->spi=new CSpiHard(spichannel);
	
	this->pincs->SetModeOut_PP();
	this->pinclk->SetModeAF_PP();
	this->pinmiso->SetModeAF_PP();
	this->pinmosi->SetModeAF_PP();
		
}
CW25Qxxx::~CW25Qxxx(){

}

void CW25Qxxx::Init()
{    
    /* Deselect the FLASH: Chip Select high */    
	this->pincs->Set();
   
	this->spi->Init();
}

uint32_t CW25Qxxx::ReadDeviceID(void)
{
    uint32_t Temp = 0;

    /* Select the FLASH: Chip Select low */
    this->pincs->Reset(); 

    /* Send "RDID " instruction */
    this->spi->WriteByte(W25X_DeviceID);
    this->spi->ReadByte();
    this->spi->ReadByte();
    this->spi->ReadByte();

    /* Read a byte from the FLASH */
    Temp = this->spi->ReadByte();

    /* Deselect the FLASH: Chip Select high */
    this->pincs->Set();

    return Temp;
}

uint32_t CW25Qxxx::ReadID(void)
{
    uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    /* Select the FLASH: Chip Select low */
    this->pincs->Reset();

    /* Send "RDID " instruction */
    this->spi->WriteByte(W25X_JedecDeviceID);

    /* Read a byte from the FLASH */
    Temp0 = this->spi->ReadByte();

    /* Read a byte from the FLASH */
    Temp1 = this->spi->ReadByte();

    /* Read a byte from the FLASH */
    Temp2 = this->spi->ReadByte();

    /* Deselect the FLASH: Chip Select high */
    this->pincs->Set();

    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

    return Temp;
}

void CW25Qxxx::PowerDown(void)
{
    /* Select the FLASH: Chip Select low */
    this->pincs->Reset();

    /* Send "Power Down" instruction */
    this->spi->WriteByte(W25X_PowerDown);

    /* Deselect the FLASH: Chip Select high */
    this->pincs->Set();
}

void CW25Qxxx::SectorErase(uint32_t SectorAddr)
{
    /* Send write enable instruction */
    this->WriteEnable();
    this->WaitForWriteEnd();
    /* Sector Erase */
    /* Select the FLASH: Chip Select low */
    this->pincs->Reset();
    /* Send Sector Erase instruction */
    this->spi->WriteByte(W25X_SectorErase);
    /* Send SectorAddr high nibble address byte */
    this->spi->WriteByte((SectorAddr &0xFF0000) >> 16);
    /* Send SectorAddr medium nibble address byte */
    this->spi->WriteByte((SectorAddr &0xFF00) >> 8);
    /* Send SectorAddr low nibble address byte */
    this->spi->WriteByte(SectorAddr &0xFF);
    /* Deselect the FLASH: Chip Select high */
    this->pincs->Set();
    /* Wait the end of Flash writing */
    this->WaitForWriteEnd();
}

void CW25Qxxx::BulkErase(void)
{
    /* Send write enable instruction */
    this->WriteEnable();

    /* Bulk Erase */
    /* Select the FLASH: Chip Select low */
    this->pincs->Reset();
    /* Send Bulk Erase instruction  */
    this->spi->WriteByte(W25X_ChipErase);
    /* Deselect the FLASH: Chip Select high */
    this->pincs->Set();

    /* Wait the end of Flash writing */
    this->WaitForWriteEnd();
}

void CW25Qxxx::PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    /* Enable the write access to the FLASH */
    this->WriteEnable();

    /* Select the FLASH: Chip Select low */
    this->pincs->Reset();
    /* Send "Write to Memory " instruction */
    this->spi->WriteByte(W25X_PageProgram);
    /* Send WriteAddr high nibble address byte to write to */
    this->spi->WriteByte((WriteAddr &0xFF0000) >> 16);
    /* Send WriteAddr medium nibble address byte to write to */
    this->spi->WriteByte((WriteAddr &0xFF00) >> 8);
    /* Send WriteAddr low nibble address byte to write to */
    this->spi->WriteByte(WriteAddr &0xFF);

    if (NumByteToWrite > SPI_FLASH_PerWritePageSize)
    {
        NumByteToWrite = SPI_FLASH_PerWritePageSize;
        //printf("\n\r Err: SPI_FLASH_PageWrite too large!");
    }

    /* while there is data to be written on the FLASH */
    while (NumByteToWrite--)
    {
        /* Send the current byte */
        this->spi->WriteByte(*pBuffer);
        /* Point on the next byte to be written */
        pBuffer++;
    }

    /* Deselect the FLASH: Chip Select high */
    this->pincs->Set();

    /* Wait the end of Flash writing */
    this->WaitForWriteEnd();
}

void CW25Qxxx::BufferWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

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
            this->PageWrite(pBuffer, WriteAddr, NumByteToWrite);
        }
        else
         /* NumByteToWrite > SPI_FLASH_PageSize */
        {
            while (NumOfPage--)
            {
                this->PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr += SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }

            this->PageWrite(pBuffer, WriteAddr, NumOfSingle);
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

                this->PageWrite(pBuffer, WriteAddr, count);
                WriteAddr += count;
                pBuffer += count;

                this->PageWrite(pBuffer, WriteAddr, temp);
            }
            else
            {
                this->PageWrite(pBuffer, WriteAddr, NumByteToWrite);
            }
        }
        else
         /* NumByteToWrite > SPI_FLASH_PageSize */
        {
            NumByteToWrite -= count;
            NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
            NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

            this->PageWrite(pBuffer, WriteAddr, count);
            WriteAddr += count;
            pBuffer += count;

            while (NumOfPage--)
            {
                this->PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr += SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }

            if (NumOfSingle != 0)
            {
                this->PageWrite(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

void CW25Qxxx::BufferRead(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    /* Select the FLASH: Chip Select low */
    this->pincs->Reset();

    /* Send "Read from Memory " instruction */
    this->spi->WriteByte(W25X_ReadData);

    /* Send ReadAddr high nibble address byte to read from */
    this->spi->WriteByte((ReadAddr &0xFF0000) >> 16);
    /* Send ReadAddr medium nibble address byte to read from */
    this->spi->WriteByte((ReadAddr &0xFF00) >> 8);
    /* Send ReadAddr low nibble address byte to read from */
    this->spi->WriteByte(ReadAddr &0xFF);

    while (NumByteToRead--)
     /* while there is data to be read */
    {
        /* Read a byte from the FLASH */
        *pBuffer = this->spi->ReadByte();
        /* Point to the next location where the byte read will be saved */
        pBuffer++;
    }

    /* Deselect the FLASH: Chip Select high */
    this->pincs->Set();
}

void CW25Qxxx::StartReadSequence(uint32_t ReadAddr)
{
    /* Select the FLASH: Chip Select low */
    this->pincs->Reset();

    /* Send "Read from Memory " instruction */
    this->spi->WriteByte(W25X_ReadData);

    /* Send the 24-bit address of the address to read from -----------------------*/
    /* Send ReadAddr high nibble address byte */
    this->spi->WriteByte((ReadAddr &0xFF0000) >> 16);
    /* Send ReadAddr medium nibble address byte */
    this->spi->WriteByte((ReadAddr &0xFF00) >> 8);
    /* Send ReadAddr low nibble address byte */
    this->spi->WriteByte(ReadAddr &0xFF);
}

void CW25Qxxx::WriteEnable(void)
{
    /* Select the FLASH: Chip Select low */
    this->pincs->Reset();

    /* Send "Write Enable" instruction */
    this->spi->WriteByte(W25X_WriteEnable);

    /* Deselect the FLASH: Chip Select high */
    this->pincs->Set();
}
#define SET 1	//½â¾ö±àÒë´íÎó
void CW25Qxxx::WaitForWriteEnd(void)
{
    uint8_t FLASH_Status = 0;

    /* Select the FLASH: Chip Select low */
    this->pincs->Reset();

    /* Send "Read Status Register" instruction */
    this->spi->WriteByte(W25X_ReadStatusReg);

    /* Loop as long as the memory is busy with a write cycle */
    do
    {
        /* Send a dummy byte to generate the clock needed by the FLASH
        and put the value of the status register in FLASH_Status variable */
        FLASH_Status = this->spi->ReadByte();
    }
    while ((FLASH_Status &WIP_Flag) == SET); /* Write in progress */

    /* Deselect the FLASH: Chip Select high */
    this->pincs->Set();
}

//»½ÐÑ
void CW25Qxxx::WAKEUP(void)
{
    /* Select the FLASH: Chip Select low */
    this->pincs->Reset();

    /* Send "Power Down" instruction */
    this->spi->WriteByte(W25X_ReleasePowerDown);

    /* Deselect the FLASH: Chip Select high */
    this->pincs->Set(); //µÈ´ýTRES1
}
