#pragma once
#include "InputPort.h"


// ��ȡί��
typedef void (*IOReadHandler)(PinPort , bool );
/* �ж�״̬�ṹ�� */
/* һ��16���ж��ߣ���ζ��ͬһ����ÿһ��ֻ����һ������ʹ���ж� */
typedef struct TIntState
{
    PinPort Pin;
    IOReadHandler Handler;
    bool OldValue;
} IntState;

class CExti:protected InputPort
{
    public:
        CExti(PinPort pin);
        void Init(); //��ʼ��
        void On(); //���ж�
        void Off(); //�ر��ж�		
		void Register(IOReadHandler handler);// ע��ص�  ���ж�ʹ��
	private:
		void Exti0_state(bool onoff);
		uint eXTI_Line;//�ж���
		byte nVIC_IRQChannel;//�ж�����
		byte gPIO_PinSource;//�ж���
		byte gPIO_PortSourceGPIO;//�˿�
};
