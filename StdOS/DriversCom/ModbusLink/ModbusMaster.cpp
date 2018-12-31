#include "ModbusLink.h"

ModbusMasterLink::ModbusMasterLink(USART &uart) :ModbusBase(uart)
{
	
}

bool ModbusMasterLink::GetValue(uint8_t id, uint16_t addr, uint16_t len)
{
	uint8_t buf[10];
	this->txFrame.Address = id;
	this->txFrame.regAddr = addr;
	this->txFrame.Length = len;
	

	Sys.Sleep(100);
}