#include "USART.h"
#include "FIFOBuffer.h"
#include "DataFrameBrSensor.h"

#ifndef _BRSENSORLILNK_H
#define _BRSENSORLILNK_H

class BrSensorLink
{
public:
	DataFrameBrSensor txFrame;
	DataFrameBrSensor rxFrame;
	USART &com;
public:
	BrSensorLink(USART &uart);
	bool CheckFrame();
	bool Send();
};

#endif // !_BRSENSORLILNK_H
