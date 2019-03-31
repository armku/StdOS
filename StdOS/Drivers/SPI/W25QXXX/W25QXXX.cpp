#include "W25QXXX.h"

//读取芯片ID
//返回值如下:				   
//0XEF13,表示芯片型号为W25Q80  
//0XEF14,表示芯片型号为W25Q16    
//0XEF15,表示芯片型号为W25Q32  
//0XEF16,表示芯片型号为W25Q64 
//0XEF17,表示芯片型号为W25Q128 	 

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


/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = "0123456789";
#define countof(a)      (sizeof(a) / sizeof(*(a)))
int  BufferSize = 10;
uint8_t Rx_Buffer[11];
TestStatus TransferStatus1 = FAILED;
/* Private typedef -----------------------------------------------------------*/
//#define SPI_FLASH_PageSize      4096
#define SPI_FLASH_PageSize      256
#define SPI_FLASH_PerWritePageSize      256

SPI25QXX::SPI25QXX()
{
	
}
void SPI25QXX::SetSpi(Spi *spi)
{
	this->_spi = spi;
}
/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI25QXX::Init(void)
{	
	this->_spi->Stop();
}
/*******************************************************************************
* Function Name  : SPI_FLASH_SectorErase
* Description    : Erases the specified FLASH sector.
* Input          : SectorAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI25QXX::SectorErase(uint32_t SectorAddr)
{
	/* Send write enable instruction */
	WriteEnable();
	WaitForWriteEnd();
	/* Sector Erase */
	/* Select the FLASH: Chip Select low */
	this->_spi->Start();
	this->_spi->Start();
	/* Send Sector Erase instruction */
	this->_spi->Write(W25X_SectorErase);
	/* Send SectorAddr high nibble address byte */
	this->_spi->Write((SectorAddr & 0xFF0000) >> 16);
	/* Send SectorAddr medium nibble address byte */
	this->_spi->Write((SectorAddr & 0xFF00) >> 8);
	/* Send SectorAddr low nibble address byte */
	this->_spi->Write(SectorAddr & 0xFF);
	/* Deselect the FLASH: Chip Select high */
	this->_spi->Stop();
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
void SPI25QXX::BulkErase(void)
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
void SPI25QXX::PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	/* Enable the write access to the FLASH */
	WriteEnable();

	/* Select the FLASH: Chip Select low */
	this->_spi->Start();
	/* Send "Write to Memory " instruction */
	this->_spi->Write(W25X_PageProgram);
	/* Send WriteAddr high nibble address byte to write to */
	this->_spi->Write((WriteAddr & 0xFF0000) >> 16);
	/* Send WriteAddr medium nibble address byte to write to */
	this->_spi->Write((WriteAddr & 0xFF00) >> 8);
	/* Send WriteAddr low nibble address byte to write to */
	this->_spi->Write(WriteAddr & 0xFF);

	if (NumByteToWrite > SPI_FLASH_PerWritePageSize)
	{
		NumByteToWrite = SPI_FLASH_PerWritePageSize;
		//printf("\n\r Err: SPI_FLASH_PageWrite too large!");
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
void SPI25QXX::BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

	Addr = WriteAddr % SPI_FLASH_PageSize;
	count = SPI_FLASH_PageSize - Addr;
	NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
	NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

	if (Addr == 0) /* WriteAddr is SPI_FLASH_PageSize aligned  */
	{
		if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
		{
			PageWrite(pBuffer, WriteAddr, NumByteToWrite);
		}
		else /* NumByteToWrite > SPI_FLASH_PageSize */
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
	else /* WriteAddr is not SPI_FLASH_PageSize aligned  */
	{
		if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
		{
			if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
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
		else /* NumByteToWrite > SPI_FLASH_PageSize */
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
//void SPI25QXX::SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
//{
//  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, i;
//	uint32_t CurrentAddr;

//	
//	CurrentAddr = WriteAddr;
//	
//  Addr = WriteAddr % SPI_FLASH_PageSize;
//  count = SPI_FLASH_PageSize - Addr;
//	
//	NumOfPage = ( NumByteToWrite - count ) / SPI_FLASH_PageSize;
//	NumOfSingle = ( NumByteToWrite - count ) % SPI_FLASH_PageSize;
//	

//	if ( count )
//	{
//		SPI_FLASH_PageWrite ( pBuffer, CurrentAddr, count );
//		
//		CurrentAddr += count;
//		pBuffer += count;
//		
//	}
//	
//	for ( i = 0; i < NumOfPage; i ++ )
//	{
//		SPI_FLASH_PageWrite ( pBuffer, CurrentAddr, SPI_FLASH_PageSize );
//		
//		CurrentAddr += SPI_FLASH_PageSize;
//		pBuffer += SPI_FLASH_PageSize;
//		
//	}
//	
//	if ( NumOfSingle )
//		SPI_FLASH_PageWrite ( pBuffer, CurrentAddr, NumOfSingle );

//}

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
void SPI25QXX::BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	/* Select the FLASH: Chip Select low */
	this->_spi->Start();

	/* Send "Read from Memory " instruction */
	this->_spi->Write(W25X_ReadData);

	/* Send ReadAddr high nibble address byte to read from */
	this->_spi->Write((ReadAddr & 0xFF0000) >> 16);
	/* Send ReadAddr medium nibble address byte to read from */
	this->_spi->Write((ReadAddr & 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte to read from */
	this->_spi->Write(ReadAddr & 0xFF);

	while (NumByteToRead--) /* while there is data to be read */
	{
		/* Read a byte from the FLASH */
		*pBuffer = this->_spi->Read();
		/* Point to the next location where the byte read will be saved */
		pBuffer++;
	}

	/* Deselect the FLASH: Chip Select high */
	this->_spi->Stop();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
uint32_t SPI25QXX::ReadID(void)
{
	uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

	/* Select the FLASH: Chip Select low */
	this->_spi->Start();

	/* Send "RDID " instruction */
	this->_spi->Write(W25X_JedecDeviceID);

	/* Read a byte from the FLASH */
	Temp0 = this->_spi->Read();

	/* Read a byte from the FLASH */
	Temp1 = this->_spi->Read();

	/* Read a byte from the FLASH */
	Temp2 = this->_spi->Read();

	/* Deselect the FLASH: Chip Select high */
	this->_spi->Stop();

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
uint32_t SPI25QXX::ReadDeviceID(void)
{
	uint32_t Temp = 0;

	/* Select the FLASH: Chip Select low */
	this->_spi->Start();

	/* Send "RDID " instruction */
	this->_spi->Write(W25X_DeviceID);
	this->_spi->Read();
	this->_spi->Read();
	this->_spi->Read();

	/* Read a byte from the FLASH */
	Temp = this->_spi->Read();

	/* Deselect the FLASH: Chip Select high */
	this->_spi->Stop();

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
void SPI25QXX::StartReadSequence(uint32_t ReadAddr)
{
	/* Select the FLASH: Chip Select low */
	this->_spi->Start();

	/* Send "Read from Memory " instruction */
	this->_spi->Write(W25X_ReadData);

	/* Send the 24-bit address of the address to read from -----------------------*/
	/* Send ReadAddr high nibble address byte */
	this->_spi->Write((ReadAddr & 0xFF0000) >> 16);
	/* Send ReadAddr medium nibble address byte */
	this->_spi->Write((ReadAddr & 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte */
	this->_spi->Write(ReadAddr & 0xFF);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WriteEnable
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI25QXX::WriteEnable(void)
{
	/* Select the FLASH: Chip Select low */
	this->_spi->Start();

	/* Send "Write Enable" instruction */
	this->_spi->Write(W25X_WriteEnable);

	/* Deselect the FLASH: Chip Select high */
	this->_spi->Stop();
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
void SPI25QXX::WaitForWriteEnd(void)
{
	uint8_t FLASH_Status = 0;

	/* Select the FLASH: Chip Select low */
	this->_spi->Start();

	/* Send "Read Status Register" instruction */
	this->_spi->Write(W25X_ReadStatusReg);

	/* Loop as long as the memory is busy with a write cycle */
	do
	{
		/* Send a dummy byte to generate the clock needed by the FLASH
		and put the value of the status register in FLASH_Status variable */
		FLASH_Status = this->_spi->Read();
	} while ((FLASH_Status & WIP_Flag) != 0); /* Write in progress */

	/* Deselect the FLASH: Chip Select high */
	this->_spi->Stop();
}


//进入掉电模式
void SPI25QXX::PowerDown(void)
{
	/* Select the FLASH: Chip Select low */
	this->_spi->Start();

	/* Send "Power Down" instruction */
	this->_spi->Write(W25X_PowerDown);

	/* Deselect the FLASH: Chip Select high */
	this->_spi->Stop();
}

//唤醒
void SPI25QXX::WAKEUP(void)
{
	/* Select the FLASH: Chip Select low */
	this->_spi->Start();

	/* Send "Power Down" instruction */
	this->_spi->Write(W25X_ReleasePowerDown);

	/* Deselect the FLASH: Chip Select high */
	this->_spi->Stop();                  //等待TRES1
}

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
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
	while (BufferLength--)
	{
		if (*pBuffer1 != *pBuffer2)
		{
			return FAILED;
		}

		pBuffer1++;
		pBuffer2++;
	}
	return PASSED;
}
void SPI25QXX::Test()
{
	debug_printf("\r\n 8Mbyte flash(W25Q64)Test\r\n");
	/* 8M串行flash W25Q64初始化 */
	
	/* Get SPI Flash Device ID */
	DeviceID = ReadDeviceID();

	delay(200);

	/* Get SPI Flash ID */
	FlashID = ReadID();

	debug_printf("FlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);

	/* Check the SPI Flash ID */
	if (FlashID == sFLASH_ID)  /* #define  sFLASH_ID  0XEF4017 */
	{
		debug_printf("winbond flash W25Q64!\r\n");

		/* Erase SPI FLASH Sector to write on */
		SectorErase(FLASH_SectorToErase);

		/* 将发送缓冲区的数据写到flash中 */
		BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
		BufferWrite(Tx_Buffer, 252, BufferSize);
		debug_printf("Write Data is：%s\r\t", Tx_Buffer);

		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		debug_printf("Read data is：%s\r\n", Rx_Buffer);

		/* 检查写入的数据与读出的数据是否相等 */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

		if (PASSED == TransferStatus1)
		{
			debug_printf("8M flash(W25Q64)Test Success!\n\r");
		}
		else
		{
			debug_printf("8M flash(W25Q64)test fail!\n\r");
		}
	}// if (FlashID == sFLASH_ID)
	else
	{
		debug_printf("can not get W25Q64 ID!\n\r");
	}
	PowerDown();
}




/////////////////////////////////////////////////////////////
//////////////////以下为历史




















