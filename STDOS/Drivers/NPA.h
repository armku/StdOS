#pragma once

#include "Type.h"
#include "Port.h"
#include "I2C.h"

class CNPA
{
    public:
        CNPA(Pin pinsck, Pin pinsda, byte devAddr = 0X51);
        int Read(void);
    private:
        SoftI2C *pi2c;
        byte DEVER_ADDR; //…Ë±∏µÿ÷∑
};
