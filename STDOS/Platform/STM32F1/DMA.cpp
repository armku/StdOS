#include "Type.h"
#include "DMA.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif

// ֹͣ
bool DMA::WaitForStop()
{
	return false;
}// ��ʼ
bool DMA::Start()
{
	return false;
}
