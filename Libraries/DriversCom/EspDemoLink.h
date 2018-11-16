#ifndef _ESPDEMOLINK_H
#define _ESPDEMOLINK_H

#include "stm32f10x.h"
#include "FIFOBuffer.h"
#include "USART.h"

class EspDemoLink
{
public:
	DataFrame txFrame;
	DataFrame rxFrame;
	USART &com;
	OutputPort pinch;
	OutputPort pinrst;
public:
	EspDemoLink(USART &uart);
	bool CheckFrame();
	bool Send();
private:

};


#endif // !_ESPDEMOLINK_H
