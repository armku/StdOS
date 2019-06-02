#include <string.h>
#include <stdio.h>
#include "Esp8266.h"
#include "Sys.h"

//要连接的热点的名称，即WIFI名称
#define      macUser_ESP8266_ApSsid           "@PHICOMM_18" 

//要连接的热点的密钥
#define      macUser_ESP8266_ApPwd            "wildfire" 

//要连接的服务器的 IP，即电脑的IP
#define      macUser_ESP8266_TcpServer_IP     "www.armku.com" 

//要连接的服务器的端口
#define      macUser_ESP8266_TcpServer_Port    "8081"         


Esp8266::Esp8266()
{
	this->step = 0;
	this->FlagRcvAuto = 0;
	this->FlagConnected = 0;
	this->FlagConnectStep = 0;
}

Esp8266::~Esp8266()
{
}
//连接服务器
void Esp8266::Connect()
{
	switch (this->FlagConnectStep)
	{
	case 0:
		this->CH_PD = 1;
		this->RST = 1;
		this->FlagConnectStep++;
		break;
	case 1:
		if (this->AT())
			this->FlagConnectStep++;
		//debug_printf("AT send step:%d\r\n", this->FlagConnectStep);
		break;
	case 2:
		if (this->ESP8266_Net_Mode_Choose(STA))
			this->FlagConnectStep++;
		break;
	case 3:
		if (this->ESP8266_JoinAP(macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd))
			this->FlagConnectStep++;
		break;
	case 4:
		if (this->ESP8266_Enable_MultipleId(DISABLE))
			this->FlagConnectStep++;
		break;
	case 5:
		if (this->ESP8266_Link_Server(enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0))
			this->FlagConnectStep++;
		break;
	case 6:
		if (this->ESP8266_UnvarnishSend())
			this->FlagConnectStep++;
		break;
	case 7:
		if (this->ESP8266_SendStr("Hello workd!"))
			;
		break;
	default:
		break;
	}
}
void Esp8266::Cmd(char* cmd, int len)
{

}
bool Esp8266::ESP8266_Cmd(char* cmd, char* reply1, char* reply2, uint32_t waittime)
{
	this->Reply1 = reply1;
	this->Reply2 = reply2;
	this->Cmd(cmd);
	this->WaitTime = 0;
	if ((reply1 == 0) && (reply2 == 0))                      //不需要接收数据
		return true;
	this->WaitTime = waittime;
	delayMicroseconds(waittime);

	
	//UART1_send_data((uint8_t*)this->bufRcv, this->readlen);
	this->DealBufIn(this->bufRcv, this->readlen);

}
void Esp8266::Cmd(char* cmd)
{
	int len = strlen(cmd);
	if (len <= 0)
		return;
		
	char buf[120];
	for (int i = 0; i < len; i++)
		buf[i] = cmd[i];
	buf[len] = 0X0D;
	buf[len + 1] = 0X0A;
	this->send_buf((uint8_t*)buf, len+2);
}
//处理接收到的数据
bool Esp8266::DealBufIn(char* buf, int len)
{
	//debug_printf("Rcv:%s step:%d reply1: %d reply2:%d\r\n",buf,this->step, this->IsReply1(buf), this->IsReply2(buf));
	if (this->IsReply1(buf))
		return true;
	if (this->IsReply2(buf))
		return true;

	return false;
}
bool Esp8266::AT()
{
	return this->ESP8266_Cmd("AT","AT",NULL,500);
}
bool Esp8266::ESP8266_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode)
{
	switch (enumMode)
	{
	case STA:
		return ESP8266_Cmd("AT+CWMODE=1", "OK", "no change", 500);
	case AP:
		return ESP8266_Cmd("AT+CWMODE=2", "OK", "no change", 500);
	case STA_AP:
		return ESP8266_Cmd("AT+CWMODE=3", "OK", "no change", 500);
	default:
		return false;
	}
}
bool Esp8266::ESP8266_JoinAP(char* pSSID, char* pPassWord)
{
	char cCmd[120];

	sprintf(cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord);
	debug_printf("JoinAP send:%s\r\n",cCmd);

	return ESP8266_Cmd(cCmd, "OK", NULL, 5000);
}
bool Esp8266::ESP8266_Enable_MultipleId(FunctionalState enumEnUnvarnishTx)
{
	char cStr[20];

	sprintf(cStr, "AT+CIPMUX=%d", (enumEnUnvarnishTx ? 1 : 0));

	return ESP8266_Cmd(cStr, "OK", NULL, 500);
}
bool Esp8266::ESP8266_Link_Server(ENUM_NetPro_TypeDef enumE, char* ip, char* ComNum, ENUM_ID_NO_TypeDef id)
{
	char cStr[100] = { 0 }, cCmd[120];

	switch (enumE)
	{
	case enumTCP:
		sprintf(cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum);
		break;

	case enumUDP:
		sprintf(cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum);
		break;

	default:
		break;
	}

	if (id < 5)
		sprintf(cCmd, "AT+CIPSTART=%d,%s", id, cStr);

	else
		sprintf(cCmd, "AT+CIPSTART=%s", cStr);

	return ESP8266_Cmd(cCmd, "OK", "ALREAY CONNECT", 4000);
}
bool Esp8266::ESP8266_UnvarnishSend(void)
{
	if (!ESP8266_Cmd("AT+CIPMODE=1", "OK", 0, 500))
		return false;

	return
		ESP8266_Cmd("AT+CIPSEND", "OK", ">", 500);
}
//发送字符串
bool Esp8266::ESP8266_SendStr(char* str)
{
	this->ESP8266_SendString(DISABLE, str, 0, Single_ID_0);
	this->Cmd(str);
	return true;
}
void  Esp8266::ESP8266_ExitUnvarnishSend(void)
{
	/*Delay_ms(1000);

	macESP8266_Usart("+++");

	Delay_ms(500);*/
}


bool Esp8266::IsReply1(char* buf)
{
	return this->IsReply(buf,this->Reply1);
}
bool Esp8266::IsReply2(char* buf)
{
	return this->IsReply(buf,this->Reply2);
}
bool Esp8266::IsReply(char* buf,char* reply)
{
	if (reply == NULL)
		return false;
	if (strstr(buf, reply))
		return true;
	else
		return false;
}
bool Esp8266::ESP8266_SendString(FunctionalState enumEnUnvarnishTx, char* pStr, uint32_t ulStrLength, ENUM_ID_NO_TypeDef ucId)
{
	char cStr[120];
	bool bRet = false;


	if (enumEnUnvarnishTx)
	{
		this->Cmd(pStr);

		bRet = true;

	}

	else
	{
		if (ucId < 5)
			sprintf(cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2);

		else
			sprintf(cStr, "AT+CIPSEND=%d", ulStrLength + 2);

		ESP8266_Cmd(cStr, "> ", 0, 1000);

		bRet = this->ESP8266_Cmd(pStr, "SEND OK", 0, 1000);
	}

	return bRet;
}
void Esp8266::SetPinCH_PD(Pin p)
{
	this->CH_PD.SetPin(p);
	this->CH_PD.pinMode(GPIO_Out_PP);
}
void Esp8266::SetPinRST(Pin p)
{
	this->RST.SetPin(p);
	this->RST.pinMode(GPIO_Out_PP);
}

void Esp8266::Init()
{
	this->RST = 1;
	this->CH_PD = 0;
}
