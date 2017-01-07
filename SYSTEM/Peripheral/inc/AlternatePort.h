#pragma once

#include "OutputPort.h"


// ���������
class AlternatePort: public OutputPort
{
    public:
        AlternatePort(): OutputPort()
        {}
        // �������һ������������������Ҫ����
        AlternatePort(Pin pin, bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED): OutputPort(pin,invert,openDrain,speed)
        {}

    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);

        #if DEBUG
            virtual bool OnReserve(Pin pin, bool flag);
        #endif 
};
