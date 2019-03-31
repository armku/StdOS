#include "../W25QXXX.h"

#define _W25QXX_TEST_CPP

#ifdef _W25QXX_TEST_CPP
/************************** SPI Flash 函数宏定义********************************/
#include "stm32f10x.h"
Spi spi;

SPI25QXX s25xx(&spi);
    void w25qxxxtest()
    {       
		spi.SetPin(PA5,PA6,PA7);
		spi.SetNss(PA4);

		//GPIO_InitTypeDef GPIO_InitStructure;
		///*!< Configure SPI_FLASH_SPI_CS_PIN pin: SPI_FLASH Card CS pin */
		//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		//GPIO_Init(GPIOA, &GPIO_InitStructure);

		///*!< Configure SPI_FLASH_SPI pins: SCK */
		//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		//GPIO_Init(GPIOA, &GPIO_InitStructure);

		///*!< Configure SPI_FLASH_SPI pins: MISO */
		//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		//GPIO_Init(GPIOA, &GPIO_InitStructure);

		///*!< Configure SPI_FLASH_SPI pins: MOSI */
		//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		//GPIO_Init(GPIOA, &GPIO_InitStructure);
		s25xx.Test();
		
    }
#endif
