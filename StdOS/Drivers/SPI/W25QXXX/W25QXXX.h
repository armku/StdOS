#ifndef _W25QXXX_H
    #define _W25QXXX_H

    #include "Kernel\Sys.h"
    #include "OnChip\Spi.h"

    //W25X系列/Q系列芯片列表	   
    typedef enum
    {
        W25QXXX80 = 0XEF13, W25QXXX16 = 0XEF14, W25QXXX32 = 0XEF15, W25QXXX64 = 0XEF16, W25QXXX128 = 0XEF17
    } W25QXXXType;
	    
	typedef enum { FAILED = 0, PASSED = !FAILED } TestStatus;
	class SPI25QXX
	{
	public:
		SPI25QXX();
		void SetSpi(Spi *spi);
		void SectorErase(uint32_t SectorAddr);
		void BulkErase(void);
		void PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
		void BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
		void BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
		uint32_t ReadID(void);
		uint32_t ReadDeviceID(void);
		void StartReadSequence(uint32_t ReadAddr);
		void PowerDown(void);
		void WAKEUP(void);

		void WriteEnable(void);
		void WaitForWriteEnd(void);

		void Test();
	public:
		uint32_t DeviceID;
		uint32_t FlashID;
		Spi *_spi; // 内部Spi对象
	};
#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
	//#define  sFLASH_ID              0xEF3015     //W25X16
	//#define  sFLASH_ID              0xEF4015	 //W25Q16
#define  sFLASH_ID              0XEF4017    //W25Q64
	
#endif
