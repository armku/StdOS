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
	uint8_t data[MAX_FRAME_DATA_LENGTH];//��ǰ����
	bool isUpdated;
	uint8_t dataLength; //��ǰ���ݳ���
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
	DataFrame txFrame;
	DataFrame rxFrame;
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

	pFun_UART_buf send_buf;		// ���ͻ�����
private:
private:
	bool Receive();//��������
};

#define ArrayLength(arr) (int)(sizeof(arr)/sizeof(arr[0]))

#endif // !MQTT_H
