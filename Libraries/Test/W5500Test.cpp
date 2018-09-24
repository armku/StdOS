#include "Drivers\W5500.h"

#define _W5500_TEST_CPP
#ifdef _W5500_TEST_CPP

void W5500Routin(void * param)
{
	//debug_printf("hello \n");
}

void W5500Test()
{
	Sys.AddTask(W5500Routin, 0, 0, 1000, "W5500Routin");
}
#endif
