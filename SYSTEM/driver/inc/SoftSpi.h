#pragma once
#include "BasePort.h"

class CSoftSpi
{
public:
    CSoftSpi(PinPort pincs, PinPort pinsck, PinPort pindi, PinPort pindo, uint32_t nus = 0); //延时时间默认0us   
    uint8_t Init();
    uint8_t WaitBusy();
    uint8_t spi_writebyte(uint8_t da);
    uint8_t spi_readbyte(void);
public:
    BasePort *portsck;
    BasePort *portdi;
    BasePort *portdo;
public:
    BasePort *portcs;
private:
    uint32_t delayus;//延时时间
};
