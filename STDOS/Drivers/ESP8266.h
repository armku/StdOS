#ifndef _ESP8266_H
#include "Sys.h"
#include "Port.h"
#include "Handlers\IHandler.h"

#define RX_BUF_MAX_LEN     1024 

typedef struct
{
	char RxBuf[RX_BUF_MAX_LEN];
	int Length;
	int FlagFinish;
}Fram_T;

class Esp8266:public IHandler
{
public:
	typedef enum
	{
		STA, AP, STA_AP
			} ENUMNetModeTypeDef;
	typedef enum
	{
		enumTCP, enumUDP,
	} ENUMNetProTypeDef;


	typedef enum
	{
		MultipleID0 = 0, MultipleID1 = 1, MultipleID2 = 2, MultipleID3 = 3, MultipleID4 = 4, SingleID0 = 5,
	} ENUMIDNOTypeDef;

	typedef enum
	{
		OPEN = 0, WEP = 1, WPA_PSK = 2, WPA2_PSK = 3, WPA_WPA2_PSK = 4,
	} ENUMAPPsdModeTypeDef;
	/// <summary>
	/// ��ȡ���ݴ���
	/// </summary>
	/// <param name="buf"></param>
	/// <returns></returns>
	virtual Buffer & Read(Buffer &bs);
	void Init();
	void Rst();
	bool Cmd(char *cmd, char *reply1, char *reply2, int waittime);
	void Test();
	bool NetModeChoose(ENUMNetModeTypeDef enumMode);
	bool JoinAP(char *pSSID, char *pPassWord);
	bool BuildAP(char *pSSID, char *pPassWord, ENUMAPPsdModeTypeDef enunPsdMode);
	bool EnableMultipleId(bool enumEnUnvarnishTx);
	bool LinkServer(ENUMNetProTypeDef enumE, char *ip, char *ComNum, ENUMIDNOTypeDef id);
	bool StartOrShutServer(bool enumMode, char *pPortNum, char *pTimeOver);
	int GetLinkStatus();
	int GetIdLinkStatus();
	int InquireApIp(char *pApIp, int ucArrayLength);
	bool UnvarnishSend();
	void ExitUnvarnishSend();
	bool SendString(bool enumEnUnvarnishTx, char *pStr, int ulStrLength, ENUMIDNOTypeDef ucId);
	char *ReceiveString(bool enumEnUnvarnishTx);
	void SetPin(Pin prst, Pin pch);
	int step;//����

	Buffer* bsRcv1;//�ڴ�����Ӧ����1
	Buffer* bsRcv2;//�ڴ�����Ӧ����2
	Buffer* bsAT;//ATָ��
private:	
	char *itoa(int value, char *string, int radix);
	void USART_printf(char *Data, ...);
	char bufRcv1[100];//�ڴ�����Ӧ1
	char bufRcv2[100];//�ڴ�����Ӧ2
	int flagRcvCnt;//�ڴ�����Ӧ���ݳ���
private:
	OutputPort pRst;
	OutputPort pCH;
public:
	volatile bool FlagTcpClosed;//�Ƿ�Ͽ�����
	};

extern Fram_T strEsp8266_Fram_Record;
#endif
