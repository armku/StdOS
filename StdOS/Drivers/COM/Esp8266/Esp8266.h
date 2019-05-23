#ifndef _ESP8266_H
#define _ESP8266_H

#include "Port.h"
#include "../HAL/STM32F1/ARCH_UART.h"

/******************************* ESP8266 �������Ͷ��� ***************************/
typedef enum {
	STA,
	AP,
	STA_AP
} ENUM_Net_ModeTypeDef;


typedef enum {
	enumTCP,
	enumUDP,
} ENUM_NetPro_TypeDef;


typedef enum {
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;


typedef enum {
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;

typedef enum { DISABLE = 0, ENABLE = !DISABLE } FunctionalState;

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
	bool ESP8266_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode);
	bool ESP8266_JoinAP(char* pSSID, char* pPassWord);
	bool ESP8266_Enable_MultipleId(FunctionalState enumEnUnvarnishTx);
	bool ESP8266_Link_Server(ENUM_NetPro_TypeDef enumE, char* ip, char* ComNum, ENUM_ID_NO_TypeDef id);
	bool ESP8266_UnvarnishSend(void);
	void ESP8266_ExitUnvarnishSend(void);
	void ESP8266_SendStr(char* str);//�����ַ���

	bool ESP8266_Cmd(char* cmd, char* reply1, char* reply2, uint32_t waittime);
	void Cmd(char* cmd);
	bool DealBufIn(char* buf, int len);//������յ�������
	bool IsReply1(char* buf);
	bool IsReply2(char* buf);
	bool IsReply(char* buf);
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
