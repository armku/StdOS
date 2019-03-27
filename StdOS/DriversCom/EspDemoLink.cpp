#include "EspDemoLink.h"
#include "Sys.h"
#include <string.h>  
#include "OnChip/USART.h"
#include "BspPlatform/Interrupt.h"
#include "BspPlatform\BspPlatform.h"
#include "OnChip\Port.h"
#include "OnChip\Configuration.h"
#include "Buffer.h"
#include <stdio.h>  
#include <string.h>  
#include "Sys.h"

EspDemoLink::EspDemoLink(USART &uart) :com(uart)
{
	com.ClearRxBuf();
	com.ClearTxBuf();
}
bool EspDemoLink::Kick(void)
{
	com.ClearRxBuf();
	com.ClearTxBuf();
	com << "AT\r\n";
	return RecvFind("OK");
}
bool EspDemoLink::RecvFind(char const *target, unsigned char timeout)
{
	if (!ReceiveAndWait((char*)target, timeout))
		return false;
	return true;
}
void EspDemoLink::ClearBuffer()
{
	unsigned int i = 0;
	for (; i < ESP8266_RECEIVE_BUFFER_SIZE; ++i)
		mReceiveBuffer[i] = 0;
}
bool EspDemoLink::ReceiveAndWait(const char* targetString, unsigned char timeOut)
{
	u8 temp;
	mReceiveBufferIndex = 0;
	ClearBuffer();
	double tartTime = Sys.Ms();
	while ((Sys.Ms() - tartTime) < timeOut)
	{
		while (com.RxSize() > 0)
		{
			com.GetBytes(&temp, 1);//从串口接收缓冲区接收数据
			if (temp == '\0')
				continue;
			mReceiveBuffer[mReceiveBufferIndex++] = temp;//放入esp的缓冲区

		}

		if (strstr(mReceiveBuffer, targetString))
			return true;

	}
	if (mReceiveBufferIndex > 0)//接收到了数据，加上结束标志
	{
		mReceiveBuffer[mReceiveBufferIndex] = '\0';
	}
	return false;
}
bool EspDemoLink::CheckFrame()
{
	int rxlen = com.RxSize();

	if (com.GetBytes(&rxFrame.data[rxFrame.dataLength], rxlen))
	{
		rxFrame.dataLength += rxlen;
	}
	if (!rxFrame.CheckFrame())
		return false;
	else 
		return true;
}

bool EspDemoLink::Send()
{
	if (!txFrame.isUpdated) //no new frame data, no need to send
		return false;
	//if (!com.SendByte(txFrame.header))  //send frame header
	//	return false;
	//if (txFrame.fnCode > MAX_FN_CODE || !com.SendByte(txFrame.fnCode))  //send function code
	//	return false;
	//txFrame.dataLength = DATA_LENGTH[txFrame.fnCode][DIRECTION_SEND];
	//if (!com.SendByte(txFrame.dataLength))  //send data length
	//	return false;
	//if (!com.SendBytes(txFrame.data, txFrame.dataLength)) //send data;
	//	return false;
	//txFrame.CreateCheckCode();
	//if (!com.SendByte(txFrame.checkSum))    //send check code
	//	return false;
	txFrame.isUpdated = false;
	return true;
}
void EspDemoLink::cmd(char *cmd)
{
	static char buf[100];
	static int len = 0;
	len = sprintf(buf, "%s\r\n", cmd);
	this->com.SendBytes(buf, len);
}
void EspDemoLink::EchoOn(bool on)
{
	this->com << "ATE" << (on ? 1 : 0) << "\r\n";
}
void EspDemoLink::SetPin(Pin pch, Pin prst)
{
	this->pinch.SetPin(PG13);
	this->pinch.pinMode(GPIO_Out_PP);
	this->pinch = 0;

	this->pinrst.SetPin(PG14);
	this->pinrst.pinMode(GPIO_Out_PP);
	this->pinrst = 0;
}
void EspDemoLink::Init()
{
	delayMicroseconds(500);
	this->pinch = 1;
	this->pinrst = 1;
}
