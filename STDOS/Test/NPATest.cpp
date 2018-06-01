#include "Drivers\NPA.h"

//#define _NPA_TEST_CPP
#ifdef _NPA_TEST_CPP

CNPA npa(PB5, PB6);

void NPATestRoutin(void* param)
{
	int curP = npa.Read();
	//curP = ((float)(curP - 0x2900) / (0x3f00 - 0x2900));
	debug_printf("大气AD：%d", curP);
	debug_printf("大气压：%f", curP *1.0 * 0.007278646);
}

void NPATestInit()
{

	Sys.AddTask(NPATestRoutin, 0, 0, 100, "npatest");
}
#endif // !_NPA_TEST_CPP
