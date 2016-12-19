#pragma once

#include <stdint.h>
#include "Port.h"
#include "SpiHard.h"


class CW25Qxxx
{
    public:
        CW25Qxxx(uint16_t pinscs,uint16_t pinsclk=PA5,uint16_t pinsmiso=PA6,uint16_t pinsmosi=PA7,ESpiChannel spichannel=CHSPI1);
        ~CW25Qxxx();
    public:
        void Init();
        uint32_t ReadDeviceID(void);
        uint32_t ReadID(void);
        void PowerDown(void);
        void SectorErase(uint32_t SectorAddr);
        void BufferWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
        void BufferRead(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
    private:
        void WAKEUP(void);
        void WaitForWriteEnd(void);
        void WriteEnable(void);               
        void StartReadSequence(uint32_t ReadAddr);
        void PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
        void BulkErase(void);
	private:
		CPort *pincs;
		CPort *pinclk;
		CPort *pinmiso;
		CPort *pinmosi;
		CSpiHard *spi;
};
