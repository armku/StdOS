#pragma once
#include "InputPort.h"

// ��ȡί�� ���ź� ���ŵ�ƽ״̬
typedef void(*IOReadHandler)(PinPort, bool);

class CExti: protected InputPort
{
    public:
        CExti(PinPort pin);
        void Init(); //��ʼ��
        void On(); //���ж�
        void Off(); //�ر��ж�		
        void Register(IOReadHandler handler); // ע��ص�
    private:
        void Exti0_state(bool onoff);
        uint eXTI_Line; //�ж���
        byte nVIC_IRQChannel; //�ж�����
        byte gPIO_PinSource; //�ж���
        byte gPIO_PortSourceGPIO; //�˿�
};
