#pragma once

#include "Sys.h"
#include "FIFORing.h"

class SerialPortOld
{
	public:
		SerialPortOld(COM_Def comPort,uint bound=115200);
		COM_Def ComPort;//�˿ں�
		
		
};
extern uint com1timeidle; //����1����ʱ��

extern CFIFORing com1buf;//����1���ջ�����
