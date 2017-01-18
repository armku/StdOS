#pragma once

#include "OutputPort.h"


// ���������
class AlternatePort: public OutputPort
{
    public:
        AlternatePort(): OutputPort()
        {
			Init(false,false);
		}
        // �������һ������������������Ҫ����
        AlternatePort(Pin pin, bool invert = false, bool openDrain = false, uint speed = GPIO_MAX_SPEED): OutputPort(pin,invert,openDrain,speed)
        {
			Init(invert,openDrain,speed);
			Set(pin);
		}

    protected:
        virtual void OnConfig(GPIO_InitTypeDef &gpio);
};
