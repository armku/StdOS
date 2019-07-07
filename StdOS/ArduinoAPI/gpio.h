#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mcu_type.h"

#ifndef NULL
#define NULL 0
#endif

#define ADC_Channel_X ((uint8_t)0xFF)
	typedef enum Pin
	{
		P0 = 0xFF,
		PA0 = 0x00,
		PA1 = 0x01,
		PA2 = 0x02,
		PA3 = 0x03,
		PA4 = 0x04,
		PA5 = 0x05,
		PA6 = 0x06,
		PA7 = 0x07,
		PA8 = 0x08,
		PA9 = 0x09,
		PA10 = 0x0A,
		PA11 = 0x0B,
		PA12 = 0x0C,
		PA13 = 0x0D,
		PA14 = 0x0E,
		PA15 = 0x0F,
		PB0 = 0x10,
		PB1 = 0x11,
		PB2 = 0x12,
		PB3 = 0x13,
		PB4 = 0x14,
		PB5 = 0x15,
		PB6 = 0x16,
		PB7 = 0x17,
		PB8 = 0x18,
		PB9 = 0x19,
		PB10 = 0x1A,
		PB11 = 0x1B,
		PB12 = 0x1C,
		PB13 = 0x1D,
		PB14 = 0x1E,
		PB15 = 0x1F,
		PC0 = 0x20,
		PC1 = 0x21,
		PC2 = 0x22,
		PC3 = 0x23,
		PC4 = 0x24,
		PC5 = 0x25,
		PC6 = 0x26,
		PC7 = 0x27,
		PC8 = 0x28,
		PC9 = 0x29,
		PC10 = 0x2A,
		PC11 = 0x2B,
		PC12 = 0x2C,
		PC13 = 0x2D,
		PC14 = 0x2E,
		PC15 = 0x2F,
		PD0 = 0x30,
		PD1 = 0x31,
		PD2 = 0x32,
		PD3 = 0x33,
		PD4 = 0x34,
		PD5 = 0x35,
		PD6 = 0x36,
		PD7 = 0x37,
		PD8 = 0x38,
		PD9 = 0x39,
		PD10 = 0x3A,
		PD11 = 0x3B,
		PD12 = 0x3C,
		PD13 = 0x3D,
		PD14 = 0x3E,
		PD15 = 0x3F,
		PE0 = 0x40,
		PE1 = 0x41,
		PE2 = 0x42,
		PE3 = 0x43,
		PE4 = 0x44,
		PE5 = 0x45,
		PE6 = 0x46,
		PE7 = 0x47,
		PE8 = 0x48,
		PE9 = 0x49,
		PE10 = 0x4A,
		PE11 = 0x4B,
		PE12 = 0x4C,
		PE13 = 0x4D,
		PE14 = 0x4E,
		PE15 = 0x4F,
		PF0 = 0x50,
		PF1 = 0x51,
		PF2 = 0x52,
		PF3 = 0x53,
		PF4 = 0x54,
		PF5 = 0x55,
		PF6 = 0x56,
		PF7 = 0x57,
		PF8 = 0x58,
		PF9 = 0x59,
		PF10 = 0x5A,
		PF11 = 0x5B,
		PF12 = 0x5C,
		PF13 = 0x5D,
		PF14 = 0x5E,
		PF15 = 0x5F,
		PG0 = 0x60,
		PG1 = 0x61,
		PG2 = 0x62,
		PG3 = 0x63,
		PG4 = 0x64,
		PG5 = 0x65,
		PG6 = 0x66,
		PG7 = 0x67,
		PG8 = 0x68,
		PG9 = 0x69,
		PG10 = 0x6A,
		PG11 = 0x6B,
		PG12 = 0x6C,
		PG13 = 0x6D,
		PG14 = 0x6E,
		PG15 = 0x6F,
		PH0 = 0x70,
		PH1 = 0x71,
		PH2 = 0x72,
		PH3 = 0x73,
		PH4 = 0x74,
		PH5 = 0x75,
		PH6 = 0x76,
		PH7 = 0x77,
		PH8 = 0x78,
		PH9 = 0x79,
		PH10 = 0x7A,
		PH11 = 0x7B,
		PH12 = 0x7C,
		PH13 = 0x7D,
		PH14 = 0x7E,
		PH15 = 0x7F,
		PI0 = 0x80,
		PI1 = 0x81,
		PI2 = 0x82,
		PI3 = 0x83,
		PI4 = 0x84,
		PI5 = 0x85,
		PI6 = 0x86,
		PI7 = 0x87,
		PI8 = 0x88,
		PI9 = 0x89,
		PI10 = 0x8A,
		PI11 = 0x8B,
		PI12 = 0x8C,
		PI13 = 0x8D,
		PI14 = 0x8E,
		PI15 = 0x8F,
		PIN_MAX = 0X90
	}Pin_Type;

	// 获取组和针脚
#define _PORT(PIN) (1 << ((PIN) & (uint16_t)0x0F))
#define _PIN(PIN) (PIN & 0x000F)
#define _PIN_NAME(pin) (pin==P0 ? '_' : ('A' + (pin >> 4))), (pin==P0 ? '0' : (pin & 0x0F))

//typedef enum {
//    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
//    PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
//    PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
//    PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15,
//    PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15,
//    PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13, PF14, PF15,
//    PG0, PG1, PG2, PG3, PG4, PG5, PG6, PG7, PG8, PG9, PG10, PG11, PG12, PG13, PG14, PG15,
//    PIN_MAX
//} Pin_Type;

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
