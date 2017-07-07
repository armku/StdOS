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
// 设置操作地址
void W25Q64::SetAddr(uint addr)
{
	
}
// 等待操作完成
bool W25Q64::WaitForEnd()
{
	
	return true;
}

// 读取编号
uint W25Q64::ReadID()
{
	return 0;
}

W25Q64::W25Q64(Spi* spi)
{
	this->_spi=spi;
}
W25Q64::~W25Q64()
{
	
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
bool W25Q64::WritePage(uint addr, byte* buf, uint count)
{
	return true;
}
// 读取一页
bool W25Q64::ReadPage(uint addr, byte* buf, uint count)
{
	return true;
}
// 写入数据
bool W25Q64::Write(uint addr, byte* buf, uint count)
{
	return true;
}
// 读取数据
bool W25Q64::Read(uint addr, byte* buf, uint count)
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
uint W25Q64::SPI_FLASH_ReadDeviceID(void)
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

  return Temp;
}
/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
uint W25Q64::SPI_FLASH_ReadID(void)
{
  uint Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* Select the FLASH: Chip Select low */
  this->_spi->Start();

  /* Send "RDID " instruction */
  this->_spi->Write(W25X_JedecDeviceID);

  /* Read a byte from the FLASH */
  Temp0 = this->_spi->Write(Dummy_Byte);

  /* Read a byte from the FLASH */
  Temp1 = this->_spi->Write(Dummy_Byte);

  /* Read a byte from the FLASH */
  Temp2 = this->_spi->Write(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  this->_spi->Stop();

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}








Spi spi(Spi1);
W25Q64 w25q64(&spi);

void SPI_FLASH_SectorErase(uint SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(byte* pBuffer, uint WriteAddr, ushort NumByteToWrite);
void SPI_FLASH_BufferWrite(byte* pBuffer, uint WriteAddr, ushort NumByteToWrite);
void SPI_FLASH_BufferRead(byte* pBuffer, uint ReadAddr, ushort NumByteToRead);
void SPI_FLASH_StartReadSequence(uint ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);

void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);



/*******************************************************************************
* Function Name  : SPI_FLASH_SectorErase
* Description    : Erases the specified FLASH sector.
* Input          : SectorAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_SectorErase(uint SectorAddr)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();
  SPI_FLASH_WaitForWriteEnd();
  /* Sector Erase */
  /* Select the FLASH: Chip Select low */
  spi.Start();
  /* Send Sector Erase instruction */
  spi.Write(W25X_SectorErase);
  /* Send SectorAddr high nibble address byte */
  spi.Write((SectorAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  spi.Write((SectorAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  spi.Write(SectorAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  spi.Stop();
  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BulkErase
* Description    : Erases the entire FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BulkErase(void)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* Bulk Erase */
  /* Select the FLASH: Chip Select low */
  spi.Start();
  /* Send Bulk Erase instruction  */
  spi.Write(W25X_ChipErase);
  /* Deselect the FLASH: Chip Select high */
  spi.Stop();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
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
void SPI_FLASH_PageWrite(byte* pBuffer, uint WriteAddr, ushort NumByteToWrite)
{
  /* Enable the write access to the FLASH */
  SPI_FLASH_WriteEnable();

  /* Select the FLASH: Chip Select low */
  spi.Start();
  /* Send "Write to Memory " instruction */
  spi.Write(W25X_PageProgram);
  /* Send WriteAddr high nibble address byte to write to */
  spi.Write((WriteAddr & 0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */
  spi.Write((WriteAddr & 0xFF00) >> 8);
  /* Send WriteAddr low nibble address byte to write to */
  spi.Write(WriteAddr & 0xFF);

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     //printf("\n\r Err: SPI_FLASH_PageWrite too large!");
  }

  /* while there is data to be written on the FLASH */
  while (NumByteToWrite--)
  {
    /* Send the current byte */
    spi.Write(*pBuffer);
    /* Point on the next byte to be written */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  spi.Stop();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
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
void SPI_FLASH_BufferWrite(byte* pBuffer, uint WriteAddr, ushort NumByteToWrite)
{
  byte NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % SPI_FLASH_PageSize;
  count = SPI_FLASH_PageSize - Addr;
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

  if (Addr == 0) /* WriteAddr is SPI_FLASH_PageSize aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /* WriteAddr is not SPI_FLASH_PageSize aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
      {
        temp = NumOfSingle - count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}
//void SPI_FLASH_BufferWrite(byte* pBuffer, uint WriteAddr, ushort NumByteToWrite)
//{
//  byte NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, i;
//	uint CurrentAddr;

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
void SPI_FLASH_BufferRead(byte* pBuffer, uint ReadAddr, ushort NumByteToRead)
{
  /* Select the FLASH: Chip Select low */
  spi.Start();

  /* Send "Read from Memory " instruction */
  spi.Write(W25X_ReadData);

  /* Send ReadAddr high nibble address byte to read from */
  spi.Write((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  spi.Write((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  spi.Write(ReadAddr & 0xFF);

  while (NumByteToRead--) /* while there is data to be read */
  {
    /* Read a byte from the FLASH */
    *pBuffer = spi.Write(Dummy_Byte);
    /* Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  spi.Stop();
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
void SPI_FLASH_StartReadSequence(uint ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  spi.Start();

  /* Send "Read from Memory " instruction */
  spi.Write(W25X_ReadData);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  spi.Write((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  spi.Write((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  spi.Write(ReadAddr & 0xFF);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WriteEnable
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WriteEnable(void)
{
  /* Select the FLASH: Chip Select low */
  spi.Start();

  /* Send "Write Enable" instruction */
  spi.Write(W25X_WriteEnable);

  /* Deselect the FLASH: Chip Select high */
  spi.Stop();
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
void SPI_FLASH_WaitForWriteEnd(void)
{
  byte FLASH_Status = 0;

  /* Select the FLASH: Chip Select low */
  spi.Start();

  /* Send "Read Status Register" instruction */
  spi.Write(W25X_ReadStatusReg);

  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = spi.Write(Dummy_Byte);	 
  }
  while ((FLASH_Status & WIP_Flag) == SET); /* Write in progress */

  /* Deselect the FLASH: Chip Select high */
  spi.Stop();
}


//进入掉电模式
void SPI_Flash_PowerDown(void)   
{ 
  /* Select the FLASH: Chip Select low */
  spi.Start();

  /* Send "Power Down" instruction */
  spi.Write(W25X_PowerDown);

  /* Deselect the FLASH: Chip Select high */
  spi.Stop();
}   

//唤醒
void SPI_Flash_WAKEUP(void)   
{
  /* Select the FLASH: Chip Select low */
  spi.Start();

  /* Send "Power Down" instruction */
  spi.Write(W25X_ReleasePowerDown);

  /* Deselect the FLASH: Chip Select high */
  spi.Stop();                   //等待TRES1
}   



typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;


/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
//#define  sFLASH_ID              0xEF3015     //W25X16
//#define  sFLASH_ID              0xEF4015	 //W25Q16
#define  sFLASH_ID              0XEF4017    //W25Q64
     

/* 发送缓冲区初始化 */
byte Tx_Buffer[] = " 感谢您选用野火stm32开发板\r\n                http://firestm32.taobao.com";
byte Rx_Buffer[BufferSize];

__IO uint DeviceID = 0;
__IO uint FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;


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
TestStatus Buffercmp(byte* pBuffer1, byte* pBuffer2, ushort BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
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
	printf("\r\n 这是一个8Mbyte串行flash(W25Q64)实验 \r\n");
	
	spi.Open();
	/* Get SPI Flash Device ID */
	DeviceID = w25q64.SPI_FLASH_ReadDeviceID();
	
	Sys.Delay(10);
	
	/* Get SPI Flash ID */
	FlashID = w25q64.SPI_FLASH_ReadID();
	
	printf("\r\n FlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* Check the SPI Flash ID */
	if (FlashID == sFLASH_ID)  /* #define  sFLASH_ID  0XEF4017 */
	{	
		printf("\r\n 检测到华邦串行flash W25Q64 !\r\n");
		
		/* Erase SPI FLASH Sector to write on */
		SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 
		
		/* 将发送缓冲区的数据写到flash中 */ 	
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
		SPI_FLASH_BufferWrite(Tx_Buffer, 252, BufferSize);
		printf("\r\n 写入的数据为：%s \r\t", Tx_Buffer);
		
		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		printf("\r\n 读出的数据为：%s \r\n", Rx_Buffer);
		
		/* 检查写入的数据与读出的数据是否相等 */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( PASSED == TransferStatus1 )
		{    
			printf("\r\n 8M串行flash(W25Q64)测试成功!\n\r");
		}
		else
		{        
			printf("\r\n 8M串行flash(W25Q64)测试失败!\n\r");
		}
	}// if (FlashID == sFLASH_ID)
	else
	{    
		printf("\r\n 获取不到 W25Q64 ID!\n\r");
	}
	
	SPI_Flash_PowerDown();  
	printf("\r\n\n\r");
}






































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
