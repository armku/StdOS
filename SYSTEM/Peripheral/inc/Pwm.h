#pragma once 
#include "Type.h"
#include "Port.h"

class PWM:private AlternatePort
{
    public:
        PWM(Pin pin, uint frq = 10000, uint duty = 50,uint prescaler=0);
        void Init();
		void SetOut(ushort pwmValue);//设置输出值
		void SetOutPercent(float per);//设置输出比例，0-100		
    private:
        uint freq;//频率
        uint duty;//输出值
		uint prescal;//预分频
};
