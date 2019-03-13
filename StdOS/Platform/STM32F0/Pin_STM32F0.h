#ifndef _PIN_STM32F0_H_
#define _PIN_STM32F0_H_ 1

#include "..\Pin.h"

/* 通用同步/异步收发器(USART)针脚 ------------------------------------------------------------------*/
/*STM32F030C8T6串口针脚*/
	//TX		RX		COM		AF
	//PA2		PA3		COM2	AF1
	//PA9		PA10	COM1	AF1
	//PA14		PA15	COM2	AF1
	//PB6		PB7		COM1	AF0
#if defined (STM32F0XX_MD) || defined (STM32F030X8) || defined (STM32F072)
	#define UARTS {USART1, USART2}
	#define UART_IRQs {USART1_IRQn, USART2_IRQn}
	#define UART_PINS {\
	 /* TX   RX   CTS  RTS */	\
		PA9, PA10,PA11,PA12,/* USART1 */	\
		PA2, PA3, PA0, PA1, /* USART2 */	\
	}
	#define UART_PINS_FULLREMAP {\
	 /* TX   RX   CTS  RTS */	\
		PB6, PB7, PA11,PA12,/* USART1 AFIO_MAPR_USART1_REMAP */	\
		PD5, PD6, PD3, PD4, /* USART2 AFIO_MAPR_USART2_REMAP */	\
	}
#elif defined (STM32F0XX_LD) || defined (STM32F030X6)
	#define UARTS {USART1}
	#define UART_IRQs {USART1_IRQn}
	#define UART_PINS {\
	 /* TX   RX   CTS  RTS */	\
		PA9, PA10,PA11,PA12,/* USART1 */	\
	}
	#define UART_PINS_FULLREMAP {\
	 /* TX   RX   CTS  RTS */	\
		PB6, PB7, PA11,PA12,/* USART1 AFIO_MAPR_USART1_REMAP */	\
	}
#endif

/* 定时器针脚 ------------------------------------------------------------------*/
#if defined (STM32F0XX_MD) || defined (STM32F030X8) || defined (STM32F072)
	#define TIMS {TIM1, TIM2, TIM3, 0, 0, TIM6, 0, 0, 0, 0, 0, 0, 0, TIM14, TIM15, TIM16, TIM17}
	#define TIM_IRQns {TIM1_BRK_UP_TRG_COM_IRQn, TIM2_IRQn, TIM3_IRQn, 0, 0, TIM6_DAC_IRQn, 0, 0, 0, 0, 0, 0, 0, TIM14_IRQn, TIM15_IRQn, TIM16_IRQn, TIM17_IRQn}
#elif defined (STM32F0XX_LD) || defined (STM32F030X6)
	#define TIMS {TIM1, TIM2, TIM3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TIM14, 0, TIM16, TIM17}
	#define TIM_IRQns {TIM1_BRK_UP_TRG_COM_IRQn, TIM2_IRQn, TIM3_IRQn, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TIM14_IRQn, 0, TIM16_IRQn, TIM17_IRQn}
#endif
#define TIM_PINS {\
	PA8, PA9, PA10,PA11,/* TIM1 */	\
	PA0, PA1, PA2, PA3, /* TIM2 */	\
	PA6, PA7, PB0, PB1, /* TIM3 */	\
	PB6, PB7, PB8, PB9, /* TIM4 */	\
	PA0, PA1, PA2, PA3, /* TIM5 */	\
	P0,  P0,  P0,  P0,	/* TIM6 */	\
	P0,  P0,  P0,  P0,	/* TIM7 */	\
	PC6, PC7, PC8, PC9	/* TIM8 */	\
}
#define TIM_PINS_FULLREMAP {\
	PE9, PE11,PE13,PE14,/* TIM1 AFIO_MAPR_TIM1_REMAP_FULLREMAP */	\
	PA15,PB3, PB10,PB11,/* TIM2 AFIO_MAPR_TIM2_REMAP_FULLREMAP */	\
	PC6, PC7, PC8, PC9, /* TIM3 AFIO_MAPR_TIM3_REMAP_FULLREMAP */	\
	PD12,PD13,PD14,PD15,/* TIM4 AFIO_MAPR_TIM4_REMAP */	\
	PA0, PA1, PA2, PA3, /* TIM5 */	\
	P0,  P0,  P0,  P0,	/* TIM6 */	\
	P0,  P0,  P0,  P0,	/* TIM7 */	\
	PC6, PC7, PC8, PC9	/* TIM8 */	\
}

/* ADC(模拟/数字转换器)针脚 ------------------------------------------------------------------*/
#define DAC_PINS {PA4,PA5}
#define ADCS {ADC1}
#define ADC_RCCS {RCC_APB2Periph_ADC1}
#define ADC1_PINS {PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PB0,PB1,PC0,PC1,PC2,PC3,PC4,PC5}
#define ADC3_PINS {PA0,PA1,PA2,PA3,PF6,PF7,PF8,PF9,PF10,PF3,PC0,PC1,PC2,PC3,PF4,PF5}

/* I2C总线针脚 ------------------------------------------------------------------*/
#define I2CS {I2C1, I2C2}

#define I2C_PINS {\
	/*	SCL		SDA	*/\
	{	PB6,	PB7	 },\
	{	PB10,	PB11 },\
/*F1 REMAP{	PA8,	PA9	 },*/\
}

/* 串行外设接口(SPI)针脚 ------------------------------------------------------------------*/
#define SPIS {SPI1, SPI2}

#define SPI_PINS_FULLREMAP	{\
	/*	NSS  CLK  MISO MOSI	*/\
		{PA4, PA5, PA6, PA7 },\
		{PB12,PB13,PB14,PB15},\
		{PA15,PB3, PB4, PB5 }\
}

/* 控制器区域网络(CAN)针脚 ------------------------------------------------------------------*/
//						  TX    RX
#define CAN_PINS		{PA12, PA11} // AFIO_MAPR_CAN_REMAP_REMAP1
#define CAN_PINS_REMAP2	{PB9,  PB8 } // AFIO_MAPR_CAN_REMAP_REMAP2
#define CAN_PINS_REMAP3	{PD1,  PD0 } // AFIO_MAPR_CAN_REMAP_REMAP3

/* 通用串行总线(USB)针脚 ------------------------------------------------------------------*/

/* SDIO针脚 ------------------------------------------------------------------*/

#endif
