#include "ADC.h"

#ifdef STM32F0
	#include "stm32f0xx.h"
#elif defined STM32F1
	#include "stm32f10x.h"
#elif defined STM32F4
	#include "stm32f4xx.h"
#endif

/* ADC(Ä£Äâ/Êý×Ö×ª»»Æ÷)Õë½Å ------------------------------------------------------------------*/
#define DAC_PINS {PA4,PA5}
#define ADC1_PINS {PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PB0,PB1,PC0,PC1,PC2,PC3,PC4,PC5}
#define ADC3_PINS {PA0,PA1,PA2,PA3,PF6,PF7,PF8,PF9,PF10,PF3,PC0,PC1,PC2,PC3,PF4,PF5}
