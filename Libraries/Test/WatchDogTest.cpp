#include "WatchDog.h"
#include "Sys.h"

//#define _WATCHDOGTEST_CPP
#ifdef _WATCHDOGTEST_CPP

void WatchDogTestInit()
{
#ifdef DEBUG
	WatchDog::Start(20000, 10000);
#else 
	WatchDog::Start();
#endif 
}
#endif
