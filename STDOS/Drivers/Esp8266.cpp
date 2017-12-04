#include <stdio.h>
#include <string.h>  
#include <stdbool.h>
#include "Esp8266.h"
#include "Sys.h"

Fram_T strEsp8266_Fram_Record = 
{
    0
};


Esp8266::Esp8266(){}
Esp8266::~Esp8266(){}
void Esp8266::Init(ITransport *port){}
char com3bufrx[100];
char com3buftx[100];
void Esp8266::Init(COM idx, int baudrate)
{
    auto sp = new SerialPort(idx);

    sp->SetBaudRate(baudrate);
    sp->Rx.SetBuf(com3bufrx, ArrayLength(com3bufrx));
    sp->Tx.SetBuf(com3buftx, ArrayLength(com3buftx));

    this->Port = sp;
	
	this->_task = Sys.AddTask(&Esp8266::Routin, this,  500,  500, "espRtn");
}

void Esp8266::Set(Pin power, Pin rst, Pin low){

}
bool Esp8266::Config(){

}
void Esp8266::SetLed(Pin led){}
void Esp8266::SetLed(OutputPort &led){}
void Esp8266::RemoveLed(){}

/*
 * ��������ESP8266_AT_Test
 * ����  ����WF-ESP8266ģ�����AT��������
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
//void ESP8266_AT_Test ( void )
//{
//	macESP8266_RST_HIGH_LEVEL();
//	
//	Delay_ms ( 1000 ); 
//	
//	while ( ! this->SendCmd ( "AT", "OK", NULL, 500 ) ) ESP8266_Rst ();  	

//}
bool Esp8266::Test(int times, int interval)
{
	#if 0
//    char count = 0;    	
//	this->_Reset = 1;
//	this->cmdType=EspCmdType::TEST;
//	Delay_ms(1000-1);
//	while (count < times)
//	{
//		if (this->SendCmdNew("AT", "OK", NULL, interval))
//			return ;
//		this->Reset(false);
//		++count;
//	}
	#else
	this->cmdType=EspCmdType::TEST;
	this->SendCmdNew("AT", NULL, NULL, interval);
	#endif
    return true;
}
bool Esp8266::Reset(bool soft)
{
    if (soft)
    {
        this->SendCmd("AT+RST", "OK", "ready", 2500);
    }
    else
    {
        this->_Reset.Down(100);
    }
}

bool Esp8266::Sleep(uint ms)
{

}
// ����ģʽ����Ҫ����
bool Esp8266::SetMode(NetworkType mode)
{
	this->WorkMode=mode;
	this->cmdType=EspCmdType::SetMode;
	switch (this->WorkMode)
    {
        case NetworkType::Station:
            return this->SendCmdNew("AT+CWMODE=1", "OK", "no change", 2500);
        case NetworkType::AP:
            return this->SendCmdNew("AT+CWMODE=2", "OK", "no change", 2500);
        case NetworkType::STA_AP:
            return this->SendCmdNew("AT+CWMODE=3", "OK", "no change", 2500);
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
bool Esp8266::JoinAP(char *ssid, char *pass)
{
    char cCmd[120];
	this->cmdType=EspCmdType::JoinAP;
    sprintf(cCmd, "AT+CWJAP=\"%s\",\"%s\"", ssid, pass);
    return this->SendCmdNew(cCmd, "OK", NULL, 5000);
}
bool Esp8266::JoinAP(const String& ssid, const String& pass)
{
	return true;
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
	this->cmdType=EspCmdType::EnableMultipleId;
    sprintf(cStr, "AT+CIPMUX=%d", (enumEnUnvarnishTx ? 1 : 0));
    return this->SendCmdNew(cStr, "OK", 0, 500);
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
    return this->SendCmd(cCmd, "OK", "ALREAY CONNECT", 999);
}
/*
 * ��������SendCmd
 * ����  ����WF-ESP8266ģ�鷢��ATָ��
 * ����  ��cmd�������͵�ָ��
 *         reply1��reply2���ڴ�����Ӧ��ΪNULL������Ӧ������Ϊ���߼���ϵ
 *         waittime���ȴ���Ӧ��ʱ��
 * ����  : 1��ָ��ͳɹ�
 *         0��ָ���ʧ��
 * ����  �����ⲿ����
 */
