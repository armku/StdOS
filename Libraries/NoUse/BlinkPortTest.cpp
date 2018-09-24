#include "App/BlinkPort.h"

//#define _BLINKPORT_TEST_H

#ifdef _BLINKPORT_TEST_H
	void BlinkRoutin(void * param)
	{
		BlinkPort* bph=(BlinkPort*)param;
		bph->Start();
	}
	OutputPort ledBlink(PB0, true);
	BlinkPort bp;
	void BlinkPortTest()
	{
		bp.Add(&ledBlink);
		bp.Start();
		Sys.AddTask(BlinkRoutin,&bp,1000,3000,"blink");
	}

#endif
