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
	char * ClientID;//����id
	int8_t FixHead;//����ͷ	
	int step = 0;//���в���
public:
	MqttLink(USART &uart);
	bool CheckFrame();
	bool Send();
	bool Connect();
private:
	bool CheckFrame(DataFrame &df);
private:
	bool Get(uint8_t & da);
	bool Gets(uint8_t * pData, uint16_t num);

};


#endif // !_MQTT_H
