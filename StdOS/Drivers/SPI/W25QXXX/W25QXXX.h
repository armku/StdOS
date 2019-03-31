#ifndef _W25QXXX_H
    #define _W25QXXX_H

    #include "Kernel\Sys.h"
    #include "OnChip\Spi.h"

	#define W25QXXXUSESPISOFT	1
    //W25Xϵ��/Qϵ��оƬ�б�	   
    typedef enum
    {
        W25QXXX80 = 0XEF13, W25QXXX16 = 0XEF14, W25QXXX32 = 0XEF15, W25QXXX64 = 0XEF16, W25QXXX128 = 0XEF17
    } W25QXXXType;

    // W25Q64�洢Flash
    class W25Q64
    {
        private:
        public:
			#if W25QXXXUSESPISOFT
				SpiSoft *_spi; // �ڲ�Spi����
			#else
				Spi *_spi; // �ڲ�Spi����
			#endif

            // �ȴ��������
            bool WaitForEnd();

            // ��ȡ���
            uint32_t ReadID();
            void WriteEnable(void); //д����
            void WriteDisable(void); //д����
        public:
            uint32_t ID; // оƬID
            uint32_t DeviceID; // �豸ID
            uint16_t PageSize; // ҳ��С
            uint16_t Retry; // �ȴ����Դ���
			#if W25QXXXUSESPISOFT
				W25Q64(SpiSoft *spi);
			#else
				W25Q64(Spi *spi);
			#endif
            // ��������
            bool EraseSector(uint32_t sector);
            // ����ҳ
            bool ErasePage(uint32_t pageAddr);

            // д��һҳ
            bool WritePage(uint32_t addr, uint8_t *buf, uint32_t count);
            // д������
            bool Write(uint32_t addr, uint8_t *buf, uint32_t count);
            // ��ȡ����
            bool Read(uint32_t addr, uint8_t *buf, uint32_t count);

            uint32_t ReadDeviceID(void); //Reads FLASH identification.
            //Erases the entire FLASH.
            void EraseChip(void); ////��������оƬ �ȴ�ʱ�䳬��...
            void WakeUp(void); //����
            void PowerDown(void); //�������ģʽ	
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
		Spi *_spi; // �ڲ�Spi����
	};
#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
	//#define  sFLASH_ID              0xEF3015     //W25X16
	//#define  sFLASH_ID              0xEF4015	 //W25Q16
#define  sFLASH_ID              0XEF4017    //W25Q64
	
#endif
