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
#include <stdio.h>

EspDemoLink::EspDemoLink(USART &uart) :com(uart)
{
	com.ClearRxBuf();
	com.ClearTxBuf();
}
bool EspDemoLink::Kick11(void)
{
	com.ClearRxBuf();
	com.ClearTxBuf();
	com << "AT\r\n";

	Sys.Sleep(200);

	this->Receive11();
}
bool EspDemoLink::Receive11(char const *target)
{
	if (com.RxSize() > 0)
	{
		//debug_printf("rxlen:%d\n", com.RxSize());
	}

	if ((com.RxSize() > 0) && CheckFrame())
	{
		//debug_printf("rcv ok rxlen:%d-%d\n", com.RxSize(), rxFrame.dataLength);
		Buffer bs(rxFrame.data, rxFrame.dataLength);
		//bs.ShowHex();
		bs.Show();
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
/*
 * 函数名：ESP8266_Net_Mode_Choose
 * 描述  ：选择WF-ESP8266模块的工作模式
 * 输入  ：enumMode，工作模式
 * 返回  : 1，选择成功
 *         0，选择失败
 * 调用  ：被外部调用
 */
bool EspDemoLink::NetModeChoose(ENUMNetModeTypeDef enumMode)
{
	com.ClearRxBuf();
	com.ClearTxBuf();
	
	switch (enumMode)
	{
	case STA:
		//return this->Cmd("AT+CWMODE=1", "OK", "no change", 2500);
		com << "AT+CWMODE=1\r\n";
	case AP:
		//return this->Cmd("AT+CWMODE=2", "OK", "no change", 2500);
		com << "AT+CWMODE=2\r\n";
	case STA_AP:
		//return this->Cmd("AT+CWMODE=3", "OK", "no change", 2500);
		com << "AT+CWMODE=3\r\n";
	default:
		return false;
	}

	Sys.Sleep(200);

	this->Receive11();
}
/*
 * 函数名：ESP8266_JoinAP
 * 描述  ：WF-ESP8266模块连接外部WiFi
 * 输入  ：pSSID，WiFi名称字符串
 *       ：pPassWord，WiFi密码字符串
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
 */
bool EspDemoLink::JoinAP(char *pSSID, char *pPassWord)
{
	char cCmd[120];
	sprintf(cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord);
	
	Sys.Sleep(200);

	this->Receive11();
	
	//return this->Cmd(cCmd, "OK", NULL, 5000);
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
	//Sys.Sleep(100);
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
	this->pinch.Set(PG13);
	this->pinch.Invert = false;
	this->pinch.OpenDrain = false;
	this->pinch.Open();
	this->pinch = 0;

	this->pinrst.Set(PG14);
	this->pinrst.Invert = false;
	this->pinrst.OpenDrain = false;
	this->pinrst.Open();
	this->pinrst = 0;
}
void EspDemoLink::Init()
{
	Sys.Delay(500);
	this->pinch = 1;
	this->pinrst = 1;
}
