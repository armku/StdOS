#include "../W25QXXX.h"

#define _W25QXX_TEST_CPP

#ifdef _W25QXX_TEST_CPP

Spi spi;

SPI25QXX s25xx;
void w25qxxxtest()
{
	/*spi.SetPin(PA5,PA6,PA7);
	spi.SetNss(PA4);*/
	spi.CPOL = CPOL_High;
	spi.CPHA = CPHA_2Edge;
	spi.Init(Spi1);

	s25xx.SetSpi(&spi);
	s25xx.Init();
	s25xx.Test();
}
#endif
