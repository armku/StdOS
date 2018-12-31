#include "ModbusLink.h"
#include "Buffer.h"

ModbusMasterLink::ModbusMasterLink(USART &uart) :ModbusBase(uart)
{
	
}
//检查数据帧是否合法
bool ModbusMasterLink::CheckFrame()
{
	int rxlen = com.RxSize();

	if (com.GetBytes(&rxFrame.data[rxFrame.Length], rxlen))
	{
		rxFrame.Length += rxlen;
	}
	//判断数据帧最小长度要求
	if (rxFrame.Length < 8)
		return false;
#ifdef  DEBUG
	Buffer(rxFrame.data, rxFrame.Length).ShowHex(true);
#endif //  DEBUG	
	if (!rxFrame.CheckFrame())
		return false;
	else
		return true;
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
	this->txFrame.Crc2 = Crc::CRC16RTU(this->txFrame.data,6);
	this->txFrame.data[6] = this->txFrame.Crc2 & 0xff;
	this->txFrame.data[7] = this->txFrame.Crc2 >> 8;
	
	this->com.SendBytes(this->txFrame.data, 8);
	
	Sys.Sleep(200);

	if (com.RxSize() == 0)
	{
		return;
	}

	if (com.RxSize() > 0)
	{
		debug_printf("rxlen:%d\n", com.RxSize());
	}

	
	if ((com.RxSize() > 0) && CheckFrame())
	{
		debug_printf("rcv ok rxlen:%d-%d\n", com.RxSize(), rxFrame.regLength);
		Buffer bs(rxFrame.data, rxFrame.regLength);
		bs.ShowHex();
		this->com.ClearRxBuf();
		this->rxFrame.regLength = 0;

		return true;
	}
	else
	{
		this->com.ClearRxBuf();
		this->rxFrame.regLength = 0;
		debug_printf("rcv error\n");
		return false;
	}
}