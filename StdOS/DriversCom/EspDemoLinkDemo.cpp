#include "EspDemoLink.h"
#include <stdio.h>
#include "Sys.h"
#include "BspPlatform/Interrupt.h"
#include "Port.h"
#include "Task.h"


#define _ESPDEMOLINKTEST_CPP
#ifdef _ESPDEMOLINKTEST_CPP

#define ApSsid                       "@PHICOMM_18"        //Ҫ���ӵ��ȵ������
#define ApPwd                        "18353217097"        //Ҫ���ӵ��ȵ����Կ
#define TcpServer_IP                 "www.emqtt.xyz"      //Ҫ���ӵķ������� IP
#define TcpServer_Port               "8888"               //Ҫ���ӵķ������Ķ˿�


USART usart333(USART3, 115200);
EspDemoLink esp(usart333);

void EspDemoLinkRoutin(void* param)
{
	static int step = 0;
	switch (step)
	{
	case 0:
		if (esp.Kick11())
			step++;
		break;
	case 1:
		if (esp.NetModeChoose(EspDemoLink::STA))
			step++;
		break;
	case 2:
		if (esp.JoinAP(ApSsid, ApPwd))
			step++;
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
