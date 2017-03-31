#pragma once 
#include "Type.h"
#include "Port.h"

class PWM:private AlternatePort
{
    public:
        PWM(Pin pin, uint frq = 10000, uint duty = 50,uint prescaler=0,bool voltlevellow=true);
        void Init();
		void SetOut(ushort pwmValue);//�������ֵ
		void SetOutPercent(float per);//�������������0-100		
    private:
        uint freq;//Ƶ��
        uint duty;//���ֵ
		uint prescal;//Ԥ��Ƶ
		uint voltLevelLow;//����ʱ������ֵС�ڱȽ�ֵʱΪ�͵�ƽ
};
