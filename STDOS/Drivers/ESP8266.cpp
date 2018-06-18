#include "Esp8266.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "stm32f10x.h" 
#include "Sys.h"
#include "Device\DeviceConfigHelper.h"

Fram_T strEsp8266_Fram_Record =
{
	0
};
uint8_t chbuf3[1000];
extern Esp8266 esp;
void com3rcv()
{
	Buffer bs1(chbuf3, ArrayLength(chbuf3));

	Rxx3.Read(bs1);


	debug_printf("COM1RCV:");
	bs1.Show(true);

	for (int i = 0; i < bs1.Length(); i++)
	{
		strEsp8266_Fram_Record.RxBuf[i] = bs1[i];
	}
	strEsp8266_Fram_Record.Length = bs1.Length();
	strEsp8266_Fram_Record.FlagFinish = 1;
	esp.FlagTcpClosed = strstr(strEsp8266_Fram_Record.RxBuf, "CLOSED\r\n") ? 1 : 0;
}
/**
* @brief  ESP8266��ʼ������
* @param  ��
* @retval ��
*/
void Esp8266::Init()
{	
	this->pRst = 1;
	this->pCH = 0;
	DeviceConfigCenter::PRcvCOM3 = com3rcv;
	DeviceConfigCenter::ConfigCom(COM3, 115200);

	this->FlagTcpClosed = 0;//�Ƿ�Ͽ�����

	this->step = 0;
}

/*
* ��������ESP8266_Rst
* ����  ������WF-ESP8266ģ��
* ����  ����
* ����  : ��
* ����  ���� ESP8266_AT_Test ����
*/
void Esp8266::Rst()
{
#if 0
	this->Cmd("AT+RST", "OK", "ready", 2500);

#else 
	this->pRst = 0;
	Sys.Sleep(500);
	this->pRst = 1;
#endif 

}

/*
* ��������this->Cmd
* ����  ����WF-ESP8266ģ�鷢��ATָ��
* ����  ��cmd�������͵�ָ��
*         reply1��reply2���ڴ�����Ӧ��ΪNULL��������Ӧ������Ϊ���߼���ϵ
*         waittime���ȴ���Ӧ��ʱ��
* ����  : 1��ָ��ͳɹ�
*         0��ָ���ʧ��
* ����  �����ⲿ����
*/
bool Esp8266::Cmd(char *cmd, char *reply1, char *reply2, int waittime)
{
	strEsp8266_Fram_Record.Length = 0; //���¿�ʼ�����µ����ݰ�

	this->USART_printf("%s\r\n", cmd);

	if ((reply1 == 0) && (reply2 == 0))
		//����Ҫ��������
		return true;

	Sys.Sleep(waittime); //��ʱ

	strEsp8266_Fram_Record.RxBuf[strEsp8266_Fram_Record.Length] = '\0';

	debug_printf("%s", strEsp8266_Fram_Record.RxBuf);

	if ((reply1 != 0) && (reply2 != 0))
		return ((bool)strstr(strEsp8266_Fram_Record.RxBuf, reply1) || (bool)strstr(strEsp8266_Fram_Record.RxBuf, reply2));

	else if (reply1 != 0)
		return ((bool)strstr(strEsp8266_Fram_Record.RxBuf, reply1));

	else
		return ((bool)strstr(strEsp8266_Fram_Record.RxBuf, reply2));
}

void Esp8266::Test()
{
	char count = 0;

	this->pRst = 1;
	Sys.Sleep(999);
	while (count < 10)
	{
		if (this->Cmd("AT", "OK", NULL, 500))
			return;
		this->Rst();
		++count;
	}
	this->pCH = 1;
}

/*
* ��������ESP8266_Net_Mode_Choose
* ����  ��ѡ��WF-ESP8266ģ��Ĺ���ģʽ
* ����  ��enumMode������ģʽ
* ����  : 1��ѡ��ɹ�
*         0��ѡ��ʧ��
* ����  �����ⲿ����
*/
bool Esp8266::NetModeChoose(ENUMNetModeTypeDef enumMode)
{
	switch (enumMode)
	{
	case STA:
		return this->Cmd("AT+CWMODE=1", "OK", "no change", 2500);

	case AP:
		return this->Cmd("AT+CWMODE=2", "OK", "no change", 2500);

	case STA_AP:
		return this->Cmd("AT+CWMODE=3", "OK", "no change", 2500);

	default:
		return false;
	}
}

