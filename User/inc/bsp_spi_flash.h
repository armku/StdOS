#pragma once

#include "stm32f10x.h"
#include "Spi.h"


/************************** SPI Flash 连接引脚定义********************************/
#define macSPIx                                     SPI1
#define macSPI_APBxClock_FUN                        RCC_APB2PeriphClockCmd
#define macSPI_CLK                                  RCC_APB2Periph_SPI1

#define macSPI_CS_APBxClock_FUN                     RCC_APB2PeriphClockCmd
#define macSPI_CS_CLK                               RCC_APB2Periph_GPIOA    
#define macSPI_CS_PORT                              GPIOA
#define macSPI_CS_PIN                               GPIO_Pin_4

/************************** SPI Flash 函数宏定义********************************/
#define macSPI_FLASH_CS_ENABLE()                       GPIO_ResetBits( macSPI_CS_PORT, macSPI_CS_PIN )

class W25QXXX
{
    public:
		W25QXXX(Spi * spi);
	
		Spi * pSpi;
		OutputPort *pcs;//片选脚
	
        void Init(void);
        void SectorErase(uint SectorAddr);
        void BulkErase(void);
        void PageWrite(byte *pBuffer, uint WriteAddr, ushort NumByteToWrite);
        void BufferWrite(byte *pBuffer, uint WriteAddr, ushort NumByteToWrite);
        void BufferRead(byte *pBuffer, uint ReadAddr, ushort NumByteToRead);
        uint ReadID(void);
        uint ReadDeviceID(void);
        void StartReadSequence(uint ReadAddr);
        void PowerDown(void);
        void WAKEUP(void);
                    
        void WriteEnable(void);
        void WaitForWriteEnd(void);
};
