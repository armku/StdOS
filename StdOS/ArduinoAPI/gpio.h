#include "../StdOS/Platform/Pin.h"

#include "mcu_type.h"

#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL 0
#endif

#define ADC_Channel_X ((uint8_t)0xFF)
	
	typedef struct STM32_PinInfo {
		GPIO_TypeDef* GPIOx;
		TIM_TypeDef* TIMx;
		ADC_TypeDef* ADCx;

		uint16_t GPIO_Pin_x;
		uint8_t TimerChannel;
		uint8_t ADC_Channel;
	} PinInfo_TypeDef;

	extern const PinInfo_TypeDef PIN_MAP[PIN_MAX];

#define IS_ADC_PIN(Pin) (PIN_MAP[Pin].ADC_Channel  != ADC_Channel_X)
#define IS_PWM_PIN(Pin) (PIN_MAP[Pin].TimerChannel != 0)
#define IS_PIN(Pin)     (Pin <= PIN_MAX)

	void GPIOx_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x, GPIOMode_TypeDef GPIO_Mode_x, GPIOSpeed_TypeDef GPIO_Speed_x);
	void GPIO_JTAG_Disable(void);
	uint8_t Get_GPIOx(uint8_t Pin);
	uint8_t Get_GPIO_PinSource(uint16_t GPIO_Pin_x);
	uint8_t Get_Pinx(uint8_t Pin);

#ifdef __cplusplus
}
#endif

#endif