/*
* ��������ESP8266_JoinAP
* ����  ��WF-ESP8266ģ�������ⲿWiFi
* ����  ��pSSID��WiFi�����ַ���
*       ��pPassWord��WiFi�����ַ���
* ����  : 1�����ӳɹ�
*         0������ʧ��
* ����  �����ⲿ����
*/
bool Esp8266::JoinAP(char *pSSID, char *pPassWord)
{
	char cCmd[120];
	sprintf(cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord);
	return this->Cmd(cCmd, "OK", NULL, 5000);
}

/*
* ��������ESP8266_BuildAP
* ����  ��WF-ESP8266ģ�鴴��WiFi�ȵ�
* ����  ��pSSID��WiFi�����ַ���
*       ��pPassWord��WiFi�����ַ���
*       ��enunPsdMode��WiFi���ܷ�ʽ�����ַ���
* ����  : 1�������ɹ�
*         0������ʧ��
* ����  �����ⲿ����
*/
bool Esp8266::BuildAP(char *pSSID, char *pPassWord, ENUMAPPsdModeTypeDef enunPsdMode)
{
	char cCmd[120];
	sprintf(cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode);
	return this->Cmd(cCmd, "OK", 0, 1000);
}

/*
* ��������ESP8266_Enable_MultipleId
* ����  ��WF-ESP8266ģ������������
* ����  ��enumEnUnvarnishTx�������Ƿ������
* ����  : 1�����óɹ�
*         0������ʧ��
* ����  �����ⲿ����
*/
bool Esp8266::EnableMultipleId(bool enumEnUnvarnishTx)
{
	char cStr[20];
	sprintf(cStr, "AT+CIPMUX=%d", (enumEnUnvarnishTx ? 1 : 0));
	return this->Cmd(cStr, "OK", 0, 500);
}

/*
* ��������ESP8266_Link_Server
* ����  ��WF-ESP8266ģ�������ⲿ������
* ����  ��enumE������Э��
*       ��ip��������IP�ַ���
*       ��ComNum���������˿��ַ���
*       ��id��ģ�����ӷ�������ID
* ����  : 1�����ӳɹ�
*         0������ʧ��
* ����  �����ⲿ����
*/
bool Esp8266::LinkServer(ENUMNetProTypeDef enumE, char *ip, char *ComNum, ENUMIDNOTypeDef id)
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
	return this->Cmd(cCmd, "OK", "ALREAY CONNECT", 4000);
}

/*
* ��������StartOrShutServer
* ����  ��WF-ESP8266ģ�鿪����رշ�����ģʽ
* ����  ��enumMode������/�ر�
*       ��pPortNum���������˿ں��ַ���
*       ��pTimeOver����������ʱʱ���ַ�������λ����
* ����  : 1�������ɹ�
*         0������ʧ��
* ����  �����ⲿ����
*/
bool Esp8266::StartOrShutServer(bool enumMode, char *pPortNum, char *pTimeOver)
{
	char cCmd1[120], cCmd2[120];

	if (enumMode)
	{
		sprintf(cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum);

		sprintf(cCmd2, "AT+CIPSTO=%s", pTimeOver);

		return (this->Cmd(cCmd1, "OK", 0, 500) && this->Cmd(cCmd2, "OK", 0, 500));
	}
	else
	{
		sprintf(cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum);

		return this->Cmd(cCmd1, "OK", 0, 500);
	}
}

/*
* ��������GetLinkStatus
* ����  ����ȡ WF-ESP8266 ������״̬�����ʺϵ��˿�ʱʹ��
* ����  ����
* ����  : 2�����ip
*         3����������
*         3��ʧȥ����
*         0����ȡ״̬ʧ��
* ����  �����ⲿ����
*/
int Esp8266::GetLinkStatus()
{
	if (this->Cmd("AT+CIPSTATUS", "OK", 0, 500))
	{
		if (strstr(strEsp8266_Fram_Record.RxBuf, "STATUS:2\r\n"))
			return 2;

		else if (strstr(strEsp8266_Fram_Record.RxBuf, "STATUS:3\r\n"))
			return 3;

		else if (strstr(strEsp8266_Fram_Record.RxBuf, "STATUS:4\r\n"))
			return 4;
	}
	return 0;
}

