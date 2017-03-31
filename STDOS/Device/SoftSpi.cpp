#include "SoftSpi.h"
#include "Sys.h"

CSoftSpi::CSoftSpi(Pin pincs, Pin pinsck, Pin pindi, Pin pindo, uint nus)
{
    this->pportcs=new OutputPort(pincs);
    this->pportsck=new OutputPort(pinsck);
    this->pportdi=new OutputPort(pindi);
    this->pportdo=new InputPort(pindo,true);
    this->delayus = nus;
}

byte CSoftSpi::Init()
{
    return 0;
}
/*---------------------------------------------------------
忙状态判断，最长等待时间，200 X 10 ms=2S
---------------------------------------------------------*/
byte CSoftSpi::WaitBusy()
{
    ushort i;
    *this->pportcs=0;
    i = 0;
    while (this->pportdo->Read() > 0)
    {
        Sys.Sleep(10);
        i++;
        if (i > 200)
            return 1;
    }
    *this->pportcs=1;
    return 0;
}

//SPI写字节
byte CSoftSpi::spi_writebyte(byte da)
{
    byte i;
    byte ret = 0;
    for (i = 0; i < 8; i++)
    {
        if (da & (1 << (8 - i - 1)))
        {
            *this->pportdi=1;
        }
        else
        {
            *this->pportdi=0;
        }
		Sys.Delay(this->delayus);
        *this->pportsck=1;
        Sys.Delay(this->delayus);
        *this->pportsck=0;
        ret <<= 1;
        if (this->pportdo->Read())
        {
            ret |= 1;
        }
    }
    return ret;
}

//SPI总线读数据
byte CSoftSpi::spi_readbyte(void)
{
    return spi_writebyte(0xff);
}
