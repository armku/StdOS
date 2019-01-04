#include "EspDemoLink.h"
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
#include <stdio.h>

EspDemoLink::EspDemoLink(USART &uart) :com(uart)
{
	com.ClearRxBuf();
	com.ClearTxBuf();
}
bool EspDemoLink::Kick11(void)
{
	com.ClearRxBuf();
	com.ClearTxBuf();
	com << "AT\r\n";

	Sys.Sleep(200);

	this->Receive11();
}
bool EspDemoLink::Receive11(char const *target)
{
	if (com.RxSize() > 0)
	{
		//debug_printf("rxlen:%d\n", com.RxSize());
	}

	if ((com.RxSize() > 0) && CheckFrame())
	{
		//debug_printf("rcv ok rxlen:%d-%d\n", com.RxSize(), rxFrame.dataLength);
		Buffer bs(rxFrame.data, rxFrame.dataLength);
		//bs.ShowHex();
		bs.Show();
		this->com.ClearRxBuf();
		this->rxFrame.dataLength = 0;
		if (strstr((char*)this->rxFrame.data, target))
			return true;
		else if (strstr((char*)this->rxFrame.data, "AT"))
			return true;
		else
			return false;
	}
	else
	{
		debug_printf("rcv error\n");
		return false;
	}
}
/*
 * 函数名：ESP8266_Net_Mode_Choose
 * 描述  ：选择WF-ESP8266模块的工作模式
 * 输入  ：enumMode，工作模式
 * 返回  : 1，选择成功
 *         0，选择失败
 * 调用  ：被外部调用
 */
bool EspDemoLink::NetModeChoose(ENUMNetModeTypeDef enumMode)
{
	com.ClearRxBuf();
	com.ClearTxBuf();
	
	switch (enumMode)
	{
	case STA:
		//return this->Cmd("AT+CWMODE=1", "OK", "no change", 2500);
		com << "AT+CWMODE=1\r\n";
		break;
	case AP:
		//return this->Cmd("AT+CWMODE=2", "OK", "no change", 2500);
		com << "AT+CWMODE=2\r\n";
		break;
	case STA_AP:
		//return this->Cmd("AT+CWMODE=3", "OK", "no change", 2500);
		com << "AT+CWMODE=3\r\n";
		break;
	default:
		return false;
	}

	Sys.Sleep(200);

	this->Receive11();
}
/*
 * 函数名：ESP8266_JoinAP
 * 描述  ：WF-ESP8266模块连接外部WiFi
 * 输入  ：pSSID，WiFi名称字符串
 *       ：pPassWord，WiFi密码字符串
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
 */
bool EspDemoLink::JoinAP(char *pSSID, char *pPassWord)
{
	char cCmd[120];
	sprintf(cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord);
	com << cCmd<<"\r\n";
	*pCOM1 << cCmd<<"\r\n";
	Sys.Sleep(200);

	return this->Receive11();
	
	//return this->Cmd(cCmd, "OK", NULL, 5000);
}
/*
 * 函数名：ESP8266_Enable_MultipleId
 * 描述  ：WF-ESP8266模块启动多连接
 * 输入  ：enumEnUnvarnishTx，配置是否多连接
 * 返回  : 1，配置成功
 *         0，配置失败
 * 调用  ：被外部调用
 */
bool EspDemoLink::EnableMultipleId(bool enumEnUnvarnishTx)
{
	char cStr[20];
	sprintf(cStr, "AT+CIPMUX=%d", (enumEnUnvarnishTx ? 1 : 0));
	com << cStr<<"\r\n";
	Sys.Sleep(200);

	this->Receive11();
	//return this->Cmd(cStr, "OK", 0, 500);
}
/*
 * 函数名：ESP8266_Link_Server
 * 描述  ：WF-ESP8266模块连接外部服务器
 * 输入  ：enumE，网络协议
 *       ：ip，服务器IP字符串
 *       ：ComNum，服务器端口字符串
 *       ：id，模块连接服务器的ID
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
 */
bool EspDemoLink::LinkServer(ENUMNetProTypeDef enumE, char *ip, char *ComNum, ENUMIDNOTypeDef id)
{
	char cStr[100] =
	{
		0
	}
	, cCmd[120];
	switch (enumE)
	{
	case enumTCP:
		sprintf(cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum);
		break;
	case enumUDP:
		sprintf(cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum);
		break;
	default:
		break;
	}
	if (id < 5)
		sprintf(cCmd, "AT+CIPSTART=%d,%s", id, cStr);
	else
		sprintf(cCmd, "AT+CIPSTART=%s", cStr);

	com << cCmd << "\r\n";
	Sys.Sleep(200);

	this->Receive11();
	//return this->Cmd(cCmd, "OK", "ALREAY CONNECT", 4000);
}
/*
 * 函数名：UnvarnishSend
 * 描述  ：配置WF-ESP8266模块进入透传发送
 * 输入  ：无
 * 返回  : 1，配置成功
 *         0，配置失败
 * 调用  ：被外部调用
 */
bool EspDemoLink::UnvarnishSend()
{
	com << "AT+CIPMODE=1"<<"\r\n";
	Sys.Sleep(200);

	if (!this->Receive11())
		return false;

	com << "AT+CIPSEND"<<"\r\n";
	Sys.Sleep(200);

	return this->Receive11();

	/*if (!this->Cmd("AT+CIPMODE=1", "OK", 0, 500))
		return false;
	return this->Cmd("AT+CIPSEND", "OK", ">", 500);*/
}
/*
 * 函数名：SendString
 * 描述  ：WF-ESP8266模块发送字符串
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 *       ：pStr，要发送的字符串
 *       ：ulStrLength，要发送的字符串的字节数
 *       ：ucId，哪个ID发送的字符串
 * 返回  : 1，发送成功
 *         0，发送失败
 * 调用  ：被外部调用
 */
bool EspDemoLink::SendString(bool enumEnUnvarnishTx, char *pStr, int ulStrLength, ENUMIDNOTypeDef ucId)
{
	char cStr[20];
	bool bRet = false;
	if (enumEnUnvarnishTx)
	{
		//this->USART_printf("%s", pStr);
		com << pStr;
		Sys.Sleep(200);

		return this->Receive11();
		bRet = true;
	}
	else
	{
		if (ucId < 5)
			sprintf(cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2);
		else
			sprintf(cStr, "AT+CIPSEND=%d", ulStrLength + 2);


		com << pStr<<"\r\n";
		Sys.Sleep(200);

		return this->Receive11();
		/*this->Cmd(cStr, "> ", 0, 1000);
		bRet = this->Cmd(pStr, "SEND OK", 0, 1000);*/
	}
	return bRet;
}
void EspDemoLink::EchoOn(bool on)
{
	this->com << "ATE" << (on ? 1 : 0) << "\r\n";
}






















bool EspDemoLink::CheckFrame()
{
	int rxlen = com.RxSize();

	if (com.GetBytes(&rxFrame.data[rxFrame.dataLength], rxlen))
	{
		rxFrame.dataLength += rxlen;
	}
	if (!rxFrame.CheckFrame())
		return false;
	else 
		return true;
}

void EspDemoLink::SetPin(Pin pch, Pin prst)
{
	this->pinch.Set(PG13);
	this->pinch.Invert = false;
	this->pinch.OpenDrain = false;
	this->pinch.Open();
	this->pinch = 0;

	this->pinrst.Set(PG14);
	this->pinrst.Invert = false;
	this->pinrst.OpenDrain = false;
	this->pinrst.Open();
	this->pinrst = 0;
}
void EspDemoLink::Init()
{
	Sys.Delay(500);
	this->pinch = 1;
	this->pinrst = 1;
}
