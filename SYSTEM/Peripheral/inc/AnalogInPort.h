#pragma once

#include "Port.h"

// 模拟输入输出口
class AnalogInPort: public Port
{
    public:
        AnalogInPort(Pin pin)
        {
                Set(pin);
        }

    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
};
