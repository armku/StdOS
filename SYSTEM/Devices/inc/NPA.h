#pragma once

#include "Type.h"
#include "Port.h"
#include "SoftI2C.h"

class CNPA
{
    public:
        CNPA(PinPort pinsck, PinPort pinsda, byte devAddr = 0X51);
        ~CNPA();
    public:
        void Init(void);
        int Read(void);
    private:
        CSoftI2C *pi2c;
        byte DEVER_ADDR; //…Ë±∏µÿ÷∑
};
