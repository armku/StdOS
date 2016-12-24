#include "SpiSoft.h"
#include "delay.h"

CSpiSoft::CSpiSoft(PinPort pincs, PinPort pinsck, PinPort pindi, PinPort pindo, uint32_t nus)
{
    this->portcs = new CPort(pincs);
    this->portsck = new CPort(pinsck);
    this->portdi = new CPort(pindi);
    this->portdo = new CPort(pindo);

    this->portcs->SetModeOut_PP();
    this->portsck->SetModeOut_PP();
    this->portdi->SetModeOut_PP();
    this->portdo->SetModeIN_FLOATING();

    this->delayus = nus;
}

CSpiSoft::~CSpiSoft() {}
uint8_t CSpiSoft::Init()
{
    return 0;
}
/*---------------------------------------------------------
忙状态判断，最长等待时间，200 X 10 ms=2S
---------------------------------------------------------*/
uint8_t CSpiSoft::WaitBusy()
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

//SPI写字节
uint8_t CSpiSoft::spi_writebyte(uint8_t da)
{
    uint8_t i;
    uint8_t ret = 0;
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

//SPI总线读数据
uint8_t CSpiSoft::spi_readbyte(void)
{
    return spi_writebyte(0xff);
}
