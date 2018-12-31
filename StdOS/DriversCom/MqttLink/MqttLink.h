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
	char * Server;//������
	int Port;//Զ�̶˿ں�
	char * ClientID;//�ͻ���id�����ܴ���23�ֽ�
	char* Topic;//����
	int8_t FixHead;//����ͷ	
	int step = 0;//���в���
	uint16_t MessageID;//�ź�ID
public:
	MqttLink(USART &uart);
	bool CheckFrame();
	bool Send();
	bool Connect();
	bool Puslish(uint8_t *buf,int len);//��������
	bool Puslish_Release();//����
private:
private:
	bool Receive();//��������

};


#endif // !_MQTT_H
