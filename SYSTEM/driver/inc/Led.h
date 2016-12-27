#pragma once

#include "Port.h"

class CLed:public BasePort
{
    public:
        CLed(PinPort pin, uint16_t period1 = 1000, uint16_t onTime1 = 500);
        ~CLed();
	public:
        uint8_t Init();
        void On();
        void Off(); 
    private:
        uint16_t period; //����
        uint16_t onTime; //����ʱ��
};
