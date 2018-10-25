#include "ModbusLink.h"
#include "Buffer.h"

ModbusSlaveLink::ModbusSlaveLink(USART &uart) :com(uart)
{

}

bool ModbusSlaveLink::CheckFrame()
{
	int rxlen = com.RxSize();

	if (com.GetBytes(&rxFrame.data[rxFrame.dataLength], rxlen))
	{
		rxFrame.dataLength += rxlen;
	}
	if (!rxFrame.CheckFrame())
		return false;
	debug_printf("devid:%d fnCode:%d regAddr:%d reglen:%d framelen:%d buflen:%d\n", rxFrame.devid, 
		rxFrame.fnCode, 
		rxFrame.regAddr, 
		rxFrame.regLength,
		rxFrame.frameLength,
		rxFrame.dataLength);
	if (rxFrame.frameLength > 3)
	{
		auto crc11 = Crc::CRC16RTU(rxFrame.data, rxFrame.frameLength - 2);
		debug_printf("crc cal:%04X rcv:%04X\n", crc11, rxFrame.checkSum);
	}
	Buffer(rxFrame.data, rxFrame.dataLength).ShowHex(true);
	//com.SendBytes(buf485, len);

	return rxFrame.CheckFrame();
}

bool ModbusSlaveLink::Send()
{
	if (!txFrame.isUpdated) //no new frame data, no need to send
		return false;	
	txFrame.data[0] = txFrame.devid;
	txFrame.data[1] = txFrame.fnCode;
	auto crc = Crc::CRC16RTU(txFrame.data, txFrame.frameLength - 2);
	txFrame.data[txFrame.frameLength - 2] = crc & 0xff;
	txFrame.data[txFrame.frameLength - 1] = crc >> 8;
	com.SendBytes(txFrame.data, txFrame.frameLength);
	txFrame.isUpdated = false;
	rxFrame.RemoveOneFrame();//移除处理完的接收数据帧
	return true;
}
