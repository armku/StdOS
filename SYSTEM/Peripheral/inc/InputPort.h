#pragma once

#include "Sys.h"

//�ص��������� downָʾ���»��ǵ��𴥷�
typedef void(*BtnInCallback)(Pin pin, bool down);

class InputPort
{
    public:
		InputPort();
        InputPort(Pin pin);
        void Init();
         
		Pin _Pin; // ���
   
        void On(); //���ж�
        void Off(); //�ر��ж�		
        void Register(BtnInCallback handler); // ע��ص�		
    private:
        void Exti0_state(bool onoff);
        uint eXTI_Line; //�ж���
        byte nVIC_IRQChannel; //�ж�����
        byte gPIO_PinSource; //�ж���
        byte gPIO_PortSourceGPIO; //�˿�	
	public:        	
		void Write(const bool value);//д��ֵ��true���ߵ�ƽ��false���͵�ƽ
		void operator = (const bool onoff);//д��ֵ��true:�� false:�ر�
		void operator = (const int hilo);//д��ֵ������:�ߵ�ƽ 0:�͵�ƽ
        void SetModeAIN();
        void SetModeIN_FLOATING();
        void SetModeINPUT_IPD();
        void SetModeINPUT_IPU();
        void SetModeOut_OD();
        void SetModeOut_PP();
        void SetModeAF_OD();
        void SetModeAF_PP();
        void Set(); //��������
        void Reset(); //��λ����        
        byte Read(void); 
		static bool ReadPinPort(Pin pin);//��ȡ�˿�״̬
    protected:         
        void SetMode(PIN_MODE mode); //����ģʽ		
};
