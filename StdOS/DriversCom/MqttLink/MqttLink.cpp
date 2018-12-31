#include <string.h>
#include "MqttLink.h"
#include "Sys.h"
#include "Buffer.h"

MqttLink::MqttLink(USART &uart) :com(uart)
{
	com.ClearRxBuf();
	com.ClearTxBuf();
}

bool MqttLink::CheckFrame()
{
	int rxlen = com.RxSize();

	if (com.GetBytes(&rxFrame.data[rxFrame.dataLength], rxlen))
	{
		rxFrame.dataLength += rxlen;
	}
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
	this->com.SendBytes(this->txFrame.data, this->txFrame.dataLength);
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
		this->txFrame.data[14 + i] = this->ClientID[i];
	}
	this->txFrame.dataLength = 14 + strlen(ClientID);

	this->Send();
	Sys.Sleep(200);
	
	if (com.RxSize() > 0)
	{
		debug_printf("rxlen:%d\n",com.RxSize());		
	}

	if ((com.RxSize() > 0) && CheckFrame())
	{
		debug_printf("rcv ok rxlen:%d-%d\n", com.RxSize(),rxFrame.dataLength);
		Buffer bs(rxFrame.data, rxFrame.dataLength);
		bs.ShowHex();
		this->com.ClearRxBuf();
		this->rxFrame.dataLength = 0;
		
		return true;
	}
	else
	{
		debug_printf("rcv error\n");
		return false;
	}
}
//发送数据
bool MqttLink::Send(uint8_t *buf, int len)
{
	uint8_t bufsend[11];
	bufsend[0] = 0X34;
	bufsend[1] = 0X0C;
	bufsend[2] = 0X00;
	bufsend[3] = 0X05;
	bufsend[4] = 0X47;
	bufsend[5] = 0X2F;
	bufsend[6] = 0X64;
	bufsend[7] = 0X64;
	bufsend[8] = 0X64;
	bufsend[9] = this->MessageID >> 8;
	bufsend[10] = this->MessageID & 0xff;


	for (int i = 0; i < 11; i++)
	{
		this->txFrame.data[i] = bufsend[i];
	}
	for (int i = 0; i < len; i++)
	{
		this->txFrame.data[12 + i] = buf[i];
	}
	this->txFrame.dataLength = 12+ len;

	this->Send();

	Sys.Sleep(200);
	this->Puslish_Release();
	return true;
}
//发布
bool MqttLink::Puslish_Release()
{
	uint8_t buf[4];
	buf[0] = 0X62;
	buf[1] = 0X02;
	buf[2] = this->MessageID >> 8;
	buf[3] = this->MessageID & 0XFF;

	this->MessageID++;

	for (int i = 0; i < ArrayLength(buf); i++)
	{
		this->txFrame.data[i] = buf[i];
	}
	this->txFrame.dataLength = ArrayLength(buf);

	this->Send();
}
