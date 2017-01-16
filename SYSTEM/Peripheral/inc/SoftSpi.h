#pragma once

#include "OutputPort.h"
#include "InputPortNew.h"

class CSoftSpi
{
public:
    CSoftSpi(Pin pincs, Pin pinsck, Pin pindi, Pin pindo, uint nus = 0); //��ʱʱ��Ĭ��0us   
    byte Init();
    byte WaitBusy();
    byte spi_writebyte(byte da);
    byte spi_readbyte(void);
public:
    OutputPort portsck;
    OutputPort portdi;
    InputPortNew portdo;
    OutputPort portcs;
private:
    uint delayus;//��ʱʱ��
};
