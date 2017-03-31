#pragma once

#include "Type.h"
#include "Port.h"
#include "SoftI2C.h"

class CNPA
{
    public:
        CNPA(Pin pinsck, Pin pinsda, byte devAddr = 0X51);        
        void Init(void);
        int Read(void);
    private:
        CSoftI2C *pi2c;
        byte DEVER_ADDR; //�豸��ַ
};
