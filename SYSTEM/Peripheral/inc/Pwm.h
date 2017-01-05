#pragma once 
#include "Type.h"
#include "AlternatePort.h"

class PWM:private AlternatePort
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
