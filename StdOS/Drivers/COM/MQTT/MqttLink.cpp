#include <string.h>
#include <stdio.h>
#include "MqttLink.h"
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

bool MqttLink::CheckFrame()
{
	/*int rxlen = com.RxSize();*/

	/*if (com.GetBytes(&rxFrame.data[rxFrame.dataLength], rxlen))
	{
		rxFrame.dataLength += rxlen;
	}*/
}

bool MqttLink::Send()
{
	/*this->com.SendBytes(this->txFrame.data, this->txFrame.dataLength);*/
	//发送到远端数据
	this->send_buf(this->txFrame.data, this->txFrame.dataLength);
	return true;
}
bool MqttLink::Connect()
{
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
	//while (true)
	{
		this->Send();
		/*Sys.Sleep(200);*/
		//vTaskDelay(500);
		this->Receive();
	}
}
//接收数据
bool MqttLink::Receive()
{
	/*if (com.RxSize() > 0)
	{
		debug_printf("rxlen:%d\n", com.RxSize());
	}

	if ((com.RxSize() > 0) && CheckFrame())
	{
		debug_printf("rcv ok rxlen:%d-%d\n", com.RxSize(), rxFrame.dataLength);
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
	}*/
}
int icnttest = 0;
//发送数据
bool MqttLink::Puslish(uint8_t *buf, int len)
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


	for (int i = 0; i < 100; i++)
		this->txFrame.data[i] = 0;
	sprintf((char*)(this->txFrame.data), "%d 发送测试\r\n\0", icnttest++);
	printf("%d 发送测试\r\n", icnttest);
	this->txFrame.dataLength = strlen((char*)this->txFrame.data);
	this->Send();
	///*Sys.Sleep(200);*/
	//this->Receive();


	//this->Puslish_Release();
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
	/*Sys.Sleep(200);*/
	this->Receive();
}
//订阅主题
bool MqttLink::Subscribe(char* topc)
{

	return 1;
}


