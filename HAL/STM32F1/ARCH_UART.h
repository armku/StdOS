#ifndef _ARCH_UART_H
#define _ARCH_UART_H

#include <stdint.h>
#include "type.h"

typedef struct
	{
		//uint8_t data[1024];
		uint8_t data[512];
		uint32_t len;
	}
	_data_Q_t;
	
	typedef void (*pFun_UART)(uint8_t c); 


#endif
