#pragma once 
#include "FIFORing.h"

void uart_init(uint bound);

extern uint com1timeidle; //����1����ʱ��

extern CFIFORing com1buf;//����1���ջ�����
