#pragma once 
#include "Type.h"
#include "InputPort.h"

class PWM: public InputPort
{
    public:
        PWM(Pin pin, uint frq = 10000, uint duty = 50);
        void Init();
		void SetOut(ushort pwmValue);//�������ֵ
		void SetOutPercent(float per);//�������������0-100		
    private:
        uint freq;//Ƶ��
        uint duty;//���ֵ
};