bool Esp8266::SendCmdNew(char *cmd, char *reply1, char *reply2, int waittime)
{
    strEsp8266_Fram_Record .Length = 0; //���¿�ʼ�����µ����ݰ�
	this->Port->Rx.Clear();

    this->USART_printf("%s\r\n", cmd);

    return true;
}

/*
 * ��������SendCmd
 * ����  ����WF-ESP8266ģ�鷢��ATָ��
 * ����  ��cmd�������͵�ָ��
 *         reply1��reply2���ڴ�����Ӧ��ΪNULL������Ӧ������Ϊ���߼���ϵ
 *         waittime���ȴ���Ӧ��ʱ��
 * ����  : 1��ָ��ͳɹ�
 *         0��ָ���ʧ��
 * ����  �����ⲿ����
 */
bool Esp8266::SendCmd(char *cmd, char *reply1, char *reply2, int waittime)
{
    strEsp8266_Fram_Record .Length = 0; //���¿�ʼ�����µ����ݰ�

    this->USART_printf("%s\r\n", cmd);

    if ((reply1 == 0) && (reply2 == 0))
    //����Ҫ��������
        return true;

    Delay_ms(waittime); //��ʱ

    strEsp8266_Fram_Record .RxBuf[strEsp8266_Fram_Record .Length] = '\0';

    debug_printf("%s", strEsp8266_Fram_Record .RxBuf);

    if ((reply1 != 0) && (reply2 != 0))
        return ((bool)strstr(strEsp8266_Fram_Record .RxBuf, reply1) || (bool)strstr(strEsp8266_Fram_Record .RxBuf, reply2));

    else if (reply1 != 0)
        return ((bool)strstr(strEsp8266_Fram_Record .RxBuf, reply1));

    else
        return ((bool)strstr(strEsp8266_Fram_Record .RxBuf, reply2));
}






// �����յ������ݰ�
void Esp8266::Process()
{
	
}
// ���ݵ���
void Esp8266::OnReceive(Buffer& bs)
{
	switch(this->cmdType)
	{
		case EspCmdType::TEST:
			if(strstr(strEsp8266_Fram_Record .RxBuf, "OK"))
			{
				this->cmdType=EspCmdType::NONE;
				this->RunStep=1;
			}
			break;
		case EspCmdType::SetMode:
			if(strstr(strEsp8266_Fram_Record .RxBuf, "OK"))
			{
				this->cmdType=EspCmdType::NONE;
				this->RunStep=2;
			}
			if(strstr(strEsp8266_Fram_Record .RxBuf, "no change"))
			{
				this->cmdType=EspCmdType::NONE;
				this->RunStep=2;
			}
			break;
		case EspCmdType::JoinAP:
			if(strstr(strEsp8266_Fram_Record .RxBuf, "OK"))
			{
				this->cmdType=EspCmdType::NONE;
				this->RunStep=3;
			}
			break;
		case EspCmdType::EnableMultipleId:
			if(strstr(strEsp8266_Fram_Record .RxBuf, "OK"))
			{
				this->cmdType=EspCmdType::NONE;
				this->RunStep=4;
			}
			break;
		case EspCmdType::NONE:
		default:
			break;
	}
}

 //#define ApSsid                     "dd-wrt"               //Ҫ���ӵ��ȵ������
    #define ApSsid                       "NETGEAR77"        //Ҫ���ӵ��ȵ������
    #define ApPwd                        "18353217097"        //Ҫ���ӵ��ȵ����Կ
    //#define TcpServer_IP                 "121.42.164.17"      //Ҫ���ӵķ������� IP
    #define TcpServer_IP                 "192.168.0.169"      //Ҫ���ӵķ������� IP
    #define TcpServer_Port               "8888"               //Ҫ���ӵķ������Ķ˿�
