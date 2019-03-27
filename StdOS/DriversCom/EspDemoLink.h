#ifndef _ESPDEMOLINK_H
#define _ESPDEMOLINK_H

#include "stm32f10x.h"
#include "FIFOBuffer.h"
#include "DataFrameEspDemoLink.h"
#include "USART.h"

#define ESP8266_DEFAULT_TIMEOUT  1  //µ¥Î»£ºs
#define ESP8266_RECEIVE_BUFFER_SIZE  50

class EspDemoLink
{
public:
	DataFrameEspDemoLink txFrame;
	DataFrameEspDemoLink rxFrame;
	USART &com;
	Port pinch;
	Port pinrst;
public:
	EspDemoLink(USART &uart);
	bool CheckFrame();
	bool Send();
	bool Kick(void);
	bool RecvFind(char const *target, unsigned char timeout = ESP8266_DEFAULT_TIMEOUT);
	bool ReceiveAndWait(char const* targetString, unsigned char timeOut = ESP8266_DEFAULT_TIMEOUT);
	void cmd(char *cmd);
	void cmd(const char *cmd) { this->cmd((char*)cmd); }
	void Init();

	void SetPin(Pin pch,Pin prst);

	void EchoOn(bool on = false);
private:
	unsigned int mReceiveBufferIndex;
	void ClearBuffer();
	char mReceiveBuffer[ESP8266_RECEIVE_BUFFER_SIZE];
};


#endif // !_ESPDEMOLINK_H
