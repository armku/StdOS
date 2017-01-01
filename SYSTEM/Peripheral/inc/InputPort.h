#pragma once

#include "BasePort.h"

//�ص��������� downָʾ���»��ǵ��𴥷�
typedef void(*BtnInCallback)(PinPort pin, bool down);

class InputPort: protected BasePort
{
    public:
        InputPort(PinPort pin, byte invert = 2);
        void Init();
        //void Open();	
        //void UsePress();//�����¼�����
    public:
        //BtnInCallback callBack;//�ص�����
        //uint PressTime;//��������
    private:
        bool binvert; //�˿ڷ�ת
    public:
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
