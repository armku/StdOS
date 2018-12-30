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
	this->txFrame.data[0] = this->FixHead;
	this->txFrame.data[1] = 0x15;
	this->txFrame.data[2] = 0x00;
	this->txFrame.data[3] = 0x04;
	this->txFrame.data[4] = 'M';
	this->txFrame.data[5] = 'Q';
	this->txFrame.data[6] = 'T';
	this->txFrame.data[7] = 'T';
	this->txFrame.data[8] = 0x04;
	this->txFrame.data[9] = 0x02;
	this->txFrame.data[10] = 0x00;
	this->txFrame.data[11] = 0x3C;
	this->txFrame.data[12] = 0x00;
	this->txFrame.data[13] = strlen(ClientID);
	for (int i = 0; i < strlen(ClientID); i++)
	{
		this->txFrame.data[14+i] = this->ClientID[i];
	}
	
	this->com.SendBytes(this->txFrame.data, 14+strlen(ClientID));
}
