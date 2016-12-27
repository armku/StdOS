#pragma once 
#include <stdint.h>
#include "BasePort.h"

class PWM: public BasePort
{
    public:
        PWM(PinPort pin, uint frq = 10000, uint duty = 50);
        void Init();
		void SetOut(ushort pwmValue);//�������ֵ
		void SetOutPercent(float per);//�������������0-100		
    private:
        uint freq;//Ƶ��
        uint duty;//���ֵ
};
