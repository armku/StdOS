#pragma once
#include "Port.h"

class CSoftSpi
{
public:
    CSoftSpi(Pin pincs, Pin pinsck, Pin pindi, Pin pindo, uint nus = 0); //延时时间默认0us   
    byte Init();
    byte WaitBusy();
    byte spi_writebyte(byte da);
    byte spi_readbyte(void);
public:
    Port *portsck;
    Port *portdi;
    Port *portdo;
public:
    Port *portcs;
private:
    uint delayus;//延时时间
};
