#pragma once

#include <stdint.h>
#include "Port.h"
#include "I2CSoft.h"

class CNPA
{
    public:
        CNPA(PinPort pinsck, PinPort pinsda, uint8_t devAddr = 0X51);
        ~CNPA();
    public:
        void Init(void);
        int Read(void);
    private:
        CI2CSoft *pi2c;
        uint8_t DEVER_ADDR; //…Ë±∏µÿ÷∑
};