/*
* ��������ESP8266_Get_IdLinkStatus
* ����  ����ȡ WF-ESP8266 �Ķ˿ڣ�Id������״̬�����ʺ϶�˿�ʱʹ��
* ����  ����
* ����  : �˿ڣ�Id��������״̬����5λΪ��Чλ���ֱ��ӦId5~0��ĳλ����1����Id���������ӣ�������0����Idδ��������
* ����  �����ⲿ����
*/
int Esp8266::GetIdLinkStatus()
{
	uint8_t ucIdLinkStatus = 0x00;


	if (this->Cmd("AT+CIPSTATUS", "OK", 0, 500))
	{
		if (strstr(strEsp8266_Fram_Record.RxBuf, "+CIPSTATUS:0,"))
			ucIdLinkStatus |= 0x01;
		else
			ucIdLinkStatus &= ~0x01;

		if (strstr(strEsp8266_Fram_Record.RxBuf, "+CIPSTATUS:1,"))
			ucIdLinkStatus |= 0x02;
		else
			ucIdLinkStatus &= ~0x02;

		if (strstr(strEsp8266_Fram_Record.RxBuf, "+CIPSTATUS:2,"))
			ucIdLinkStatus |= 0x04;
		else
			ucIdLinkStatus &= ~0x04;

		if (strstr(strEsp8266_Fram_Record.RxBuf, "+CIPSTATUS:3,"))
			ucIdLinkStatus |= 0x08;
		else
			ucIdLinkStatus &= ~0x08;

		if (strstr(strEsp8266_Fram_Record.RxBuf, "+CIPSTATUS:4,"))
			ucIdLinkStatus |= 0x10;
		else
			ucIdLinkStatus &= ~0x10;
	}
	return ucIdLinkStatus;
}

/*
* ��������InquireApIp
* ����  ����ȡ F-ESP8266 �� AP IP
* ����  ��pApIp����� AP IP ��������׵�ַ
*         ucArrayLength����� AP IP ������ĳ���
* ����  : 0����ȡʧ��
*         1����ȡ�ɹ�
* ����  �����ⲿ����
*/
int Esp8266::InquireApIp(char *pApIp, int ucArrayLength)
{
	char uc;
	char *pCh;
	this->Cmd("AT+CIFSR", "OK", 0, 500);
	pCh = strstr(strEsp8266_Fram_Record.RxBuf, "APIP,\"");
	if (pCh)
		pCh += 6;
	else
		return 0;
	for (uc = 0; uc < ucArrayLength; uc++)
	{
		pApIp[uc] = *(pCh + uc);

		if (pApIp[uc] == '\"')
		{
			pApIp[uc] = '\0';
			break;
		}
	}
	return 1;
}

/*
* ��������UnvarnishSend
* ����  ������WF-ESP8266ģ�����͸������
* ����  ����
* ����  : 1�����óɹ�
*         0������ʧ��
* ����  �����ⲿ����
*/
bool Esp8266::UnvarnishSend()
{
	if (!this->Cmd("AT+CIPMODE=1", "OK", 0, 500))
		return false;
	return this->Cmd("AT+CIPSEND", "OK", ">", 500);
}

/*
* ��������ExitUnvarnishSend
* ����  ������WF-ESP8266ģ���˳�͸��ģʽ
* ����  ����
* ����  : ��
* ����  �����ⲿ����
*/
void Esp8266::ExitUnvarnishSend()
{
	Sys.Sleep(999);
	this->USART_printf("+++");
	Sys.Sleep(500);
}

/*
* ��������SendString
* ����  ��WF-ESP8266ģ�鷢���ַ���
* ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
*       ��pStr��Ҫ���͵��ַ���
*       ��ulStrLength��Ҫ���͵��ַ������ֽ���
*       ��ucId���ĸ�ID���͵��ַ���
* ����  : 1�����ͳɹ�
*         0������ʧ��
* ����  �����ⲿ����
*/
bool Esp8266::SendString(bool enumEnUnvarnishTx, char *pStr, int ulStrLength, ENUMIDNOTypeDef ucId)
{
	char cStr[20];
	bool bRet = false;
	int len = strlen(pStr);
	//	USART_SendData(USART3, 0x01);
	//	USART_SendData(USART3, 0x01);
	//	USART_SendData(USART3, len-2);
	//	USART_SendData(USART3, 0x00);
	if (enumEnUnvarnishTx)
	{
		this->USART_printf("%s", pStr);
		bRet = true;
	}
	else
	{
		if (ucId < 5)
			sprintf(cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2);
		else
			sprintf(cStr, "AT+CIPSEND=%d", ulStrLength + 2);
		this->Cmd(cStr, "> ", 0, 1000);
		bRet = this->Cmd(pStr, "SEND OK", 0, 1000);
	}
	return bRet;
}


