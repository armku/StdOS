#pragma once

#include "Type.h"
#include "BasePort.h"

class OutputPort:public BasePort
{
	public:
		OutputPort(PinPort pin,byte invert=2);
		void Init(PinPort pin,byte invert=2);
		void Write(bool onoff);//�������
		void Open();
	private:
		bool binvert;//�˿ڷ�ת
};
