#include "AD7689.h"

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

void Ad7689testInit(void*param)
{
	ad7689.SetPin(PB13, PB15, PB14, PB12);
	ad7689.Init();

	Sys.AddTask(Test123, NULL, 0, 1000, "²âÊÔ");
	Sys.AddTask(adRouton123,0,0,10,"adRead");
}

#endif //_AD7689_TEST_H
