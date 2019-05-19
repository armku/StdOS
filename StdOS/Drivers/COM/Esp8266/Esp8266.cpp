#include <string.h>
#include <stdio.h>
#include "Esp8266.h"
#include "Sys.h"

Esp8266::Esp8266()
{
	this->step = 0;
}

Esp8266::~Esp8266()
{
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
}
void Esp8266::Cmd(char* cmd)
{
	int len = strlen(cmd);
	if (len <= 0)
		return;
		
	char buf[20];
	for (int i = 0; i < len; i++)
		buf[i] = cmd[i];
	buf[len] = 0X0D;
	buf[len + 1] = 0X0A;
	this->send_buf((uint8_t*)buf, len+2);
}
//处理接收到的数据
bool Esp8266::DealBufIn(char* buf, int len)
{
	debug_printf("Rcv:%s",buf);
	debug_printf("reply1:%d \r\n",this->IsReply1(buf));
	debug_printf("reply2:%d \r\n", this->IsReply2(buf));
	if (this->IsReply1(buf))
		this->step++;
	if (this->IsReply2(buf))
		this->step++;

	return false;
}
void Esp8266::AT()
{
	this->ESP8266_Cmd("AT","AT",NULL,500);
}
bool Esp8266::ESP8266_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode)
{
	switch (enumMode)
	{
	case STA:
		return ESP8266_Cmd("AT+CWMODE=1", "OK", "no change", 2500);

	case AP:
		return ESP8266_Cmd("AT+CWMODE=2", "OK", "no change", 2500);

	case STA_AP:
		return ESP8266_Cmd("AT+CWMODE=3", "OK", "no change", 2500);

	default:
		return false;
	}
}
bool Esp8266::ESP8266_JoinAP(char* pSSID, char* pPassWord)
{
	char cCmd[120];

	sprintf(cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord);

	return ESP8266_Cmd(cCmd, "OK", NULL, 5000);
}
bool Esp8266::ESP8266_Enable_MultipleId(FunctionalState enumEnUnvarnishTx)
{
	char cStr[20];

	sprintf(cStr, "AT+CIPMUX=%d", (enumEnUnvarnishTx ? 1 : 0));

	return ESP8266_Cmd(cStr, "OK", 0, 500);
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

bool Esp8266::IsReply1(char* buf)
{
	if (this->Reply1 == NULL)
		return false;
	if(strstr(buf,this->Reply1))
		return true;
	else
		return false;
}
bool Esp8266::IsReply2(char* buf)
{
	if (this->Reply2 == NULL)
		return false;
		if (strstr(buf, this->Reply2))
			return true;
		else
			return false;
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
