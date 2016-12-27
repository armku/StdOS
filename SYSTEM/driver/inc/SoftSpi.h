#pragma once
#include "BasePort.h"

class CSoftSpi
{
public:
    CSoftSpi(PinPort pincs, PinPort pinsck, PinPort pindi, PinPort pindo, uint32_t nus = 0); //��ʱʱ��Ĭ��0us   
    byte Init();
    byte WaitBusy();
    byte spi_writebyte(byte da);
    byte spi_readbyte(void);
public:
    BasePort *portsck;
    BasePort *portdi;
    BasePort *portdo;
public:
    BasePort *portcs;
private:
    uint32_t delayus;//��ʱʱ��
};
