#include "SoftSpi.h"
#include "delay.h"

CSoftSpi::CSoftSpi(PinPort pincs, PinPort pinsck, PinPort pindi, PinPort pindo, uint32_t nus)
{
    this->portcs = new BasePort(pincs);
    this->portsck = new BasePort(pinsck);
    this->portdi = new BasePort(pindi);
    this->portdo = new BasePort(pindo);

    this->portcs->SetModeOut_PP();
    this->portsck->SetModeOut_PP();
    this->portdi->SetModeOut_PP();
    this->portdo->SetModeIN_FLOATING();

    this->delayus = nus;
}


byte CSoftSpi::Init()
{
    return 0;
}
/*---------------------------------------------------------
æ״̬�жϣ���ȴ�ʱ�䣬200 X 10 ms=2S
---------------------------------------------------------*/
byte CSoftSpi::WaitBusy()
{
    uint16_t i;
    this->portcs->Reset();
    i = 0;
    while (this->portdo->Read() > 0)
    {
        delay_ms(10);
        i++;
        if (i > 200)
            return 1;
    }
    this->portcs->Set();
    return 0;
}

//SPIд�ֽ�
byte CSoftSpi::spi_writebyte(byte da)
{
    byte i;
    byte ret = 0;
    for (i = 0; i < 8; i++)
    {
        if (da & (1 << (8 - i - 1)))
        {
            this->portdi->Set();
        }
        else
        {
            this->portdi->Reset();
        }
        delay_us(this->delayus);
        this->portsck->Set();
        delay_us(this->delayus);
        this->portsck->Reset();
        ret <<= 1;
        if (this->portdo->Read())
        {
            ret |= 1;
        }
    }
    return ret;
}

//SPI���߶�����
byte CSoftSpi::spi_readbyte(void)
{
    return spi_writebyte(0xff);
}
