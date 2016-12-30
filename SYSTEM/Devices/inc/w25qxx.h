#pragma once

#include "Type.h"
#include "BasePort.h"
#include "HardSpi.h"


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
        void BufferWrite(byte *pBuffer, uint32_t WriteAddr, ushort NumByteToWrite);
        void BufferRead(byte *pBuffer, uint32_t ReadAddr, ushort NumByteToRead);
    private:
        void WAKEUP(void);
        void WaitForWriteEnd(void);
        void WriteEnable(void);               
        void StartReadSequence(uint32_t ReadAddr);
        void PageWrite(byte *pBuffer, uint32_t WriteAddr, ushort NumByteToWrite);
        void BulkErase(void);
	private:
		BasePort *pincs;
		BasePort *pinclk;
		BasePort *pinmiso;
		BasePort *pinmosi;
		CHardSpi *spi;
};
