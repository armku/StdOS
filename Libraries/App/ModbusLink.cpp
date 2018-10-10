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
	rxFrame.CheckFrame();
	debug_printf("devid:%d fnCode:%d regAddr:%d reglen:%d\n", rxFrame.devid, rxFrame.fnCode, rxFrame.regAddr, rxFrame.regLength);
	/*auto crc11 = Crc::CRC16RTU(rxFrame.data, rxFrame.dataLength - 2);
	debug_printf("crc cal:%04X\n", crc11);*/
	debug_printf("rcb one frame\n");
	debug_printf("datalen:%d crc:%04X \n", rxFrame.dataLength, rxFrame.checkSum);
	Buffer bf(rxFrame.data, rxFrame.dataLength);
	bf.ShowHex(true);
	//com.SendBytes(buf485, len);

	return rxFrame.CheckFrame();
}
bool DataFrameModbus::CheckFrame()
{
	if (dataLength >= 8)
	{
		this->devid = this->data[0];
		this->fnCode = this->data[1];
		this->regAddr = this->data[2];
		this->regAddr <<= 8;
		this->regAddr |= this->data[3];
		this->regLength = this->data[4];
		this->regLength <<= 8;
		this->regLength |= this->data[5];
				
		this->frameLength = 8;
		switch (this->fnCode)
		{
		case 0X02:
		case 0X01:
		case 0X05:
		case 0X04:
		case 0X03:
		case 0X06:
			frameLength = 8;
			break;
		case 0x0F://多个写 0 区输出继电器(指令代码: 0X0F) 
		case 0x10://多个写 4 区输出寄存器(指令代码: 0X10) 
			frameLength = data[6] + frameLength;
			break;
		default:
			frameLength = 0;
			break;
		}
		if (frameLength == 0)
		{
			//非法指令，直接清空缓冲区
			frameLength = this->dataLength;
			this->RemoveOneFrame();
			return false;
		}
		//需要的长度不够，直接返回
		if (frameLength > this->dataLength)
			return false;
		this->checkSum = this->data[this->frameLength-1];
		this->checkSum <<= 8;
		this->checkSum |= this->data[this->frameLength - 2];
				
		if (this->VerifyCheckCode())
		{
			return true;
		}
		else
		{
			this->RemoveOneFrame();
			return false;
		}
	}
	return false;
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
