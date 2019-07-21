#include <string.h>
#include <stdio.h>
#include "MqttLink.h"
#include "Sys.h"
#include "Buffer.h"
/*
连接服务器 登录
客户ID:123456789
10 15 00 04 4D 51 54 54 04 02 00 3C 00 09 31 32 33 34 35 36 37 38 39 (23)
*/

MqttLink::MqttLink()
{
	this->FlagConnected = false;
	this->FlagConnectStep = 0;
}

bool MqttLink::Send()
{
	/*this->com.SendBytes(this->txFrame.data, this->txFrame.dataLength);*/
	//发送到远端数据
	this->send_buf(this->txFrame.data, this->txFrame.dataLength);
	return true;
}
//连接服务器
bool MqttLink::CONNECTServer()
{
	int connected = 1;

	this->txFrame.data[0] = this->FixHead;
	this->txFrame.data[1] = 12 + strlen(ClientID);
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
	//debug_printf("Send Login Command\r\n");
	delay(500);
	char bufneed[] = { 0x20,0x02,0x00,0x00 };
	if (this->readlen == 4)
	{
		//debug_printf("Rec Length 4\r\n");
		//20 02 00 00 (4)
		for (int i = 0; i < 4; i++)
		{
			if (this->bufRcv[i] != bufneed[i])
				connected = 0;
		}
		if (connected)
		{
			debug_printf("Login to Server OK\r\n");
			return true;
		}
		else
		{
			debug_printf("Login to Server ERROR\r\n");
		}
	}
	else
	{
		debug_printf("Login to Server rcv length %d ERROR\r\n", this->readlen);
	}
	return false;
}
bool MqttLink::Connect()
{
	switch (this->FlagConnectStep)
	{
	case 0:
		if (this->CONNECTServer())
		{
			this->FlagConnected = true;
			this->FlagConnectStep++;
		}
		break;
	case 1:
		break;
	default:
		break;
	}
	return false;
}
//发送数据
bool MqttLink::Puslish(char *buf, int len)
{
	int topticlen = strlen(this->Topic);

	this->txFrame.data[0] = 0X34;
	this->txFrame.data[1] = len + topticlen + 4;
	this->txFrame.data[2] = 0X00;
	this->txFrame.data[3] = topticlen;
	for (int i = 0; i < topticlen; i++)
	{
		this->txFrame.data[4 + i] = this->Topic[i];
	}

	this->txFrame.data[4 + topticlen + 0] = this->MessageID >> 8;
	this->txFrame.data[4 + topticlen + 1] = this->MessageID & 0xff;
	
	for (int i = 0; i < len; i++)
	{
		this->txFrame.data[6 + topticlen + i] = buf[i];
	}
	this->txFrame.dataLength = 6 + topticlen + len;

	this->Send();
	delay(100);
	char bufneed[] = { 0x50,0x02,0x00,0x00 };//响应，最后一个字节为数据帧流水号
	if (this->readlen == 4)
	{
		debug_printf("Puslish Rec Length 4\r\n");
		Buffer(this->bufRcv, this->readlen).ShowHex(true);
		for (int i = 0; i < 3; i++)
		{
			if (this->bufRcv[i] != bufneed[i])
				return false;
		}
		return true;
	}
	else
	{
		debug_printf("Puslish to Server rcv length %d ERROR\r\n", this->readlen);
		Buffer(this->bufRcv, this->readlen).ShowHex(true);
	}
	return false;
}
//发布
bool MqttLink::Puslish_Release()
{
	byte buf[4];
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
	delay(100);
	char bufneed[] = { 0x50,0x02,0x00,0x00 };//响应，最后一个字节为数据帧流水号
	if (this->readlen == 4)
	{
		debug_printf("Puslish_Release Rec Length 4\r\n");
		Buffer(this->bufRcv, this->readlen).ShowHex(true);
		//20 02 00 00 (4)
		for (int i = 0; i < 3; i++)
		{
			if (this->bufRcv[i] != bufneed[i])
				return false;
		}
		return true;
	}
	else
	{
		debug_printf("Puslish to Server rcv length %d ERROR\r\n", this->readlen);
		Buffer(this->bufRcv, this->readlen).ShowHex(true);
	}
	return false;
}
//发送并发布数据
bool MqttLink::PuslishAndRelease(char* buf, int len)
{
	if (this->Puslish(buf, len))
	{
		delay(50);
		return this->Puslish_Release();
	}
	else
	{
		return false;
	}
}
//订阅主题
bool MqttLink::Subscribe(char* topc)
{

	return 1;
}
