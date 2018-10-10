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

	if (rxFrame.dataLength >= 8)
	{	
		this->rxFrame.devid = this->rxFrame.data[0];
		this->rxFrame.fnCode = this->rxFrame.data[1];
		this->rxFrame.regAddr = this->rxFrame.data[2];
		this->rxFrame.regAddr <<= 8;
		this->rxFrame.regAddr |= this->rxFrame.data[3];
		this->rxFrame.regLength = this->rxFrame.data[4];
		this->rxFrame.regLength <<= 8;
		this->rxFrame.regLength |= this->rxFrame.data[5];

		debug_printf("devid:%d fnCode:%d regAddr:%d reglen:%d\n",this->rxFrame.devid,this->rxFrame.fnCode,this->rxFrame.regAddr,this->rxFrame.regLength);
		int needlen = 8;
		switch (this->rxFrame.fnCode)
		{
		case 0x0F://多个写 0 区输出继电器(指令代码: 0X0F) 
		case 0x10://多个写 4 区输出寄存器(指令代码: 0X10) 
			needlen = rxFrame.data[6] + needlen;
			break;
		default:
			break;
		}
		//需要的长度不够，直接返回
		if (needlen > this->rxFrame.dataLength)
			return false;

		auto crc11 = Crc::CRC16RTU(rxFrame.data, rxFrame.dataLength - 2);
		debug_printf("crc cal:%04X\n", crc11);

		//com.SendBytes(buf485, len);
		debug_printf("rcb one frame\n");
		Buffer bf(rxFrame.data, txFrame.dataLength);
		bf.ShowHex(true);
		debug_printf("datalen:%d crc:%04X \n",rxFrame.dataLength,rxFrame.checkSum);
		return rxFrame.VerifyCheckCode();
	}
	//return com.CheckFrame(rxFrame);
	return false;
}
bool DataFrameModbus::CheckFrame()
{

}

bool ModbusSlaveLink::Send()
{
	if (!txFrame.isUpdated) //no new frame data, no need to send
		return false;
	//if (!com.SendByte(txFrame.header))  //send frame header
	//	return false;
	if (txFrame.fnCode > MAX_FN_CODE || !com.SendByte(txFrame.fnCode))  //send function code
		return false;
	txFrame.dataLength = DATA_LENGTH[txFrame.fnCode][DIRECTION_SEND];
	if (!com.SendByte(txFrame.dataLength))  //send data length
		return false;
	if (!com.SendBytes(txFrame.data, txFrame.dataLength)) //send data;
		return false;
	txFrame.CreateCheckCode();
	if (!com.SendByte(txFrame.checkSum))    //send check code
		return false;
	txFrame.isUpdated = false;
	return true;
}
