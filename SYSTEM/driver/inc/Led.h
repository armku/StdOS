#pragma once

#include "BasePort.h"

class CLed:public BasePort
{
    public:
        CLed(PinPort pin, uint16_t period1 = 1000, uint16_t onTime1 = 500);       
	
        byte Init();
        void On();
        void Off(); 
    private:
        uint16_t period; //����
        uint16_t onTime; //����ʱ��
};
