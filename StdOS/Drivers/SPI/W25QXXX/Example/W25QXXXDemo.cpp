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
				
		s25xx.Test();		
    }
#endif
