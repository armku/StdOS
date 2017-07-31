#include "CAN.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif

/* ¿ØÖÆÆ÷ÇøÓòÍøÂç(CAN)Õë½Å ------------------------------------------------------------------*/
//						  TX    RX
#define CAN_PINS		{PA12, PA11} // AFIO_MAPR_CAN_REMAP_REMAP1
#define CAN_PINS_REMAP2	{PB9,  PB8 } // AFIO_MAPR_CAN_REMAP_REMAP2
#define CAN_PINS_REMAP3	{PD1,  PD0 } // AFIO_MAPR_CAN_REMAP_REMAP3