//ѭ������
void Esp8266::Routin()
{
	uint8_t ucStatus;
        static int icnt = 0;
        char cStr[100] = 
        {
            0
        };
		

        switch (this->RunStep)
        {
            case 0:
                debug_printf("\r\n���ڲ������� ESP8266 ......\r\n");
                this->Test();
				break;
			case 1:
                this->SetMode(NetworkType::Station);
                break;
			case 2:				
				debug_printf("\r\n���������ȵ�ͷ����� ......\r\n");
                while (!this->JoinAP(ApSsid, ApPwd))
                    ;
                break;
            case 3:				
				this->EnableMultipleId(false);
                break;
            case 4:
				while (!this->LinkServer(Esp8266::enumTCP, TcpServer_IP, TcpServer_Port, Esp8266::SingleID0))
                    ;
				debug_printf("\r\n�����ȵ�ͷ������ɹ�\r\n");
                this->RunStep++;
                break;
            case 5:
				while (!this->UnvarnishSend())
                    ;
                debug_printf("\r\n���� ESP8266 ���\r\n");
                this->RunStep++;
                break;
            case 6:				
                sprintf(cStr, "%d hello world!\r\n", ++icnt);
                this->SendString(true, cStr, 0, Esp8266::SingleID0); //��������	
                debug_printf("��������: %s", cStr);
                Delay_ms(500);
                if (this->FlagTcpClosed)
                //����Ƿ�ʧȥ����
                {
                    this->ExitUnvarnishSend(); //�˳�͸��ģʽ			
                    do
                        ucStatus = this->GetLinkStatus();
                    //��ȡ����״̬
                    while (!ucStatus);
                    if (ucStatus == 4)
                    //ȷ��ʧȥ���Ӻ�����
                    {
						//esp.RunStep=1;
                        debug_printf("\r\n���������ȵ�ͷ����� ......\r\n");
                        while (!this->JoinAP(ApSsid, ApPwd))
                            ;
                        while (!this->LinkServer(Esp8266::enumTCP, TcpServer_IP, TcpServer_Port, Esp8266::SingleID0))
                            ;
                        debug_printf("\r\n�����ȵ�ͷ������ɹ�\r\n");
                    }
                    while (!this->UnvarnishSend())
                        ;

                }
                break;
            case 77:
				//��������
				this->RunStep=4;
                break;
			default:
                this->RunStep = 0;
                break;
        }
}

















/**
 * @brief  ESP8266��ʼ������
 * @param  ��
 * @retval ��
 */
void Esp8266::Init()
{
    this->_Reset=1;
    this->ChipEnable(false);

    this->RunStep = 0;
    this->FlagTcpClosed = 0; //�Ƿ�Ͽ�����

    this->ChipEnable();
}

//��������
void Esp8266::SetPin(Pin pinChEn, Pin pinReset)
{
    this->portEnable.Set(pinChEn);
    this->portEnable.OpenDrain = false;
    this->portEnable.Invert = 0;

    this->_Reset.Set(pinReset);
    this->_Reset.OpenDrain = false;
    this->_Reset.Invert = 0;

    this->portEnable.Open();

    this->_Reset.Open();
}

//����оƬ��Ч
void Esp8266::ChipEnable(bool en)
{
    this->portEnable = en;
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
    return this->SendCmd(cCmd, "OK", 0, 1000);
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

        return (this->SendCmd(cCmd1, "OK", 0, 500) && this->SendCmd(cCmd2, "OK", 0, 500));
    }
    else
    {
        sprintf(cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum);

        return this->SendCmd(cCmd1, "OK", 0, 500);
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
    if (this->SendCmd("AT+CIPSTATUS", "OK", 0, 500))
    {
        if (strstr(strEsp8266_Fram_Record .RxBuf, "STATUS:2\r\n"))
            return 2;

        else if (strstr(strEsp8266_Fram_Record .RxBuf, "STATUS:3\r\n"))
            return 3;

        else if (strstr(strEsp8266_Fram_Record .RxBuf, "STATUS:4\r\n"))
            return 4;
    }
    return 0;
}

