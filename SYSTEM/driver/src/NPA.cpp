#include "NPA.h"
#include "delay.h"

CNPA::CNPA(PinPort pinsck, PinPort pinsda, uint8_t devAddr)
{
    this->DEVER_ADDR = devAddr;
    this->pi2c = new CI2CSoft(pinsck, pinsda);
}

CNPA::~CNPA(){}

void CNPA::Init(void)
{
    this->pi2c->Init();
}

int CNPA::Read(void)
{
    uint8_t buf1, buf2;
    int buf;
    buf1 = 0;
    buf2 = 0;
    this->pi2c->Start();
    this->pi2c->WriteByte(DEVER_ADDR);
    if (this->pi2c->WaitAck())
    {
        return  - 1;
    }
    buf1 = this->pi2c->ReadByte();
    buf2 = this->pi2c->ReadByte();
    this->pi2c->Stop();
    buf = (buf1 << 8) | buf2;
    return buf;
}
