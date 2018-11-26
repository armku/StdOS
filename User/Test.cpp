#include "DriversCom/EspDemoLink.h"
#include "Sys.h"
#include <string.h>  
#include "OnChip/USART.h"
#include "BspPlatform/Interrupt.h"
#include "BspPlatform\BspPlatform.h"
#include "OnChip\Port.h"
#include "OnChip\Configuration.h"
#include "Buffer.h"
#include <stdio.h>  
#include <string.h>  
#include "Sys.h"

USART usart333(USART3, 115200);
EspDemoLink espdemo(usart333);

int (*OnReceive)(char *buf, int len);
void EspDemoLinkRoutin(void* param)
{
	if (espdemo.com.FlagIdleOK)
	{
		espdemo.com.FlagIdleOK = false;
		if (espdemo.CheckFrame())
		{
			//debug_printf("rcv one frame:%d-%d\nRcv:",espdemo.rxFrame.frameLength,espdemo.rxFrame.dataLength);
			Buffer(espdemo.rxFrame.data, espdemo.rxFrame.frameLength).Show();
			//Buffer(espdemo.rxFrame.data, espdemo.rxFrame.frameLength).ShowHex(true);
			espdemo.rxFrame.RemoveOneFrame();
		}
		//debug_printf("com rx:%d-tx:%d frame:%d-%d\n", espdemo.com.RxCnt, espdemo.com.TxCnt, espdemo.rxFrame.Cnt, espdemo.txFrame.Cnt);
	}
}
void EspDemoLinkSendRoutin(void* param)
{
	static int i = 0;
	if (i++ == 3)
	{
		espdemo.EchoOn(false);
	}
	else
	{
		espdemo.cmd("AT");
	}
	i %= 20;

}

void EspDemoLinkTestInit()
{
	espdemo.SetPin(PG13, PG14);
	espdemo.Init();

	Sys.AddTask(EspDemoLinkRoutin, 0, 1000, 1, "EspLink");
	Sys.AddTask(EspDemoLinkSendRoutin, 0, 1000, 1000, "EspLinkSend");
}























#if 0
#include "DriversCom/esp8266/Socket_esp8266.h"
USART com3(USART3, 115200);
Socket_esp8266 mWifi(com3);

typedef struct SetData {
	char USEWIFIORGPRS;
	char IPADDR[20];
	char IPPORT[10];
	char WIFISSID[30];
	char WIFIKEY[30];
	char PEOPLENUM;
	char TRAINNUM;
	char TAGNUM;
	//u16
} SetDataStruct;

SetDataStruct mSetData;
u8 count = 0;

OutputPort pinch;
OutputPort pinrst;

void mWifiRoutin(void * param)
{
	static char buf[20];
	static int cnt = 0;
	static int step = 0;
	//int ret= mWifi.Kick();
	//debug_printf("检查连接：%d\n", ret);
	if (!mWifi.GetConnectStatus())
	{
		switch (step)
		{
		case 0:
			if (mWifi.Kick())//检查连接
			{
				step++;
			}
			break;
		case 1:
			mWifi.SetEcho(false);//关闭回响
			mWifi.SetMode(esp8266::esp8266_MODE_STATION, esp8266::esp8266_PATTERN_NULL);//设置station+ap模式
			mWifi.SetMUX(false);//单链接模式
			mWifi.QuitAP();
			if (mWifi.JoinAP(mSetData.WIFISSID, mSetData.WIFIKEY, esp8266::esp8266_PATTERN_NULL) && (count++ < 4));//加入AP
			{
				step++;
			}
			break;
		case 2:
			if (mWifi.Connect(mSetData.IPADDR, atoi(mSetData.IPPORT), Socket_Type_Stream, Socket_Protocol_IPV4) && (count++ < 4))
			{
				step++;
			}
			break;
		case 3:
			sprintf(buf, "%d hello world!\n", ++cnt);
			mWifi.Write(buf, strlen(buf));
			break;
		default:
			step = 0;
			break;
		}
		//	/*mMonitor.SetGPRSDataLogo(false);*/
			//while (!mWifi.Kick());//检查连接
		//	mWifi.SetEcho(false);//关闭回响
		//	mWifi.SetMode(esp8266::esp8266_MODE_STATION, esp8266::esp8266_PATTERN_NULL);//设置station+ap模式
		//	mWifi.SetMUX(false);//单链接模式
		//	mWifi.QuitAP();
		//	while (!mWifi.JoinAP(mSetData.WIFISSID, mSetData.WIFIKEY, esp8266::esp8266_PATTERN_NULL) && (count++ < 4));//加入AP
		//	count = 0;
		//	mWifi.Close();
		//	while (!mWifi.Connect(mSetData.IPADDR, atoi(mSetData.IPPORT), Socket_Type_Stream, Socket_Protocol_IPV4) && (count++ < 4));
		//	count = 0;
	}
	else
	{
		//	/*mMonitor.SetGPRSDataLogo(true);*/
	}
}
#include <stdio.h>  
#include <string.h>
char* ssid = "NETGEAR77";
char* key = "18353217097";
char * server = "www.braintics.net";
char *port = "8888";
void mWifiinit()
{
	pinch.Set(PG13);
	pinch.Invert = false;
	pinch.OpenDrain = false;
	pinch.Open();
	pinch = 0;

	pinrst.Set(PG14);
	pinrst.Invert = false;
	pinrst.OpenDrain = false;
	pinrst.Open();
	pinrst = 0;

	Sys.Delay(500);
	pinch = 1;
	pinrst = 1;

	strcpy(mSetData.WIFISSID, ssid);
	strcpy(mSetData.WIFIKEY, key);
	strcpy(mSetData.IPADDR, server);
	strcpy(mSetData.IPPORT, port);

	Sys.AddTask(mWifiRoutin, 0, 0, 1000, "mWifiRoutin");
}
#endif
