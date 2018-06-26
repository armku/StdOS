#include "Drivers\ESP8266.h"
#include "Device\DeviceConfigHelper.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "stm32f10x.h" 
#include "Handlers\IHandler.h"
#include "Handlers\IPipeline.h"

#define _ESP8266_TEST_CPP

#ifdef _ESP8266_TEST_CPP
//#define ApSsid                     "dd-wrt"               //Ҫ���ӵ��ȵ������
#define ApSsid                       "NETGEAR77"        //Ҫ���ӵ��ȵ������
#define ApPwd                        "18353217097"        //Ҫ���ӵ��ȵ����Կ
//#define TcpServer_IP                 "121.42.164.17"      //Ҫ���ӵķ������� IP
#define TcpServer_IP                 "47.104.108.194"      //Ҫ���ӵķ������� IP
#define TcpServer_Port               "1234"               //Ҫ���ӵķ������Ķ˿�

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
		//debug_printf("\r\n�������� ESP8266 ......\r\n");
		esp.Test();
		//esp.step++;
		break;
	case 1:
		esp.NetModeChoose(Esp8266::STA);
		esp.step++;
		break;
	case 2:
		/*while (!esp.JoinAP(ApSsid, ApPwd))
			;*/
		//esp.step++;
		break;
	case 3:
		

		
		
		esp.EnableMultipleId(DISABLE);
		while (!esp.LinkServer(Esp8266::enumTCP, TcpServer_IP, TcpServer_Port, Esp8266::SingleID0))
			;
		while (!esp.UnvarnishSend())
			;
		debug_printf("\r\n���� ESP8266 ���\r\n");
		esp.step++;
		break;
	case 4:
		sprintf(cStr, "%d hello world!\r\n", ++icnt);

		icnt %= 10;
		esp.SendString(ENABLE, cStr, 0x14, Esp8266::SingleID0); //��������	
		printf("��������: %s\r\n", cStr);
		Sys.Sleep(500);
		if (esp.FlagTcpClosed)
		{
			//����Ƿ�ʧȥ����
			esp.ExitUnvarnishSend(); //�˳�͸��ģʽ			
			do
				ucStatus = esp.GetLinkStatus();
			//��ȡ����״̬
			while (!ucStatus);
			if (ucStatus == 4)
				//ȷ��ʧȥ���Ӻ�����
			{
				debug_printf("\r\n���������ȵ�ͷ����� ......\r\n");
				while (!esp.JoinAP(ApSsid, ApPwd))
					;
				while (!esp.LinkServer(Esp8266::enumTCP, TcpServer_IP, TcpServer_Port, Esp8266::SingleID0))
					;
				debug_printf("\r\n�����ȵ�ͷ������ɹ�\r\n");
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

IPipeline pipeline;
HandlerShow handlershow;
HandlerShowHex handlershowhex;
IHandlerESP8266 handleresp8266;
uint8_t chbuf3[1000];

void com3rcv()
{
	Buffer bs1(chbuf3, ArrayLength(chbuf3));

	Rxx3.Read(bs1);

	pipeline.Read(bs1);
	esp.Rcv(bs1);
}
/**
* @brief  ESP8266 ��Sta Tcp Client��͸��
* @param  ��
* @retval ��
*/
void Esp8266TestInit()
{	
	esp.SetPin(PG14,PG13);
	esp.Init(); //��ʼ��WiFiģ��ʹ�õĽӿں�����
	DeviceConfigCenter::PRcvCOM3 = com3rcv;
	DeviceConfigCenter::ConfigCom(COM3, 115200);
	
	debug_printf("\r\nҰ�� WF-ESP8266 WiFiģ���������\r\n"); //��ӡ����������ʾ��Ϣ
	Sys.AddTask(espRoutin,0,0,1000,"espRoutin");	
		
	pipeline.AddLast(&handlershowhex);
	pipeline.AddLast(&handlershow);
	pipeline.AddLast(&handleresp8266);
}



#endif
