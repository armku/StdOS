#include "AnalogInPort.h"


void AnalogInPort::OnConfig(GPIO_InitTypeDef &gpio)
{
    Port::OnConfig(gpio);

    #ifdef STM32F1
        gpio.GPIO_Mode = GPIO_Mode_AIN; //
    #else 
        gpio.GPIO_Mode = GPIO_Mode_AN;
        //gpio.GPIO_OType = !Floating ? GPIO_OType_OD : GPIO_OType_PP;
    #endif 
}