/*
* ��������ESP8266_ReceiveString
* ����  ��WF-ESP8266ģ������ַ���
* ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
* ����  : ���յ����ַ����׵�ַ
* ����  �����ⲿ����
*/
char *Esp8266::ReceiveString(bool enumEnUnvarnishTx)
{
	char *pRecStr = 0;
	strEsp8266_Fram_Record.Length = 0;
	strEsp8266_Fram_Record.FlagFinish = 0;
	while (!strEsp8266_Fram_Record.FlagFinish)
		;
	strEsp8266_Fram_Record.RxBuf[strEsp8266_Fram_Record.Length] = '\0';
	if (enumEnUnvarnishTx)
		pRecStr = strEsp8266_Fram_Record.RxBuf;
	else
	{
		if (strstr(strEsp8266_Fram_Record.RxBuf, "+IPD"))
			pRecStr = strEsp8266_Fram_Record.RxBuf;
	}
	return pRecStr;
}

char *Esp8266::itoa(int value, char *string, int radix)
{
	int i, d;
	int flag = 0;
	char *ptr = string;

	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
		*ptr = 0;
		return string;
	}

	if (!value)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}

	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
		*ptr++ = '-';

		/* Make the value positive. */
		value *= -1;

	}

	for (i = 10000; i > 0; i /= 10)
	{
		d = value / i;

		if (d || flag)
		{
			*ptr++ = (char)(d + 0x30);
			value -= (d *i);
			flag = 1;
		}
	}

	/* Null terminate the string. */
	*ptr = 0;

	return string;
}
#include <stdarg.h>
/*
* ��������USART2_printf
* ����  ����ʽ�������������C���е�printf��������û���õ�C��
* ����  ��-USARTx ����ͨ��������ֻ�õ��˴���2����USART2
*		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
*			   -...    ��������
* ���  ����
* ����  ����
* ����  ���ⲿ����
*         ����Ӧ��USART2_printf( USART2, "\r\n this is a demo \r\n" );
*            		 USART2_printf( USART2, "\r\n %d \r\n", i );
*            		 USART2_printf( USART2, "\r\n %s \r\n", j );
*/
void Esp8266::USART_printf(char *Data, ...)
{
	const char *s;
	int d;
	char buf[16];

	va_list ap;
	va_start(ap, Data);

	while (*Data != 0)
		// �ж��Ƿ񵽴��ַ���������
	{
		if (*Data == 0x5c)
			//'\'
		{
			switch (*++Data)
			{
			case 'r':
				//�س���
				USART_SendData(USART3, 0x0d);
				Data++;
				break;

			case 'n':
				//���з�
				USART_SendData(USART3, 0x0a);
				Data++;
				break;

			default:
				Data++;
				break;
			}
		}

		else if (*Data == '%')
		{
			//
			switch (*++Data)
			{
			case 's':
				//�ַ���
				s = va_arg(ap, const char*);
				for (; *s; s++)
				{
					USART_SendData(USART3, *s);
					while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
						;
				}
				Data++;
				break;
			case 'd':
				//ʮ����
				d = va_arg(ap, int);
				itoa(d, buf, 10);
				for (s = buf; *s; s++)
				{
					USART_SendData(USART3, *s);
					while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
						;
				}
				Data++;
				break;
			default:
				Data++;
				break;
			}
		}
		else
			USART_SendData(USART3, *Data++);

		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
			;
	}
}
void Esp8266::SetPin(Pin prst, Pin pch)
{
	this->pRst.Set(prst);
	this->pRst.Invert = false;
	this->pRst.OpenDrain = false;

	this->pCH.Set(pch);
	this->pCH.Invert = false;
	this->pCH.OpenDrain = false;

	this->pRst.Open();
	this->pCH.Open();

	this->pRst = 1;
	this->pCH = 0;
}