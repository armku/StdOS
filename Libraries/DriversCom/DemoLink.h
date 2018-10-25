#ifndef _DEMOLINK_H
#define _DEMOLINK_H

#include "stm32f10x.h"
#include "FIFOBuffer.h"
#include "USART.h"

class DemoLink
{
public:
	DataFrame txFrame;
	DataFrame rxFrame;
	USART &com;
public:
	DemoLink(USART &uart);
	bool CheckFrame();
	bool Send();
private:

};

#endif // !_DEMOLINK_H