/*
 * ��������ESP8266_Get_IdLinkStatus
 * ����  ����ȡ WF-ESP8266 �Ķ˿ڣ�Id������״̬�����ʺ϶�˿�ʱʹ��
 * ����  ����
 * ����  : �˿ڣ�Id��������״̬����5λΪ��Чλ���ֱ��ӦId5~0��ĳλ����1���Id���������ӣ�������0���Idδ��������
 * ����  �����ⲿ����
 */
int Esp8266::GetIdLinkStatus()
{
    uint8_t ucIdLinkStatus = 0x00;


    if (this->SendCmd("AT+CIPSTATUS", "OK", 0, 500))
    {
        if (strstr(strEsp8266_Fram_Record .RxBuf, "+CIPSTATUS:0,"))
            ucIdLinkStatus |= 0x01;
        else
            ucIdLinkStatus &= ~0x01;

        if (strstr(strEsp8266_Fram_Record .RxBuf, "+CIPSTATUS:1,"))
            ucIdLinkStatus |= 0x02;
        else
            ucIdLinkStatus &= ~0x02;

        if (strstr(strEsp8266_Fram_Record .RxBuf, "+CIPSTATUS:2,"))
            ucIdLinkStatus |= 0x04;
        else
            ucIdLinkStatus &= ~0x04;

        if (strstr(strEsp8266_Fram_Record .RxBuf, "+CIPSTATUS:3,"))
            ucIdLinkStatus |= 0x08;
        else
            ucIdLinkStatus &= ~0x08;

        if (strstr(strEsp8266_Fram_Record .RxBuf, "+CIPSTATUS:4,"))
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
    this->SendCmd("AT+CIFSR", "OK", 0, 500);
    pCh = strstr(strEsp8266_Fram_Record .RxBuf, "APIP,\"");
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
    if (!this->SendCmd("AT+CIPMODE=1", "OK", 0, 500))
        return false;
    return this->SendCmd("AT+CIPSEND", "OK", ">", 500);
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
    Delay_ms(1000);
    this->USART_printf("+++");
    Delay_ms(500);
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
        this->SendCmd(cStr, "> ", 0, 1000);
        bRet = this->SendCmd(pStr, "SEND OK", 0, 1000);
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
    strEsp8266_Fram_Record .Length = 0;
    strEsp8266_Fram_Record .FlagFinish = 0;
    while (!strEsp8266_Fram_Record .FlagFinish)
        ;
    strEsp8266_Fram_Record .RxBuf[strEsp8266_Fram_Record .Length] = '\0';
    if (enumEnUnvarnishTx)
        pRecStr = strEsp8266_Fram_Record .RxBuf;
    else
    {
        if (strstr(strEsp8266_Fram_Record .RxBuf, "+IPD"))
            pRecStr = strEsp8266_Fram_Record .RxBuf;
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
        value *=  - 1;

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

    char bufSend[200];
    int bufSendPos;
    int bufSendMax = ArrayLength(bufSend);

    bufSendPos = 0;

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
                    bufSend[bufSendPos++] = 0X0D;
                    Data++;
                    break;

                case 'n':
                    //���з�
                    bufSend[bufSendPos++] = 0X0A;
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
                    for (;  *s; s++)
                    {
                        bufSend[bufSendPos++] =  *s;
                    }
                    Data++;
                    break;
                case 'd':
                    //ʮ����
                    d = va_arg(ap, int);
                    itoa(d, buf, 10);
                    for (s = buf;  *s; s++)
                    {
                        bufSend[bufSendPos++] =  *s;
                    }
                    Data++;
                    break;
                default:
                    Data++;
                    break;
            }
        }
        else
            bufSend[bufSendPos++] =  *Data++;
    }
	#if 1
	Buffer bs(bufSend,bufSendPos);
	this->Port->Write(bs);
	#else
    this->SendData1(&bufSend[0], bufSendPos);
	#endif
}

#include "stm32f10x.h" 
//��������
void Esp8266::SendData1(char *buf, int len)
{
    #if 1
        USART_TypeDef *USARTx = USART3;
        for (int i = 0; i < len; i++)
        {
            USART_SendData(USARTx, buf[i]);
            while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
                ;
        }
    #else 
        if (this->psp)
        {
            Buffer bs(buf, len);
            this->psp->Write(bs);
        }
    #endif 
}
