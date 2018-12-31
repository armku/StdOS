#include "ModbusLink.h"

ModbusMasterLink::ModbusMasterLink(USART &uart) :ModbusBase(uart)
{
	
}

bool ModbusMasterLink::GetValue()
{

	Sys.Sleep(100);
}