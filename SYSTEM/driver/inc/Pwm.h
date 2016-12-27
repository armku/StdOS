#pragma once 
#include <stdint.h>
#include "BasePort.h"

class PWM: public BasePort
{
    public:
        PWM(PinPort pin, uint32_t frq = 10000, uint32_t duty = 50);
        void Init();
		void SetOut(uint16_t pwmValue);//�������ֵ
		void SetOutPercent(float per);//�������������0-100		
    private:
        uint32_t freq;//Ƶ��
        uint32_t duty;//���ֵ
};
