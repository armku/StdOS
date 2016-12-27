#pragma once

#include <stdint.h>
#include "BasePort.h"
#include "SoftI2C.h"

class CNPA
{
    public:
        CNPA(PinPort pinsck, PinPort pinsda, uint8_t devAddr = 0X51);
        ~CNPA();
    public:
        void Init(void);
        int Read(void);
    private:
        CSoftI2C *pi2c;
        uint8_t DEVER_ADDR; //…Ë±∏µÿ÷∑
};
