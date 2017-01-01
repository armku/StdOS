#pragma once
#include "BasePort.h"


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

class CExti:public BasePort
{
    public:
        CExti(PinPort pin);
        void Init(); //��ʼ��
        void On(); //���ж�
        void Off(); //�ر��ж�		
		void TIO_Register(PinPort pin, IOReadHandler handler);// ע��ص�  ���ж�ʹ��
	private:
		BasePort *port;	//�˿�
	private:
		void Exti0_state(bool onoff);
		uint eXTI_Line;//�ж���
		byte nVIC_IRQChannel;//�ж�����
		byte gPIO_PinSource;//�ж���
		byte gPIO_PortSourceGPIO;//�˿�
};
