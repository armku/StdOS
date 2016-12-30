#pragma once

#include "BasePort.h"

class OutputPort:protected BasePort
{
	public:
		OutputPort(PinPort pin,byte invert=2);
		void Init(PinPort pin,byte invert=2);
		void Write(bool onoff);//�������
		void Open();
		void Toggle(); //��ת����
	private:
		bool binvert;//�˿ڷ�ת
};
