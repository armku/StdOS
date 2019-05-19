#include <string.h>
#include "Esp8266.h"

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
void Esp8266::AT()
{
	this->ESP8266_Cmd("AT","AT",NULL,500);
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
