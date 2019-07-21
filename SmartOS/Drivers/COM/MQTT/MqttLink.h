#ifndef MQTT_H
#define MQTT_H

//#include <stdint.h>
#include "../HAL/STM32F1/ARCH_UART.h"

#define MAX_FRAME_DATA_LENGTH 200

struct DataFrame
{
public:
	byte data[MAX_FRAME_DATA_LENGTH];//当前数据
	byte dataLength; //当前数据长度
public:	
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
	byte FixHead;//报文头	
	int step;//运行步骤
	ushort MessageID;//信号ID
	bool Send();
	bool Connect();
	bool Puslish(char *buf, int len);//发送数据
	bool PuslishAndRelease(char* buf, int len);//发送并发布数据
	bool Puslish(byte*buf, int len)
	{
		return Puslish((char *)buf, len);
	}
	bool PuslishAndRelease(byte* buf, int len)
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
};

#define ArrayLength(arr) (int)(sizeof(arr)/sizeof(arr[0]))

#endif // !MQTT_H
