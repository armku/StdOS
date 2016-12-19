#pragma once
#include "Port.h"

class CExti
{
    public:
        CExti(uint16_t pin);
        ~CExti();
    public:
        void Init(); //��ʼ��
        void On(); //���ж�
        void Off(); //�ر��ж�
	private:
		CPort *port;	//�˿�
	private:
		void Exti0_state(bool onoff);
};
