#include "BspPlatform\BspPlatform.h"
#include "Bsp.h"
#include "OnChip\Port.h"
#include "OnChip\Configuration.h"
#include "Buffer.h"
#include <stdio.h>  
#include <string.h>  

#ifdef STM32F0
OutputPort led1(PC6, true);
OutputPort led2(PC7, true);
OutputPort led3(PC8, true);
OutputPort led4(PC9, true);

OutputPort ledss[] =
{
	led1, led2, led3, led4
};
#elif defined STM32F1
#if 1
OutputPort led1(PB0, false);
OutputPort led2(PF7, false);
OutputPort led3(PF8, false);
#else 
OutputPort led1(PD0, false);
OutputPort led2(PD1, false);
OutputPort led3(PD2, false);
OutputPort ledLCD(PD12, false);
#endif 
#elif defined STM32F4
#if 1
//正点原子开发板F4
OutputPort led1(PF9, false);
OutputPort led2(PF10, false);
#else 
//安福莱开发板F4-V5
OutputPort led1(PI10, false);
OutputPort led2(PF7, false);
OutputPort led3(PC2, false);
OutputPort led4(PC2, false);
#endif 
#endif

void LedTask(void *param)
{
#ifdef STM32F0
	static uint8_t flag = 1;
	auto leds = (OutputPort*)param;

	flag <<= 1;
	flag &= 0x0f;
	if (flag == 0)
	{
		flag = 1;
	}

	led1 = !led1;
	led2 = !led2;
	led3 = !led3;
	led4 = !led4;
#elif defined STM32F1
	auto leds = (OutputPort*)param;
	*leds = !* leds;
	//    led2 = key0;
#elif defined STM32F4
	OutputPort *leds = (OutputPort*)param;
	*leds = !* leds;
	//led2 = !led2;
	//    led3 = !led3;
	//    led4 = !led4;
	//led2 = keyup;
#endif 
}
#include "OnChip/USART.h"
#include "BspPlatform/Interrupt.h"

USART usart111(USART1, 115200);


void mWifiinit();
void BspInit()
{
	led1 = 1;
	led2 = 1;
	Sys.AddTask(LedTask, &led1, 0, 500, "LedTask");

	mWifiinit();
}

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
			if(mWifi.JoinAP(mSetData.WIFISSID, mSetData.WIFIKEY, esp8266::esp8266_PATTERN_NULL) && (count++ < 4));//加入AP
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
