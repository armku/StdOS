#include "../W25QXXX.h"

#define _W25QXX_TEST_CPP

#ifdef _W25QXX_TEST_CPP
/************************** SPI Flash 函数宏定义********************************/

SPI25QXX s25xx;
    void w25qxxxtest()
    {       
		s25xx.Test();
		
    }
#endif
