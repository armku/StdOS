#pragma once

#include "OutputPort.h"


// 复用输出口
class AlternatePort: public OutputPort
{
    public:
        AlternatePort(): OutputPort()
        {}
        // 复用输出一般采用推挽输出，不需要倒置
        AlternatePort(Pin pin, bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED): OutputPort(pin,invert,openDrain,speed)
        {}

    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);

        #if DEBUG
            virtual bool OnReserve(Pin pin, bool flag);
        #endif 
};
