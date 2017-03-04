#pragma once

#include "Spi.h"

class W25QXXX
{
    public:
		W25QXXX(Spi * spi,Pin pincs=P0);
	
        void Init(void);
        void SectorErase(uint SectorAddr);
        void BulkErase(void);
        void PageWrite(byte *pBuffer, uint WriteAddr, int NumByteToWrite);
        void Write(byte *pBuffer, uint WriteAddr, int NumByteToWrite);
        void Read(byte *pBuffer, uint ReadAddr, int NumByteToRead);
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
