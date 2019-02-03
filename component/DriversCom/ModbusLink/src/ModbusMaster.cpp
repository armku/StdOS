#include "ModbusLink.h"
#include "../../../../component/lib/Buffer.h"
#include "../../../../StdOS/Kernel/Sys.h"

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
	this->txFrame.Crc2 = CRC16RTU(this->txFrame.data,6);
	this->txFrame.data[6] = this->txFrame.Crc2 & 0xff;
	this->txFrame.data[7] = this->txFrame.Crc2 >> 8;
	
	this->com.SendBytes(this->txFrame.data, 8);
	
	Sys.Sleep(300);

	if (com.RxSize() == 0)
	{
		return false;
	}

	if ((com.RxSize() > 0) && CheckFrame())
	{
		for (int i = 0; i < len; i++)
		{
			this->RegInputs[0].Reg[addr + i] = this->rxFrame.data[3 + i * 2];
			this->RegInputs[0].Reg[addr + i] <<= 8;
			this->RegInputs[0].Reg[addr + i] += this->rxFrame.data[4 + i * 2];
		}
		debug_printf("rcv OK %d %d %d\n", this->RegInputs[0].Reg[0], this->RegInputs[0].Reg[1], this->RegInputs[0].Reg[2]);
		this->com.ClearRxBuf();
		this->rxFrame.regLength = 0;
		this->rxFrame.dataLength = 0;

		return true;
	}
	else
	{
		this->com.ClearRxBuf();
		this->rxFrame.regLength = 0;
		this->rxFrame.dataLength = 0;
		debug_printf("rcv error\n");
		return false;
	}
}
