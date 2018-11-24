#ifndef _DEMOLINK_H
#define _DEMOLINK_H

#include "stm32f10x.h"
#include "FIFOBuffer.h"
#include "DataFrame.h"
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
	bool CheckFrame(DataFrame &df);
private:
	bool Get(uint8_t & da);
	bool Gets(uint8_t * pData, uint16_t num);

};

#endif // !_DEMOLINK_H
