#ifndef _ESPDEMOLINK_H
#define _ESPDEMOLINK_H

#include "stm32f10x.h"
#include "FIFOBuffer.h"
#include "DataFrameEspDemoLink.h"
#include "USART.h"

#define ESP8266_DEFAULT_TIMEOUT  1  //µ¥Î»£ºs

class EspDemoLink
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
public:
	DataFrameEspDemoLink txFrame;
	DataFrameEspDemoLink rxFrame;
	USART &com;
	OutputPort pinch;
	OutputPort pinrst;
public:
	EspDemoLink(USART &uart);
	bool Kick11();
	bool Receive11(char const *target="OK");
	bool NetModeChoose(ENUMNetModeTypeDef enumMode);
	bool JoinAP(char *pSSID, char *pPassWord);
	bool EnableMultipleId(bool enumEnUnvarnishTx);
	bool LinkServer(ENUMNetProTypeDef enumE, char *ip, char *ComNum, ENUMIDNOTypeDef id);
	bool UnvarnishSend();
	bool SendString(bool enumEnUnvarnishTx, char *pStr, int ulStrLength, ENUMIDNOTypeDef ucId);
	void EchoOn(bool on = false);





	bool CheckFrame();
	bool Send();
	void cmd(char *cmd);
	void cmd(const char *cmd) { this->cmd((char*)cmd); }
	void Init();

	void SetPin(Pin pch,Pin prst);

	
private:
	unsigned int mReceiveBufferIndex;
};


#endif // !_ESPDEMOLINK_H
