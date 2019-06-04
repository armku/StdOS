#ifndef MQTT_H
#define MQTT_H

#include <stdint.h>

class MqttLink
{
public:
	char * Server;//������
	int Port;//Զ�̶˿ں�
	char * ClientID;//�ͻ���id�����ܴ���23�ֽ�
	char* Topic;//����
	int8_t FixHead;//����ͷ	
	int step;//���в���
	uint16_t MessageID;//�ź�ID
	bool CheckFrame();
	bool Send();
	bool Connect();
	bool Puslish(uint8_t *buf, int len);//��������
	bool Puslish(char *buf, int len)
	{
		return Puslish((uint8_t *)buf, len);
	}
	bool Puslish_Release();//����
	bool Subscribe(char* topc);//��������
private:
private:
	bool Receive();//��������
};

#endif // !MQTT_H
