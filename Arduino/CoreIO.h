#ifndef _COREIO_H
#define _COREIO_H

#include <stdint.h>
#include "../component/Type.h"
#include "../Porting/Pin_STM32F1.h"

#ifdef __cplusplus
extern "C" {
#endif
	void pinMode(Pin pin, uint8_t mode);
	void digitalWrite(Pin pin,uint8_t value);
	int8_t digitalRead();

#ifdef __cplusplus
}
#endif
#endif