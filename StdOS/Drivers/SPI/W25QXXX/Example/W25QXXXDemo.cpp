#include "../W25QXXX.h"

#define _W25QXX_TEST_CPP

#ifdef _W25QXX_TEST_CPP
/************************** SPI Flash 函数宏定义********************************/

SPI25QXX s25xx;
extern int  BufferSize;

/* 发送缓冲区初始化 */
extern uint8_t Tx_Buffer[] ;
extern		uint8_t Rx_Buffer[];

extern TestStatus TransferStatus1 ;
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

    void w25qxxxtest()
    {       
		debug_printf("\r\n 8Mbyte flash(W25Q64)Test\r\n");

		/* 8M串行flash W25Q64初始化 */
		s25xx.SPI_FLASH_Init();

		/* Get SPI Flash Device ID */
		s25xx.DeviceID = s25xx.SPI_FLASH_ReadDeviceID();

		 delay(200);

		/* Get SPI Flash ID */
		 s25xx.FlashID = s25xx.SPI_FLASH_ReadID();

		debug_printf("\r\n FlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", s25xx.FlashID, s25xx.DeviceID);

		/* Check the SPI Flash ID */
		if (s25xx.FlashID == sFLASH_ID)  /* #define  sFLASH_ID  0XEF4017 */
		{
			debug_printf("\r\n winbond flash W25Q64 !\r\n");

			/* Erase SPI FLASH Sector to write on */
			s25xx.SPI_FLASH_SectorErase(FLASH_SectorToErase);

			/* 将发送缓冲区的数据写到flash中 */
			s25xx.SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
			s25xx.SPI_FLASH_BufferWrite(Tx_Buffer, 252, BufferSize);
			debug_printf("\r\n Write Data is：%s \r\t", Tx_Buffer);

			/* 将刚刚写入的数据读出来放到接收缓冲区中 */
			s25xx.SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
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

		s25xx.SPI_Flash_PowerDown();
        debug_printf("\r\n\r\n");
    }
#endif
