#include "../W25QXXX.h"

#define _W25QXX_TEST_CPP

#ifdef _W25QXX_TEST_CPP
/************************** SPI Flash 函数宏定义********************************/

Spi spi;

SPI25QXX s25xx(&spi);
    void w25qxxxtest()
    {       
		spi.SetPin(PA4,PA5,PA6);
		spi.SetNss(PA7);
		s25xx.Test();
		
    }
#endif
