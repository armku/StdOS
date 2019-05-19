#ifndef _ESP8266_H
#define _ESP8266_H

#include "Port.h"
#include "../HAL/STM32F1/ARCH_UART.h"

class Esp8266
{
public:
	Esp8266();
	~Esp8266();

	void SetPinCH_PD(Pin p);
	void SetPinRST(Pin p);

	void Cmd(char* cmd, int len);

	void Init();
	void AT();
	bool ESP8266_Cmd(char* cmd, char* reply1, char* reply2, uint32_t waittime);
	void Cmd(char* cmd);
	int step;//������

	char* Reply1;//�ڴ��ظ�1
	char* Reply2;//�ڴ��ظ�2
	uint32_t WaitTime;//�ȴ�ͨ����Ӧʱ��-ms
//private:
	Port CH_PD;//
	Port RST;//
	pFun_UART send_byte;        // ���ڷ���һ�ֽ�����
	pFun_UART_buf send_buf;		// ���ͻ�����
};



#endif // !_ESP8266_H
