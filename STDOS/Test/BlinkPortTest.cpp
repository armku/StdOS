#include "App/BlinkPort.h"

#define _BLINKPORT_H

#ifdef _BLINKPORT_H
	OutputPort ledBlink(PB0, true);
	BlinkPort bp;
	void BlinkPortTest()
	{
		bp.Add(&ledBlink);
		bp.Start();
	}

#endif
