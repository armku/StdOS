#pragma once

#include "Spi.h"

class W25QXXX
{
    public:
		W25QXXX(Spi * spi,Pin pincs=P0);
	
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
	private:
		Spi * pSpi;
		OutputPort *pcs;//Æ¬Ñ¡½Å
};
