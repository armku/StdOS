﻿#ifndef _Token_H_
#define _Token_H_

#include "Kernel\Sys.h"
#include "Net\ITransport.h"
#include "Net\Socket.h"
#include "Message\DataStore.h"

#include "TokenNet\TokenClient.h"
#include "TinyNet\TinyServer.h"

#include "TokenNet\Gateway.h"

#include "App\FlushPort.h"

// 令牌网
class Token
{
public:
	static void Setup(ushort code, cstring name, COM message = COM1, int baudRate = 0);

	static NetworkInterface* CreateW5500(SPI spi, Pin irq, Pin rst = P0, Pin power = P0, IDataPort* led = nullptr);
	static NetworkInterface* Create2860(SPI spi, Pin irq, Pin rst);

	static ITransport* Create2401(SPI spi, Pin ce, Pin irq, Pin power = P0, bool powerInvert = false, IDataPort* led = nullptr);
	static ITransport* CreateShunCom(COM index, int baudRate, Pin rst, Pin power, Pin slp, Pin cfg, IDataPort* led = nullptr);
	
	static TokenClient* CreateClient(NetworkInterface* host);
	static TokenClient* CreateClient2860(NetworkInterface* host);
	static TinyServer* CreateServer(ITransport* port);

	void SetPower(ITransport* port);
};

#endif
