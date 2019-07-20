#include "USART.h"
#include "FIFOBuffer.h"
#include "DataFrameBr.h"

#ifndef _BRLINK_H
#define _BRLINK_H

class BrLink
{
public:
	DataFrameBr txFrame;
	DataFrameBr rxFrame;
	USART &com;
public:
	BrLink(USART &uart);
	bool CheckFrame();
	bool Send();
};

#endif // !_BRLINK_H
