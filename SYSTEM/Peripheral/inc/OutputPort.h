#pragma once

#include "Port.h"






















#if 0
class OutputPort:protected Port
{
	public:
		OutputPort();
		OutputPort(Pin pin,byte invert=2);
		void Init(Pin pin,byte invert=2);
		void Write(bool onoff);//�������
		void Open();
		void Toggle(); //��ת����
	private:
		bool binvert;//�˿ڷ�ת
};
#endif
