#include "NPA.h"

CNPA::CNPA(uint8_t devAddr)
{
	this->DEVER_ADDR = devAddr;
}

void CNPA::SetPin(Pin pinsck, Pin pinsda)
{
	this->pi2c.SetPin(pinsck, pinsda);
}
int CNPA::Read()
{
    uint8_t buf1, buf2;
    int buf;
    buf1 = 0;
    buf2 = 0;
    this->pi2c.Start();
    this->pi2c.WriteByte(DEVER_ADDR);
    if (this->pi2c.WaitAck())
    {
        return  - 1;
    }
    buf1 = this->pi2c.ReadByte(true);
    buf2 = this->pi2c.ReadByte(false);
    this->pi2c.Stop();
    buf = (buf1 << 8) | buf2;
    return buf;
}
//读取大气压值
float CNPA::ReadP()
{
	return this->Read() * 0.007278646;
}
