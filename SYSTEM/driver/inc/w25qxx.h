#pragma once

#include <stdint.h>
#include "BasePort.h"
#include "SpiHard.h"


class CW25Qxxx
{
    public:
        CW25Qxxx(PinPort pinscs,PinPort pinsclk=PA5,PinPort pinsmiso=PA6,PinPort pinsmosi=PA7,ESpiChannel spichannel=CHSPI1);
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
		BasePort *pincs;
		BasePort *pinclk;
		BasePort *pinmiso;
		BasePort *pinmosi;
		CSpiHard *spi;
};
