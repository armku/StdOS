#pragma once
#include "BasePort.h"

class CExti
{
    public:
        CExti(PinPort pin);
        ~CExti();
    public:
        void Init(); //��ʼ��
        void On(); //���ж�
        void Off(); //�ر��ж�
	private:
		BasePort *port;	//�˿�
	private:
		void Exti0_state(bool onoff);
		uint eXTI_Line;//�ж���
		byte nVIC_IRQChannel;//�ж�����
		byte gPIO_PinSource;//�ж���
		byte gPIO_PortSourceGPIO;//�˿�
};
