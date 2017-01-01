#pragma once
#include "InputPort.h"

//�ص��������� downָʾ���»��ǵ��𴥷�
typedef void(*BtnInCallback)(PinPort pin,bool down); 

class CExti: protected InputPort
{
    public:
        CExti(PinPort pin,byte invert=2);
        void Init(PinPort pin,byte invert=2); //��ʼ��
        void On(); //���ж�
        void Off(); //�ر��ж�		
        void Register(BtnInCallback handler); // ע��ص�
    private:
        void Exti0_state(bool onoff);
        uint eXTI_Line; //�ж���
        byte nVIC_IRQChannel; //�ж�����
        byte gPIO_PinSource; //�ж���
        byte gPIO_PortSourceGPIO; //�˿�
};
