#pragma once

#include "Port.h"

class CLed:public CPort
{
    public:
        CLed(uint16_t pin, uint16_t period1 = 1000, uint16_t onTime1 = 500);
        ~CLed();
	public:
        uint8_t Init();
        void On();
        void Off(); 
    private:
        uint16_t period; //周期
        uint16_t onTime; //灯亮时间
};
