#pragma once 
#include "FIFORing.h"

void uart_init(uint32_t bound);

extern uint32_t com1timeidle; //串口1空闲时间

extern CFIFORing com1buf;//串口1接收缓冲区
