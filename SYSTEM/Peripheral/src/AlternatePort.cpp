#include <stdio.h>
#include "AlternatePort.h"

void AlternatePort::OnConfig(GPIO_InitTypeDef &gpio)
{	
    OutputPort::OnConfig(gpio);

    #ifdef STM32F1
        gpio.GPIO_Mode = OpenDrain ? GPIO_Mode_AF_OD : GPIO_Mode_AF_PP;
    #else 
        gpio.GPIO_Mode = GPIO_Mode_AF;
        gpio.GPIO_OType = OpenDrain ? GPIO_OType_OD : GPIO_OType_PP;
    #endif 	
}
