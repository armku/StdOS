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
	class SPI25QXX
	{
	public:
		void SPI_FLASH_Init(void);
		void SPI_FLASH_SectorErase(uint32_t SectorAddr);
		void SPI_FLASH_BulkErase(void);
		void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
		void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
		void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
		uint32_t SPI_FLASH_ReadID(void);
		uint32_t SPI_FLASH_ReadDeviceID(void);
		void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
		void SPI_Flash_PowerDown(void);
		void SPI_Flash_WAKEUP(void);

		uint8_t SPI_FLASH_ReadByte(void);
		uint8_t SPI_FLASH_SendByte(uint8_t byte);
		uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);
		void SPI_FLASH_WriteEnable(void);
		void SPI_FLASH_WaitForWriteEnd(void);
	};
#endif
