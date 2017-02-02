#pragma once

#include "Sys.h"

//�ص��������� downָʾ���»��ǵ��𴥷�
typedef void(*BtnInCallbackOld)(Pin pin, bool down);

class InputPortOld
{
    public:
		InputPortOld();
        InputPortOld(Pin pin);
        void InitOld();
         
		Pin _Pin; // ���
   
        void On(); //���ж�
        void Off(); //�ر��ж�		
        void RegisterOld(BtnInCallbackOld handler); // ע��ص�		
    private:
        void Exti0_state(bool onoff);
        uint eXTI_Line; //�ж���
        byte nVIC_IRQChannel; //�ж�����
        byte gPIO_PinSource; //�ж���
        byte gPIO_PortSourceGPIO; //�˿�	
	public:        	
		void Write(const bool value);//д��ֵ��true���ߵ�ƽ��false���͵�ƽ			
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
