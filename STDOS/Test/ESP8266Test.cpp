#include "Drivers\ESP8266.h"
#include "Device\DeviceConfigHelper.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "stm32f10x.h" 

#define _ESP8266_TEST_CPP

#ifdef _ESP8266_TEST_CPP
//#define ApSsid                     "dd-wrt"               //要连接的热点的名称
#define ApSsid                       "NETGEAR77"        //要连接的热点的名称
#define ApPwd                        "18353217097"        //要连接的热点的密钥
//#define TcpServer_IP                 "121.42.164.17"      //要连接的服务器的 IP
#define TcpServer_IP                 "47.104.108.194"      //要连接的服务器的 IP
#define TcpServer_Port               "1234"               //要连接的服务器的端口

Esp8266 esp;
uint8_t ucStatus;
char cStr[100] =
{
	0
};
static int icnt = 0;
void espRoutin(void*param)
{
	switch (esp.step)
	{
	case 0:
		esp.step++;
		break;
	case 1:
		esp.step++;
		break;
	case 2:
		esp.step++;
		break;
	case 3:
		debug_printf("\r\n正在配置 ESP8266 ......\r\n");
		esp.Test();

		esp.NetModeChoose(Esp8266::STA);
		while (!esp.JoinAP(ApSsid, ApPwd))
			;
		esp.EnableMultipleId(DISABLE);
		while (!esp.LinkServer(Esp8266::enumTCP, TcpServer_IP, TcpServer_Port, Esp8266::SingleID0))
			;
		while (!esp.UnvarnishSend())
			;
		debug_printf("\r\n配置 ESP8266 完毕\r\n");
		esp.step++;
		break;
	case 4:
		sprintf(cStr, "%d hello world!\r\n", ++icnt);

		icnt %= 10;
		esp.SendString(ENABLE, cStr, 0x14, Esp8266::SingleID0); //发送数据	
		printf("发送数据: %s\r\n", cStr);
		Sys.Sleep(500);
		if (esp.FlagTcpClosed)
		{
			//检测是否失去连接
			esp.ExitUnvarnishSend(); //退出透传模式			
			do
				ucStatus = esp.GetLinkStatus();
			//获取连接状态
			while (!ucStatus);
			if (ucStatus == 4)
				//确认失去连接后重连
			{
				debug_printf("\r\n正在重连热点和服务器 ......\r\n");
				while (!esp.JoinAP(ApSsid, ApPwd))
					;
				while (!esp.LinkServer(Esp8266::enumTCP, TcpServer_IP, TcpServer_Port, Esp8266::SingleID0))
					;
				debug_printf("\r\n重连热点和服务器成功\r\n");
			}
			while (!esp.UnvarnishSend())
				;
		}
		break;
	default:
		esp.step = 0;
		break;
	}
}

/**
* @brief  ESP8266 （Sta Tcp Client）透传
* @param  无
* @retval 无
*/
void Esp8266TestInit()
{	
	esp.SetPin(PG14,PG13);
	esp.Init(); //初始化WiFi模块使用的接口和外设
	
	debug_printf("\r\n野火 WF-ESP8266 WiFi模块测试例程\r\n"); //打印测试例程提示信息
	Sys.AddTask(espRoutin,0,0,1000,"espRoutin");	
}

#endif
