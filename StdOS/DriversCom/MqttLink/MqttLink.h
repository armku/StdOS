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
	FIFOBuffer<uint8_t, USART_RX_BUFFER_SIZE>  mRxBuf;  //USART Rx Buffer
	USART &com;
	char * Server;//服务器
	int Port;//远程端口号
	char * ClientID;//连接id
	int8_t FixHead;//报文头	
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
