#pragma once

#include "BasePort.h"

//�ص��������� downָʾ���»��ǵ��𴥷�
typedef void(*BtnInCallback)(PinPort pin,bool down); 

class InputPort:protected BasePort
{
	public:
		InputPort(PinPort pin,byte invert=2);
		void Init(PinPort pin,byte invert=2);
		byte Read();//��ȡ�˿ڵ�ƽ
		void Open();	
		void UsePress();//�����¼�����
	public:
		BtnInCallback callBack;//�ص�����
		uint PressTime;//��������
	private:
		bool binvert;//�˿ڷ�ת
};
