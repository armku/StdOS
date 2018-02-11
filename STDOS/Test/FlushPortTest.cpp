#include "App/FlushPort.h"

//#define _FLUSHPORT_TEST_H

#ifdef _FLUSHPORT_TEST_H

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
