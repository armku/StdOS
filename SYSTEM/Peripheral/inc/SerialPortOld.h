#pragma once

#include "FIFORing.h"

class SerialPortOld
{
	public:
		
};




void uart_init(uint bound);

extern uint com1timeidle; //����1����ʱ��

extern CFIFORing com1buf;//����1���ջ�����
