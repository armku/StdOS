#include "App/FlushPort.h"

#define _FLUSHPORTTEST_H

#ifdef _FLUSHPORTTEST_H

	void FlushRoutin(void * param)
	{
		FlushPort* bph=(FlushPort*)param;
		bph->Start();
	}
	OutputPort ledBlink(PB0, true);
	FlushPort bp;
	void FlushPortTest()
	{
		bp.Port =&ledBlink;
		bp.Start();
		Sys.AddTask(FlushRoutin,&bp,1000,5000,"blink");
	}
#endif
