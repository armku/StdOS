#include "DataFrameModbus.h"
#include "stm32f10x.h"
#include "FIFOBuffer.h"
#include "USART.h"

#ifndef _MODBUSLINK_H
#define _MODBUSLINK_H

class ModbusSlaveLink
{
public:
	DataFrameModbus txFrame;
	DataFrameModbus rxFrame;
	USART &com;
public:
	ModbusSlaveLink(USART &uart);
	bool CheckFrame();
	bool Send();
private:

};

#endif // !_MODBUSLINK_H
