#include "Power.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif

void Power::OnStop(int msTime)
{
    return ;
}
void Power::OnSleep(int msTime)
{
    return ;
}



void Power::OnStandby(int msTime)
{
    return ;
}
