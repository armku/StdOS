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
	char * Server;//������
	int Port;//Զ�̶˿ں�
	char * ClientID;//�ͻ���id�����ܴ���23�ֽ�
	char* Topic;//����
	int8_t FixHead;//����ͷ	
	int step;//���в���
	uint16_t MessageID;//�ź�ID
public:
	MqttLink(HardwareSerial0_T &uart);
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
