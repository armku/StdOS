#pragma once

#include "Port.h"

typedef void(*BtnCallback)(void*); //�ص���������

class CInputPort:public CPort
{
	public:
		CInputPort(PinPort pin);
		BtnCallback Press;//�ص�����
		bool Read();//��ȡ״̬
		void UsePress();//�����¼�
		void Open();//�򿪴�����
		uint16_t PressTime;//���µ�ʱ����ms
};
