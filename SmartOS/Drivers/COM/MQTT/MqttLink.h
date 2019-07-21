#ifndef MQTT_H
#define MQTT_H

//#include <stdint.h>
#include "../HAL/STM32F1/ARCH_UART.h"

#define MAX_FRAME_DATA_LENGTH 200

struct DataFrame
{
public:
	byte data[MAX_FRAME_DATA_LENGTH];//��ǰ����
	byte dataLength; //��ǰ���ݳ���
public:	
};

class MqttLink
{
public:
	MqttLink();
	DataFrame txFrame;
	DataFrame rxFrame;
	char * Server;//������
	int Port;//Զ�̶˿ں�
	char * ClientID;//�ͻ���id�����ܴ���23�ֽ�
	char* Topic;//����
	byte FixHead;//����ͷ	
	int step;//���в���
	ushort MessageID;//�ź�ID
	bool Send();
	bool Connect();
	bool Puslish(char *buf, int len);//��������
	bool PuslishAndRelease(char* buf, int len);//���Ͳ���������
	bool Puslish(byte*buf, int len)
	{
		return Puslish((char *)buf, len);
	}
	bool PuslishAndRelease(byte* buf, int len)
	{
		return this->PuslishAndRelease((char*)buf,len);
	}
	bool Puslish_Release();//����
	bool Subscribe(char* topc);//��������

	bool CONNECTServer();//���ӷ�����

	pFun_UART_buf send_buf;		// ���ͻ�����
	int FlagConnected;			// ���ӷ������ɹ�
	int FlagConnectStep;		// ���ӷ���������
	char bufRcv[100];			// ���ջ�����
	int readlen;//��ȡ�Ļ���������
private:
};

#define ArrayLength(arr) (int)(sizeof(arr)/sizeof(arr[0]))

#endif // !MQTT_H
