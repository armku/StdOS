#ifndef _Port_H_
#define _Port_H_

#ifdef STM32F0

#elif defined STM32F1
#include "mcuGpio.h"
#include "../Bsp/ST/STM32F1/Porting/USART.h"
#include "../Bsp/ST/STM32F1/Porting/BspPlatform.h"
#include "../Bsp/ST/STM32F1/Porting/Spi.h"

#include "../Bsp/ST/STM32F1/Porting/FIFOBuffer.h"
#include "../Bsp/ST/STM32F1/Porting/Configuration.h"
#elif defined STM32F4

#endif

#endif //_Port_H_
