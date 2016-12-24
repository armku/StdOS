#pragma once
#include "Port.h"

class CSpiSoft
{
public:
    CSpiSoft(PinPort pincs, PinPort pinsck, PinPort pindi, PinPort pindo, uint32_t nus = 0); //延时时间默认0us
    ~CSpiSoft();
    uint8_t Init();
    uint8_t WaitBusy();
    uint8_t spi_writebyte(uint8_t da);
    uint8_t spi_readbyte(void);
public:
    CPort *portsck;
    CPort *portdi;
    CPort *portdo;
public:
    CPort *portcs;
private:
    uint32_t delayus;//延时时间
};
