#include "../W25QXXX.h"

#define _W25QXX_TEST_CPP

#ifdef _W25QXX_TEST_CPP

//#define SPISOFT

#ifdef SPISOFT
SpiSoft spi;
#else
Spi spi(Spi1);
#endif

SPI25QXX s25xx;
void w25qxxxtest()
{
#ifdef SPISOFT
	spi.SetPin(PA5,PA6,PA7);
	spi.SetNss(PA4);
#endif
	spi.Init(CPOL_High, CPHA_2Edge);

	s25xx.SetSpi(&spi);
	s25xx.Test();
}
#endif
