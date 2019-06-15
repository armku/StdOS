#include "../Esp8266.h"
#include "Sys.h"
#include "BspPlatform/Interrupt.h"
#include "Port.h"
#include "Task.h"
#include "Core/RingBuffer.h"
#include "Buffer.h"

#include "../Drivers/COM/MQTT/MqttLink.h"

#define _ESP8266EST_CPP
#ifdef _ESP8266EST_CPP

MqttLink mqtt;
const char *ClientID = "123456789";
const char* id = "1111";
const char* topic = "ShareDevince/RTV/IMEI";
const char* subtopic = "ShareDevince/CZT/IMEI";//订阅主题

/********************************** 用户需要设置的参数**********************************/
//要连接的热点的名称，即WIFI名称
//#define      macUser_ESP8266_ApSsid           "AndroidAP9DBE" 
#define      macUser_ESP8266_ApSsid           "@PHICOMM_18" 

//要连接的热点的密钥
#define      macUser_ESP8266_ApPwd            "ozbp8027" 

//要连接的服务器的 IP，即电脑的IP
#define      macUser_ESP8266_TcpServer_IP     "www.emqtt.xyz" 
//#define      macUser_ESP8266_TcpServer_IP     "test.armku.com" 

//要连接的服务器的端口
#define      macUser_ESP8266_TcpServer_Port    "1883"         


USARTHAL usart333(COM3,115200);
Esp8266 esp;

#include "../HAL/STM32F1/ARCH_UART.h"
uint8_t   loop_bufcom3[64] = { 0 };                             //定义环形缓冲区
RingBuffer ringRcvcom3(loop_bufcom3, ArrayLength(loop_bufcom3));
bool FlagInFrame;//接收到完整一帧数据
//向环形缓冲区【写】一字节数据
static void write_loop_buf(uint8_t dat)
{
	ringRcvcom3.Put(dat);
}

void checkComRoutin(void* param)
{
	static int RxCnt = 0;
	static int RxCntOld = 0;
	static int FlagIdleCnt = 0;//空闲时间

	/*if (!esp.FlagRcvAuto)
		return;*/

	RxCnt = ringRcvcom3.length;
	if (RxCnt != RxCntOld)
	{
		RxCntOld = RxCnt;
		return;
	}
	if (RxCnt == 0)
		return;

	if (++FlagIdleCnt > 10)
	{
		FlagInFrame = 1;

		
		if (esp.FlagConnected)
		{
			//ESP8266连接服务器未成功
			mqtt.readlen = ringRcvcom3.Get(mqtt.bufRcv, ArrayLength(mqtt.bufRcv));
			debug_printf("Rcv(%d) Hex:",mqtt.readlen);
			UART1_send_data((uint8_t*)mqtt.bufRcv, mqtt.readlen);//接收到的数据显示
			//Buffer(mqtt.bufRcv, mqtt.readlen).ShowHex();
		}
		else
		{
			//ESP8266连接服务器成功
			if (!mqtt.FlagConnected)
			{
				//正在登陆服务器
				Buffer(esp.bufRcv, esp.readlen).ShowHex();
			}
			else
			{
				//登陆服务器成功
				Buffer(esp.bufRcv, esp.readlen).ShowHex();
			}
			esp.readlen = ringRcvcom3.Get(esp.bufRcv, ArrayLength(esp.bufRcv));
			UART1_send_data((uint8_t*)esp.bufRcv, esp.readlen);//接收到的数据显示
		}
		
						
		FlagInFrame = 0;
		FlagIdleCnt = 0;
	}
}
char* publish = "abcd";
//要求1000ms调用一次
void Esp8266Routin(void* param)
{
	if (!esp.FlagConnected)
	{
		esp.Connect();
	}
	else
	{
		if (!mqtt.FlagConnected)
		{
			mqtt.Connect();
		}
		else
		{
			mqtt.Puslish(publish,4);
			delay(200);
			mqtt.Puslish_Release();
		}
		//esp.ESP8266_SendStr("Hello world!");
	}
}

void Esp8266TestInit()
{
	esp.SetPinCH_PD(PG13);
	esp.SetPinRST(PG14);
	esp.Init();
	UART_3_rcv_IRQ = write_loop_buf;
	esp.send_byte = UART_3_send_byte;         //串口发送一字节数据
	esp.send_buf = UART3_send_data;
	esp.Server = macUser_ESP8266_TcpServer_IP;
	esp.ServerPort = macUser_ESP8266_TcpServer_Port;
	esp.WIFIName = macUser_ESP8266_ApSsid;
	esp.WIFIPassword = macUser_ESP8266_ApPwd;

	mqtt.ClientID = (char*)ClientID;
	mqtt.FixHead = 0x10;
	mqtt.Topic = (char*)topic;
	//mqtt.MessageID = (char*)id;
	
	mqtt.send_buf = UART3_send_data;

	debug_printf("\r\n WF-ESP8266 WiFi模块测试例程\r\n"); //打印测试例程提示信息)

	FlagInFrame = false;
	Sys.AddTask(checkComRoutin, 0, 0, 10, "EspCOmCHK");
	Sys.AddTask(Esp8266Routin, 0, 0, 1000, "EspRoutin");
}
#endif
