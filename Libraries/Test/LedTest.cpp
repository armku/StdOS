#include "Port.h"
#include "Sys.h"

//#define _LEDTEST_CPP
#ifdef _LEDTEST_CPP
OutputPort led12(PC6, true);

void LedTask12(void *param)
{
	auto leds = (OutputPort*)param;
	*leds = !* leds;
}

void LedTestInit()
{
	Sys.AddTask(LedTask12, &led12, 0, 500, "LedTask");
}
#endif
