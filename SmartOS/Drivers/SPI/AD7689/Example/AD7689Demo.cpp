#include "../AD7689.h"
#include "Sys.h"

//#define _AD7689_TEST_CPP
#ifdef _AD7689_TEST_CPP

CAD7689 ad7689;

void adRouton123(void* param)
{
	ad7689.Routin();
}
void Test123(void *param)
{
	for (int i = 0; i < 8; i++)
	{
		debug_printf("%1.4f ", ad7689.getVolt(i));
	}
	
	debug_printf("\n");
}
SpiSoft spi1;

void Ad7689testInit(void*param)
{	
	spi1.SetPin(PA5, PA6, PA7);
	spi1.SetNss(PA4);

	spi1.Init(CPOL_High, CPHA_2Edge);

	ad7689.SetSpi(&spi1);

	ad7689.Init();

	Sys.AddTask(Test123, NULL, 0, 1000, "测试");
	Sys.AddTask(adRouton123,0,0,10,"adRead");
}

#endif //_AD7689_TEST_H
