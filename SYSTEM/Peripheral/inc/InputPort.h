#pragma once

#include "BasePort.h"



class InputPort:protected BasePort
{
	public:
		InputPort(PinPort pin,byte invert=2);
		void Init(PinPort pin,byte invert=2);		
		void Open();	
		void UsePress();//�����¼�����
	public:
		//BtnInCallback callBack;//�ص�����
		uint PressTime;//��������
	private:
		bool binvert;//�˿ڷ�ת
};
