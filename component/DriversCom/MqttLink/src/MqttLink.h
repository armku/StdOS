#ifndef _MQTT_H
#define _MQTT_H

#include "stm32f10x.h"
#include "../../../../Bsp/Porting.h"
#include "../../DataFrame.h"

class HardwareSerial0_T;

class MqttLink
{
public:
	DataFrame txFrame;
	DataFrame rxFrame;
	HardwareSerial0_T &com;
	char * Server;//服务器
	int Port;//远程端口号
	char * ClientID;//客户端id，不能大于23字节
	char* Topic;//主题
	int8_t FixHead;//报文头	
	int step;//运行步骤
	uint16_t MessageID;//信号ID
public:
	MqttLink(HardwareSerial0_T &uart);
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
