#ifndef _MQTT_H
#define _MQTT_H

#include "stm32f10x.h"
#include "FIFOBuffer.h"
#include "DataFrame.h"
#include "USART.h"

class MqttLink
{
public:
	DataFrame txFrame;
	DataFrame rxFrame;
	USART &com;
	char * Server;//服务器
	int Port;//远程端口号
	char * ClientID;//客户端id，不能大于23字节
	char* Topic;//主题
	int8_t FixHead;//报文头	
	int step = 0;//运行步骤
	uint16_t MessageID;//信号ID
public:
	MqttLink(USART &uart);
	bool CheckFrame();
	bool Send();
	bool Connect();
	bool Puslish(uint8_t *buf,int len);//发送数据
	bool Puslish_Release();//发布
private:
private:
	bool Receive();//接收数据

};


#endif // !_MQTT_H
