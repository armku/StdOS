#include "ModbusLink.h"

ModbusMasterLink::ModbusMasterLink(USART &uart) :ModbusBase(uart)
{
	
}

bool ModbusMasterLink::GetValueRegInput(uint8_t id, uint16_t addr, uint16_t len)
{
	this->txFrame.Address = id;
	this->txFrame.regAddr = addr;
	this->txFrame.regLength = len;
	this->txFrame.data[0] = this->txFrame.Address;
	this->txFrame.data[1] = 0x04;
	this->txFrame.data[2] = this->txFrame.regAddr >> 8;
	this->txFrame.data[3] = this->txFrame.regAddr & 0xff;
	this->txFrame.data[4] = this->txFrame.regLength >> 8;
	this->txFrame.data[5] = this->txFrame.regLength & 0xff;
	this->txFrame.Crc2 = Crc::CRC8(this->txFrame.data,6);
	this->txFrame.data[6] = this->txFrame.Crc2 & 0xff;
	this->txFrame.data[7] = this->txFrame.Crc2 >> 8;
	
	this->com.SendBytes(this->txFrame.data, 8);
	
	Sys.Sleep(100);
}