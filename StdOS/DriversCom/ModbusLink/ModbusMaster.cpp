#include "ModbusLink.h"

ModbusMasterLink::ModbusMasterLink(USART &uart) :ModbusBase(uart)
{
	
}

bool ModbusMasterLink::GetValueRegInput(uint8_t id, uint16_t addr, uint16_t len)
{
	uint8_t buf[10];
	for (int i = 0; i < ArrayLength(buf); i++)
	{
		buf[i] = 'A' + i;
	}
	this->txFrame.Address = id;
	this->txFrame.regAddr = addr;
	this->txFrame.Length = len;
	this->com.SendBytes(buf, ArrayLength(buf));
	

	Sys.Sleep(100);
}