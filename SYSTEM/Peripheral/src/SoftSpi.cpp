#include "SoftSpi.h"
#include "Sys.h"

CSoftSpi::CSoftSpi(Pin pincs, Pin pinsck, Pin pindi, Pin pindo, uint nus)
{
    this->portcs.Set(pincs);
    this->portsck.Set(pinsck);
    this->portdi.Set(pindi);
    this->portdo.Set(pindo);

    //this->portdo->SetModeIN_FLOATING();

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
    this->portcs=0;
    i = 0;
    while (this->portdo.Read() > 0)
    {
        Sys.Sleep(10);
        i++;
        if (i > 200)
            return 1;
    }
    this->portcs=1;
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
            this->portdi=1;
        }
        else
        {
            this->portdi=0;
        }
		Sys.Delay(this->delayus);
        this->portsck=1;
        Sys.Delay(this->delayus);
        this->portsck=0;
        ret <<= 1;
        if (this->portdo.Read())
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
