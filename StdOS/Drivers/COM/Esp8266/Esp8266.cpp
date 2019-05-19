#include <string.h>
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
