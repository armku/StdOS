#ifndef _ESP8266_H
#include "Sys.h"
#include "Port.h"

class ESP8266
{
public:
	ESP8266();
	void Init();
	void SetPin(Pin prst, Pin pch);
	void Rst();

private:
	OutputPort pRst;
	OutputPort pCH;
};



#endif // !_ESP8266_H
