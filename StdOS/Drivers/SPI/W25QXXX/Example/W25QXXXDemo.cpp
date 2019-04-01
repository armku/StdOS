#include "../W25QXXX.h"

#define _W25QXX_TEST_CPP

#ifdef _W25QXX_TEST_CPP

Spi spi(Spi1);

SPI25QXX s25xx;
void w25qxxxtest()
{
	/*spi.SetPin(PA5,PA6,PA7);
	spi.SetNss(PA4);*/
	spi.Init(CPOL_High, CPHA_2Edge);

	s25xx.SetSpi(&spi);
	s25xx.Test();
}
#endif
