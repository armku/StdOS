#ifndef _W25QXXX_H
    #define _W25QXXX_H

    #include "Kernel\Sys.h"
    #include "OnChip\Spi.h"

	#define W25QXXXUSESPISOFT	1
    //W25X系列/Q系列芯片列表	   
    typedef enum
    {
        W25QXXX80 = 0XEF13, W25QXXX16 = 0XEF14, W25QXXX32 = 0XEF15, W25QXXX64 = 0XEF16, W25QXXX128 = 0XEF17
    } W25QXXXType;

    // W25Q64存储Flash
    class W25Q64
    {
        private:
        public:
			#if W25QXXXUSESPISOFT
				SpiSoft *_spi; // 内部Spi对象
			#else
				Spi *_spi; // 内部Spi对象
			#endif

            // 等待操作完成
            bool WaitForEnd();

            // 读取编号
            uint32_t ReadID();
            void WriteEnable(void); //写允许
            void WriteDisable(void); //写保护
        public:
            uint32_t ID; // 芯片ID
            uint32_t DeviceID; // 设备ID
            uint16_t PageSize; // 页大小
            uint16_t Retry; // 等待重试次数
			#if W25QXXXUSESPISOFT
				W25Q64(SpiSoft *spi);
			#else
				W25Q64(Spi *spi);
			#endif
            // 擦除扇区
            bool EraseSector(uint32_t sector);
            // 擦除页
            bool ErasePage(uint32_t pageAddr);

            // 写入一页
            bool WritePage(uint32_t addr, uint8_t *buf, uint32_t count);
            // 写入数据
            bool Write(uint32_t addr, uint8_t *buf, uint32_t count);
            // 读取数据
            bool Read(uint32_t addr, uint8_t *buf, uint32_t count);

            uint32_t ReadDeviceID(void); //Reads FLASH identification.
            //Erases the entire FLASH.
            void EraseChip(void); ////擦除整个芯片 等待时间超长...
            void WakeUp(void); //唤醒
            void PowerDown(void); //进入掉电模式	
    };  


	typedef enum { FAILED = 0, PASSED = !FAILED } TestStatus;
	class SPI25QXX
	{
	public:
		SPI25QXX(Spi *spi);
		void Init(void);
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

		/*uint8_t ReadByte(void);*/
		/*uint8_t SendByte(uint8_t byte);*/
		uint16_t SendHalfWord(uint16_t HalfWord);
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
