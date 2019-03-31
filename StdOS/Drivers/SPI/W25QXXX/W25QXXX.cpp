#include "W25QXXX.h"
#include "stm32f10x.h"

#define      macSPI_FLASH_CS_ENABLE()                       GPIO_ResetBits( GPIOA, GPIO_Pin_4 )
#define      macSPI_FLASH_CS_DISABLE()                      GPIO_SetBits( GPIOA, GPIO_Pin_4 )
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

SPI25QXX::SPI25QXX(Spi *spi)
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
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;


	/* Enable SPI1 and GPIO clocks */
	/*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO,
		 SPI_FLASH_SPI_MISO_GPIO, SPI_FLASH_SPI_DETECT_GPIO
		 and SPI_FLASH_SPI_SCK_GPIO Periph clock enable */
		 /*!< SPI_FLASH_SPI Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	/*!< Configure SPI_FLASH_SPI_CS_PIN pin: SPI_FLASH Card CS pin */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*!< Configure SPI_FLASH_SPI pins: SCK */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*!< Configure SPI_FLASH_SPI pins: MISO */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*!< Configure SPI_FLASH_SPI pins: MOSI */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Deselect the FLASH: Chip Select high */
	macSPI_FLASH_CS_DISABLE();

	/* SPI1 configuration */
	// W25X16: data input on the DIO pin is sampled on the rising edge of the CLK. 
	// Data on the DO and DIO pins are clocked out on the falling edge of CLK.
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE);

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
	macSPI_FLASH_CS_ENABLE();
	/* Send Sector Erase instruction */
	SendByte(W25X_SectorErase);
	/* Send SectorAddr high nibble address byte */
	SendByte((SectorAddr & 0xFF0000) >> 16);
	/* Send SectorAddr medium nibble address byte */
	SendByte((SectorAddr & 0xFF00) >> 8);
	/* Send SectorAddr low nibble address byte */
	SendByte(SectorAddr & 0xFF);
	/* Deselect the FLASH: Chip Select high */
	macSPI_FLASH_CS_DISABLE();
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
	macSPI_FLASH_CS_ENABLE();
	/* Send Bulk Erase instruction  */
	SendByte(W25X_ChipErase);
	/* Deselect the FLASH: Chip Select high */
	macSPI_FLASH_CS_DISABLE();

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
	macSPI_FLASH_CS_ENABLE();
	/* Send "Write to Memory " instruction */
	SendByte(W25X_PageProgram);
	/* Send WriteAddr high nibble address byte to write to */
	SendByte((WriteAddr & 0xFF0000) >> 16);
	/* Send WriteAddr medium nibble address byte to write to */
	SendByte((WriteAddr & 0xFF00) >> 8);
	/* Send WriteAddr low nibble address byte to write to */
	SendByte(WriteAddr & 0xFF);

	if (NumByteToWrite > SPI_FLASH_PerWritePageSize)
	{
		NumByteToWrite = SPI_FLASH_PerWritePageSize;
		//printf("\n\r Err: SPI_FLASH_PageWrite too large!");
	}

	/* while there is data to be written on the FLASH */
	while (NumByteToWrite--)
	{
		/* Send the current byte */
		SendByte(*pBuffer);
		/* Point on the next byte to be written */
		pBuffer++;
	}

	/* Deselect the FLASH: Chip Select high */
	macSPI_FLASH_CS_DISABLE();

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
	macSPI_FLASH_CS_ENABLE();

	/* Send "Read from Memory " instruction */
	SendByte(W25X_ReadData);

	/* Send ReadAddr high nibble address byte to read from */
	SendByte((ReadAddr & 0xFF0000) >> 16);
	/* Send ReadAddr medium nibble address byte to read from */
	SendByte((ReadAddr & 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte to read from */
	SendByte(ReadAddr & 0xFF);

	while (NumByteToRead--) /* while there is data to be read */
	{
		/* Read a byte from the FLASH */
		*pBuffer = SendByte(Dummy_Byte);
		/* Point to the next location where the byte read will be saved */
		pBuffer++;
	}

	/* Deselect the FLASH: Chip Select high */
	macSPI_FLASH_CS_DISABLE();
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
	macSPI_FLASH_CS_ENABLE();

	/* Send "RDID " instruction */
	SendByte(W25X_JedecDeviceID);

	/* Read a byte from the FLASH */
	Temp0 = SendByte(Dummy_Byte);

	/* Read a byte from the FLASH */
	Temp1 = SendByte(Dummy_Byte);

	/* Read a byte from the FLASH */
	Temp2 = SendByte(Dummy_Byte);

	/* Deselect the FLASH: Chip Select high */
	macSPI_FLASH_CS_DISABLE();

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
	macSPI_FLASH_CS_ENABLE();

	/* Send "RDID " instruction */
	SendByte(W25X_DeviceID);
	SendByte(Dummy_Byte);
	SendByte(Dummy_Byte);
	SendByte(Dummy_Byte);

	/* Read a byte from the FLASH */
	Temp = SendByte(Dummy_Byte);

	/* Deselect the FLASH: Chip Select high */
	macSPI_FLASH_CS_DISABLE();

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
	macSPI_FLASH_CS_ENABLE();

	/* Send "Read from Memory " instruction */
	SendByte(W25X_ReadData);

	/* Send the 24-bit address of the address to read from -----------------------*/
	/* Send ReadAddr high nibble address byte */
	SendByte((ReadAddr & 0xFF0000) >> 16);
	/* Send ReadAddr medium nibble address byte */
	SendByte((ReadAddr & 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte */
	SendByte(ReadAddr & 0xFF);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadByte
* Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
uint8_t SPI25QXX::ReadByte(void)
{
	return (SendByte(Dummy_Byte));
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
uint8_t SPI25QXX::SendByte(uint8_t byte)
{
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI1, byte);

	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI1);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendHalfWord
* Description    : Sends a Half Word through the SPI interface and return the
*                  Half Word received from the SPI bus.
* Input          : Half Word : Half Word to send.
* Output         : None
* Return         : The value of the received Half Word.
*******************************************************************************/
u16 SPI25QXX::SendHalfWord(uint16_t HalfWord)
{
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/* Send Half Word through the SPI1 peripheral */
	SPI_I2S_SendData(SPI1, HalfWord);

	/* Wait to receive a Half Word */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	/* Return the Half Word read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI1);
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
	macSPI_FLASH_CS_ENABLE();

	/* Send "Write Enable" instruction */
	SendByte(W25X_WriteEnable);

	/* Deselect the FLASH: Chip Select high */
	macSPI_FLASH_CS_DISABLE();
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
	macSPI_FLASH_CS_ENABLE();

	/* Send "Read Status Register" instruction */
	SendByte(W25X_ReadStatusReg);

	/* Loop as long as the memory is busy with a write cycle */
	do
	{
		/* Send a dummy byte to generate the clock needed by the FLASH
		and put the value of the status register in FLASH_Status variable */
		FLASH_Status = SendByte(Dummy_Byte);
	} while ((FLASH_Status & WIP_Flag) == SET); /* Write in progress */

	/* Deselect the FLASH: Chip Select high */
	macSPI_FLASH_CS_DISABLE();
}


//进入掉电模式
void SPI25QXX::PowerDown(void)
{
	/* Select the FLASH: Chip Select low */
	macSPI_FLASH_CS_ENABLE();

	/* Send "Power Down" instruction */
	SendByte(W25X_PowerDown);

	/* Deselect the FLASH: Chip Select high */
	macSPI_FLASH_CS_DISABLE();
}

//唤醒
void SPI25QXX::WAKEUP(void)
{
	/* Select the FLASH: Chip Select low */
	macSPI_FLASH_CS_ENABLE();

	/* Send "Power Down" instruction */
	SendByte(W25X_ReleasePowerDown);

	/* Deselect the FLASH: Chip Select high */
	macSPI_FLASH_CS_DISABLE();                   //等待TRES1
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
	Init();

	/* Get SPI Flash Device ID */
	DeviceID = ReadDeviceID();

	delay(200);

	/* Get SPI Flash ID */
	FlashID = ReadID();

	debug_printf("\r\n FlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);

	/* Check the SPI Flash ID */
	if (FlashID == sFLASH_ID)  /* #define  sFLASH_ID  0XEF4017 */
	{
		debug_printf("\r\n winbond flash W25Q64 !\r\n");

		/* Erase SPI FLASH Sector to write on */
		SectorErase(FLASH_SectorToErase);

		/* 将发送缓冲区的数据写到flash中 */
		BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
		BufferWrite(Tx_Buffer, 252, BufferSize);
		debug_printf("\r\n Write Data is：%s \r\t", Tx_Buffer);

		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		debug_printf("\r\n Read data is：%s \r\n", Rx_Buffer);

		/* 检查写入的数据与读出的数据是否相等 */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

		if (PASSED == TransferStatus1)
		{
			debug_printf("\r\n 8M flash(W25Q64)Test Success!\n\r");
		}
		else
		{
			debug_printf("\r\n 8M flash(W25Q64)test fail!\n\r");
		}
	}// if (FlashID == sFLASH_ID)
	else
	{
		debug_printf("\r\n can not get W25Q64 ID!\n\r");
	}

	PowerDown();
	debug_printf("\r\n\r\n");
}




/////////////////////////////////////////////////////////////
//////////////////以下为历史




















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

// 读取编号
uint32_t W25Q64::ReadID()
{
    uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

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

uint32_t W25Q64::ReadDeviceID(void)
{
    uint32_t Temp = 0;

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
bool W25Q64::EraseSector(uint32_t SectorAddr)
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
bool W25Q64::ErasePage(uint32_t pageAddr)
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
    uint8_t FLASH_Status = 0;
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

bool W25Q64::WritePage(uint32_t addr, uint8_t *pBuffer, uint32_t NumByteToWrite)
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
        //StdPrintf("\n\r Err: this->PageWrite too large!");
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
bool W25Q64::Write(uint32_t addr, uint8_t *pBuffer, uint32_t NumByteToWrite)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

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
bool W25Q64::Read(uint32_t ReadAddr, uint8_t *pBuffer, uint32_t NumByteToRead)
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
