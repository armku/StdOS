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
		debug_printf("esp.Kick11\n");
		break;
	case 1:
		if (esp.NetModeChoose(EspDemoLink::STA))
			step++;
		debug_printf("esp.NetModeChoose(EspDemoLink::STA)\n");
		break;
	case 2:
		if (esp.JoinAP(ApSsid, ApPwd))
			step++;
		debug_printf("esp.JoinAP(ApSsid, ApPwd)\n");
		break;
	case 3:
		if(esp.EnableMultipleId(DISABLE))
		step++;
		debug_printf("esp.EnableMultipleId(DISABLE)\n");
		break;
	case 4:
		if(esp.LinkServer(EspDemoLink::enumTCP, TcpServer_IP, TcpServer_Port, EspDemoLink::SingleID0))
		step++;
		debug_printf("esp.EnableMultipleId(DISABLE)\n");
		break;
	case 5:
		if (esp.UnvarnishSend())
			step++;
		break;
	case 6:
		sprintf(cStr, "%d hello world!\n", ++icnt);
		esp.SendString(ENABLE, cStr, 0, EspDemoLink::SingleID0); //发送数据	
		debug_printf("发送数据: %s", cStr);
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
