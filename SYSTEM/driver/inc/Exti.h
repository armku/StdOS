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
		uint32_t eXTI_Line;//�ж���
		uint8_t nVIC_IRQChannel;//�ж�����
		uint8_t gPIO_PinSource;//�ж���
		uint8_t gPIO_PortSourceGPIO;//�˿�
};
