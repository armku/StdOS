#ifndef MQTT_H
#define MQTT_H

#include <stdint.h>

class MqttLink
{
public:
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
	bool Puslish(uint8_t *buf, int len);//发送数据
	bool Puslish(char *buf, int len)
	{
		return Puslish((uint8_t *)buf, len);
	}
	bool Puslish_Release();//发布
	bool Subscribe(char* topc);//订阅主题
private:
private:
	bool Receive();//接收数据
};

#endif // !MQTT_H
