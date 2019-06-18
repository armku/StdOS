#ifndef MQTT_H
#define MQTT_H

#include <stdint.h>
#include "../HAL/STM32F1/ARCH_UART.h"

#define MAX_FRAME_DATA_LENGTH 200
#define FRAME_HEADER 0xAA

//frame struct:
// | header | fnCode |  dataLength | data1 | ...... | datan | checkSum | 

struct DataFrame
{
public:
	const uint8_t header;
	uint8_t fnCode;
	uint8_t checkSum;
	uint8_t data[MAX_FRAME_DATA_LENGTH];//当前数据
	bool isUpdated;
	uint8_t dataLength; //当前数据长度
public:
	DataFrame() :header(FRAME_HEADER)                //constructor
	{
		isUpdated = false;
	}

	bool VerifyCheckCode()
	{
		uint8_t code = header + fnCode + dataLength;
		for (uint8_t i = 0; i < dataLength; i++)
			code += data[i];
		if (code == checkSum)
			return true;
		else
			return false;
	}
	void CreateCheckCode()
	{
		checkSum = header + fnCode + dataLength;
		for (uint8_t i = 0; i < dataLength; i++)
			checkSum += data[i];
	}
	DataFrame& operator=(const DataFrame &df)
	{
		//header = df.header;
		fnCode = df.fnCode;
		dataLength = df.dataLength;
		isUpdated = df.isUpdated;
		checkSum = df.checkSum;
		for (uint8_t i = 0; i < dataLength; i++)
			data[i] = df.data[i];
		return *this;
	}
};

class MqttLink
{
public:
	MqttLink();
	DataFrame txFrame;
	DataFrame rxFrame;
	char * Server;//服务器
	int Port;//远程端口号
	char * ClientID;//客户端id，不能大于23字节
	char* Topic;//主题
	int8_t FixHead;//报文头	
	int step;//运行步骤
	uint16_t MessageID;//信号ID
	bool CheckFrame();
	bool Send();
	bool Connect();
	bool Puslish(char *buf, int len);//发送数据
	bool PuslishAndRelease(char* buf, int len);//发送并发布数据
	bool Puslish(uint8_t*buf, int len)
	{
		return Puslish((char *)buf, len);
	}
	bool PuslishAndRelease(uint8_t* buf, int len)
	{
		return this->PuslishAndRelease((char*)buf,len);
	}
	bool Puslish_Release();//发布
	bool Subscribe(char* topc);//订阅主题

	bool CONNECTServer();//连接服务器

	pFun_UART_buf send_buf;		// 发送缓冲区
	int FlagConnected;			// 连接服务器成功
	int FlagConnectStep;		// 连接服务器步骤
	char bufRcv[100];			// 接收缓冲区
	int readlen;//读取的缓冲区长度
private:
private:
	bool Receive();//接收数据
};

#define ArrayLength(arr) (int)(sizeof(arr)/sizeof(arr[0]))

#endif // !MQTT_H
