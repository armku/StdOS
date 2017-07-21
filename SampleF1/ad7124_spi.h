#ifndef __AD7124_SPI_H
#define __AD7124_SPI_H

#include "stdint.h"

/*
	(SPI1 : AD7124-8)
	PA4		:	SPI1-NSS	
	PA5		:	SPI1-SCK
	PA6		:	SPI1-MISO
	PA7		:	SPI1-MOSI
*/

/*********************** 模拟SPI通信引脚 **************************************/
#define SPI_NSS_PIN			GPIO_Pin_4
#define SPI_NSS_PORT		GPIOA

#define SPI_SCK_PIN			GPIO_Pin_5
#define SPI_SCK_PORT		GPIOA

#define SPI_MISO_PIN		GPIO_Pin_6
#define SPI_MISO_PORT		GPIOA

#define SPI_MOSI_PIN		GPIO_Pin_7
#define SPI_MOSI_PORT		GPIOA

/********************  **************************************/
#define SPI_NSS_L()		(SPI_NSS_PORT->BRR = SPI_NSS_PIN)
#define SPI_NSS_H()		(SPI_NSS_PORT->BSRR = SPI_NSS_PIN)
#define SPI_SCK_L()		(SPI_SCK_PORT->BRR = SPI_SCK_PIN)
#define SPI_SCK_H()		(SPI_SCK_PORT->BSRR = SPI_SCK_PIN)
#define SPI_MOSI_L()	(SPI_MOSI_PORT->BRR = SPI_MOSI_PIN)
#define SPI_MOSI_H()	(SPI_MOSI_PORT->BSRR = SPI_MOSI_PIN)

#define SPI_MISO()		GPIO_ReadInputDataBit(SPI_MISO_PORT,SPI_MISO_PIN)

void AD7124_SPI_Config(void);
uint8_t spi8_run(uint8_t sendData);
uint16_t spi16_run(uint16_t sendData);
uint32_t spi24_run(uint32_t sendData);
uint32_t spi32_run(uint32_t sendData);

#endif
