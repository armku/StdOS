#pragma once 
#include "FIFORing.h"

void uart_init(uint bound);

extern uint com1timeidle; //串口1空闲时间

extern CFIFORing com1buf;//串口1接收缓冲区
