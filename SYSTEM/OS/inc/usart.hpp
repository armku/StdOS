#pragma once 
#include "FIFORing.h"

void uart_init(uint32_t bound);

extern uint32_t com1timeidle; //����1����ʱ��

extern CFIFORing com1buf;//����1���ջ�����
