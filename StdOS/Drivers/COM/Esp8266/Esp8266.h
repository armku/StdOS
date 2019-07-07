#ifndef _ESP8266_H
#define _ESP8266_H

#include "Port.h"
#include "../HAL/STM32F1/ARCH_UART.h"

/******************************* ESP8266 数据类型定义 ***************************/
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

	void Connect();//连接服务器

	void SetPinCH_PD(Pin p);
	void SetPinRST(Pin p);

	void Cmd(char* cmd, int len);

	void Init();
	void Reset();//复位
	bool AT();
	bool ESP8266_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode);
	bool ESP8266_JoinAP(char* pSSID, char* pPassWord);
	bool ESP8266_Enable_MultipleId(FunctionalState enumEnUnvarnishTx);
	bool ESP8266_Link_Server(ENUM_NetPro_TypeDef enumE, char* ip, char* ComNum, ENUM_ID_NO_TypeDef id);
	bool ESP8266_UnvarnishSend(void);//进入透传模式
	void ESP8266_ExitUnvarnishSend(void);
	bool ESP8266_SendStr(char* str);//发送字符串
	bool ESP8266_SendString(FunctionalState enumEnUnvarnishTx, char* pStr, uint32_t ulStrLength, ENUM_ID_NO_TypeDef ucId);
	bool SoftReset();//软件复位

	bool ESP8266_Cmd(char* cmd, char* reply1, char* reply2, uint32_t waittime);
	void Cmd(char* cmd);
	bool DealRespose(char* buf, int len);//处理接收到的数据
	bool IsReply1(char* buf);	// 
	bool IsReply2(char* buf);	// 
	bool IsReplyFail(char* buf);	// 
	bool IsReply(char* buf, char* reply);
	
	char* Reply1;//期待回复1
	char* Reply2;//期待回复2
	uint32_t WaitTime;//等待通信响应时间-ms
//private:
	Port CH_PD;//
	Port RST;//
	pFun_UART send_byte;        // 串口发送一字节数据
	pFun_UART_buf send_buf;		// 发送缓冲区
	char bufRcv[100];			// 接收缓冲区
	int readlen;//读取的缓冲区长度
	int FlagConnected;			// 连接服务器成功
	int FlagConnectStep;		// 连接服务器步骤
	char* Server;				// 服务器
	char* ServerPort;			// 服务器端口号
	char* WIFIName;				// WIFI name
	char* WIFIPassword;			// WIFI password
};



#endif // !_ESP8266_H
