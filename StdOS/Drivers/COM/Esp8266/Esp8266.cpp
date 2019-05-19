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

}
void Esp8266::AT()
{
	char buf[5];
	buf[0] = 'A';// bufin[0];
	buf[1] = 'T';// bufin[1];
	buf[2] = 0x0d;
	buf[3] = 0x0a;
	buf[4] = 0;

	this->send_buf((uint8_t*)buf, 4);
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
