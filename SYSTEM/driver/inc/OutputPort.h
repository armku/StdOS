#pragma once

#include "Port.h"

class OutputPort:public CPort
{
	public:
		OutputPort(PinPort pin,bool  invert=false);//�˿ںţ��Ƿ�ת��ƽ
		void Write(const bool onoff);//true���ߵ�ƽ��false���͵�ƽ
		void Init(PinPort pin,bool  invert=false);//�˿ںţ��Ƿ�ת��ƽ
		void operator = (const bool value);//��ֵ���������
	private:
		bool invertPort;//�˿ڷ�ת
};
