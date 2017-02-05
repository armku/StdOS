#pragma once

#include "Port.h"
#include "InputPort.h"

class CSoftSpi
{
public:
    CSoftSpi(Pin pincs, Pin pinsck, Pin pindi, Pin pindo, uint nus = 0); //��ʱʱ��Ĭ��0us   
    byte Init();
    byte WaitBusy();
    byte spi_writebyte(byte da);
    byte spi_readbyte(void);
public:
    OutputPort *pportsck;
    OutputPort *pportdi;
    InputPort *pportdo;
    OutputPort *pportcs;
private:
    uint delayus;//��ʱʱ��
};
