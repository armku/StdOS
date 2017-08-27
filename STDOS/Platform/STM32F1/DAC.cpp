#include "Port.h"
#include "DAC.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif

void DAConverter::OnInit()
{
}
bool DAConverter::OnOpen()
{
	return false;
}
bool DAConverter::OnClose()
{
	return false;
}
bool DAConverter::OnWrite(ushort value)
{
	value=value;
	return false;
}
