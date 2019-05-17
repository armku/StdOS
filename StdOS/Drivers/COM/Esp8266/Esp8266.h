#ifndef _ESP8266_H
#define _ESP8266_H

#include "Port.h"

class Esp8266
{
public:
	Esp8266();
	~Esp8266();

	void SetPinCH_PD(Pin p);
	void SetPinRST(Pin p);

	void Init();

private:
	Port CH_PD;//
	Port RST;//
};



#endif // !_ESP8266_H
