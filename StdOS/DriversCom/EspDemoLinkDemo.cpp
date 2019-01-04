#include "EspDemoLink.h"
#include <stdio.h>
#include <string.h>  
#include "Sys.h"
#include "BspPlatform/Interrupt.h"
#include "Port.h"
#include "Task.h"


#define _ESPDEMOLINKTEST_CPP
#ifdef _ESPDEMOLINKTEST_CPP

#define ApSsid                       "@PHICOMM_18"        //要连接的热点的名称
#define ApPwd                        "18353217097"        //要连接的热点的密钥
#define TcpServer_IP                 "www.emqtt.xyz"      //要连接的服务器的 IP
#define TcpServer_Port               "8888"               //要连接的服务器的端口


USART usart333(USART3, 115200);
EspDemoLink esp(usart333);

void EspDemoLinkRoutin(void* param)
{
	static int step = 0;
	static int icnt = 0;
	char cStr[100] =
	{
		0
	};
	switch (step)
	{
	case 0:
		if (esp.Kick11())
			step++;
		debug_printf("%02d esp.Kick11\n", step);
		break;
	case 1:
		esp.EchoOn();
		step++;
		debug_printf("%02d esp.EchoOn();\n", step);
		break;
	case 2:
		if (esp.Kick11())
			step++;
		//esp.Kick11();
		debug_printf("%02d esp.Kick11\n", step);
		break;
	case 3:
		if (esp.NetModeChoose(EspDemoLink::STA))
			step++;
		debug_printf("%02d esp.NetModeChoose(EspDemoLink::STA)\n", step);
		break;
	case 4:
		if (esp.JoinAP(ApSsid, ApPwd))
			step++;
		debug_printf("%02d esp.JoinAP(ApSsid, ApPwd)\n", step);
		break;
	case 5:
		if(esp.EnableMultipleId(DISABLE))
		step++;
		debug_printf("%02d esp.EnableMultipleId(DISABLE)\n", step);
		break;
	case 6:
		if(esp.LinkServer(EspDemoLink::enumTCP, TcpServer_IP, TcpServer_Port, EspDemoLink::SingleID0))
		step++;
		debug_printf("%02d esp.EnableMultipleId(DISABLE)\n", step);
		break;
	case 7:
		if (esp.UnvarnishSend())
			step++;
		debug_printf("%02d esp.UnvarnishSend", step);
		break;
	case 8:
		sprintf(cStr, "%d hello world!\n", ++icnt);
		esp.SendString(ENABLE, cStr, 0, EspDemoLink::SingleID0); //发送数据	
		debug_printf("%02d 发送数据: %s", step, cStr);
		break;
	default:
		break;
	}
}

void EspDemoLinkTestInit()
{
	esp.SetPin(PG13, PG14);
	esp.Init();

	Sys.AddTask(EspDemoLinkRoutin, 0, 2000, 1000, "EspLinkRoutin");
}
#endif
