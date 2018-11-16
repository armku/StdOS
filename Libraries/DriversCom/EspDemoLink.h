#ifndef _ESPDEMOLINK_H
#define _ESPDEMOLINK_H

#include "stm32f10x.h"
#include "FIFOBuffer.h"
#include "DataFrameEspDemoLink.h"
#include "USART.h"

class EspDemoLink
{
public:
	DataFrameEspDemoLink txFrame;
	DataFrameEspDemoLink rxFrame;
	USART &com;
	OutputPort pinch;
	OutputPort pinrst;
public:
	EspDemoLink(USART &uart);
	bool CheckFrame();
	bool Send();
	void cmd(char *cmd);
	void cmd(const char *cmd) { this->cmd((char*)cmd); }
	void Init();

	void SetPin(Pin pch,Pin prst);
private:

};


#endif // !_ESPDEMOLINK_H
