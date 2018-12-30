#include <string.h>
#include "MqttLink.h"

MqttLink::MqttLink(USART &uart) :com(uart)
{
	com.ClearRxBuf();
	com.ClearTxBuf();
}

bool MqttLink::CheckFrame()
{
	return this->CheckFrame(rxFrame);
}
bool MqttLink::CheckFrame(DataFrame &df)
{
	int rxlen = com.RxSize();
	uint8_t ch;
	while (com.RxSize())
	{
		if (com.GetByte(ch))
		{
			mRxBuf.Put(ch);
		}
	}
	df.isUpdated = true;
	return true;
}
bool MqttLink::Get(uint8_t & da)
{
	return true;
}
bool MqttLink::Gets(uint8_t * pData, uint16_t num)
{
	return true;
}

bool MqttLink::Send()
{
	if (!txFrame.isUpdated) //no new frame data, no need to send
		return false;
	if (!com.SendByte(txFrame.header))  //send frame header
		return false;
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
bool MqttLink::Connect()
{	
	this->com.SendBytes(ClientID, strlen(ClientID));
}
