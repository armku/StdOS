#include <stdio.h>
#include <string.h>  
#include <stdbool.h>
#include "Esp8266.h"
#include "Sys.h"

//#define ApSsid                     "dd-wrt"               //Ҫ���ӵ��ȵ������
#define ApSsid                       "NETGEAR77"        //Ҫ���ӵ��ȵ������
#define ApPwd                        "18353217097"        //Ҫ���ӵ��ȵ����Կ
//#define TcpServer_IP                 "121.42.164.17"      //Ҫ���ӵķ������� IP
#define TcpServer_IP                 "192.168.0.169"      //Ҫ���ӵķ������� IP
//#define TcpServer_IP                 "10.0.0.12"      //Ҫ���ӵķ������� IP
#define TcpServer_Port               "8888"               //Ҫ���ӵķ������Ķ˿�


Fram_T strEsp8266_Fram_Record = 
{
    0
};

Esp8266::Esp8266(){}
Esp8266::~Esp8266(){}
void Esp8266::Init(ITransport *port){}
char com3bufrx[100];
char com3buftx[100];
	
	
// ���ݵ���
void Esp8266::OnReceive(Buffer &bs)
{
    strEsp8266_Fram_Record .Length = bs.Length();
    for (int i = 0; i < strEsp8266_Fram_Record .Length; i++)
    {
        strEsp8266_Fram_Record .RxBuf[i] = bs[i];
    }
    strEsp8266_Fram_Record .FlagFinish = 1;
    this->FlagTcpClosed = strstr(strEsp8266_Fram_Record .RxBuf, "CLOSED\r\n") ? 1 : 0;
    switch (this->cmdType)
    {
        case EspCmdType::ETEST: 
			if ((this->bufrcvcnt)&&(strstr(strEsp8266_Fram_Record .RxBuf, this->bufrcv1)))
			{
				this->cmdType = EspCmdType::ENONE;
				//this->RunStep = 1;
			}
			break;
        case EspCmdType::ESetMode: 
			if ((this->bufrcvcnt)&&(strstr(strEsp8266_Fram_Record .RxBuf, this->bufrcv1)))
			{
				this->cmdType = EspCmdType::ENONE;
				this->RunStep = 2;
			}			
			if ((this->bufrcvcnt==2)&&(strstr(strEsp8266_Fram_Record .RxBuf, this->bufrcv2)))
			{
				this->cmdType = EspCmdType::ENONE;
				this->RunStep = 2;
			}			
			break;
        case EspCmdType::EJoinAP: 
			if ((this->bufrcvcnt)&&(strstr(strEsp8266_Fram_Record .RxBuf, this->bufrcv1)))
			{
				this->cmdType = EspCmdType::ENONE;
				this->RunStep = 3;
			}			
			if ((this->bufrcvcnt==2)&&(strstr(strEsp8266_Fram_Record .RxBuf, this->bufrcv2)))
			{
				this->cmdType = EspCmdType::ENONE;
				this->RunStep = 3;
			}			
			break;
        case EspCmdType::EEnableMultipleId: 
			if ((this->bufrcvcnt)&&(strstr(strEsp8266_Fram_Record .RxBuf, this->bufrcv1)))
			{
				this->cmdType = EspCmdType::ENONE;
				this->RunStep = 4;
			}			
			if ((this->bufrcvcnt==2)&&(strstr(strEsp8266_Fram_Record .RxBuf, this->bufrcv2)))
			{
				this->cmdType = EspCmdType::ENONE;
				this->RunStep = 4;
			}			
			break;
        case EspCmdType::ELinkServer: 
			if ((this->bufrcvcnt)&&(strstr(strEsp8266_Fram_Record .RxBuf, this->bufrcv1)))
			{
				this->cmdType = EspCmdType::ENONE;
				this->RunStep = 5;
			}			
			if ((this->bufrcvcnt==2)&&(strstr(strEsp8266_Fram_Record .RxBuf, this->bufrcv2)))
			{
				this->cmdType = EspCmdType::ENONE;
				this->RunStep = 5;
			}			
			break;
        case EspCmdType::EUnvarnishSend: if (strstr(strEsp8266_Fram_Record .RxBuf, "OK"))
        {
            this->cmdType = EspCmdType::ENONE;
            this->RunStep = 6;
        }
        break;
        case EspCmdType::ENONE: default:
            break;
    }
}

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
            this->Test();            
            break;
        case 1:
            debug_printf("\r\n�������ù���ģʽ ......\r\n");
            this->SetMode(NetworkType::Station);            
            break;
        case 2:
            debug_printf("\r\n���������ȵ�ͷ����� ......\r\n");
            this->JoinAP(ApSsid, ApPwd);            
            break;
        case 3:
            this->EnableMultipleId(false);
            this->cmdType = EspCmdType::EEnableMultipleId;
            break;
        case 4:
            this->LinkServer(Esp8266::enumTCP, TcpServer_IP, TcpServer_Port, Esp8266::SingleID0);
            debug_printf("\r\n�����ȵ�ͷ������ɹ�\r\n");
            break;
        case 5:
            this->UnvarnishSend();
            this->cmdType = EspCmdType::EUnvarnishSend;
            debug_printf("\r\n���� ESP8266 ���\r\n");
            break;
        case 6:
            this->Port->Printf("%d hello world!\r\n", ++icnt);
            debug_printf("��������: %d hello world!\r\n", ++icnt);
            Sys.Sleep(500);
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
            this->RunStep = 4;
            break;
        default:
            this->RunStep = 0;
            break;
    }
}

void Esp8266::Init(COM idx, int baudrate)
{
    auto sp = new SerialPort(idx);

    sp->SetBaudRate(baudrate);
    sp->Rx.SetBuf(com3bufrx, ArrayLength(com3bufrx));
    sp->Tx.SetBuf(com3buftx, ArrayLength(com3buftx));	
	
    this->Port = sp;
	
	this->At.Port=sp;
	this->At.Open();
	
	this->_Reset = 1;
	this->portEnable = 0;
	
    this->RunStep = 0;
    this->FlagTcpClosed = 0; //�Ƿ�Ͽ�����

//	this->portEnable=1;
	
    //this->_task = Sys.AddTask(&Esp8266::Routin, this, 500, 1000, "espRtn");
}

void Esp8266::Set(Pin power, Pin rst, Pin low){

}
bool Esp8266::Config()
{
	return false;
}
void Esp8266::SetLed(Pin led){}
void Esp8266::SetLed(OutputPort &led){}
void Esp8266::RemoveLed(){}

//���ý��յ��ַ���
void Esp8266::SetRcv(char *rcv1, char *rcv2,int rcvcnt)
{
	for(int i=0;i<20;i++)
	{
		this->bufrcv1[i]=rcv1[i];
		if(rcv1[i]==0)
			break;
	}
	for(int i=0;i<20;i++)
	{
		this->bufrcv2[i]=rcv2[i];
		if(rcv2[i]==0)
			break;
	}
	this->bufrcvcnt=rcvcnt;
}
bool Esp8266::Test(int times, int interval)
{
    this->At.SendCmd("AT");
	this->SetRcv("OK",NULL,1);
	this->cmdType = EspCmdType::ETEST;
    return true;
}

bool Esp8266::Reset(bool soft)
{
    if (soft)
    {
        this->At.SendCmd("AT+RST");
		this->SetRcv("OK", "ready",2);
		//this->cmdType = EspCmdType::ESetMode;
    }
    else
    {
        this->_Reset.Down(100);
    }
	return false;
}

bool Esp8266::Sleep(uint ms)
{
	return false;
}
// ����ģʽ����Ҫ����
bool Esp8266::SetMode(NetworkType mode)
{
    this->WorkMode = mode;

    switch (this->WorkMode)
    {
        case NetworkType::Station: 
			this->At.SendCmd("AT+CWMODE=1");
			break;
        case NetworkType::AP: 
			this->At.SendCmd("AT+CWMODE=2");
			break;
        case NetworkType::STA_AP: 
			this->At.SendCmd("AT+CWMODE=3");
			break;
        default:
            return false;
    }
	this->SetRcv("OK", "no change",2);
	this->cmdType = EspCmdType::ESetMode;
	return true;
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
	this->Port->Printf("AT+CWJAP=\"%s\\r\n", ssid, pass);
	this->SetRcv("OK", NULL,1);
	this->cmdType = EspCmdType::EJoinAP;
	
	return false;
}

bool Esp8266::JoinAP(const String &ssid, const String &pass)
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
	this->Port->Printf("AT+CIPMUX=%d\r\n", (enumEnUnvarnishTx ? 1 : 0));
	this->SetRcv("OK", NULL,1);
	this->cmdType = EspCmdType::EJoinAP;
	
	return false;
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
	if (id < 5)	
	{		
		switch (enumE)
		{
			case enumTCP:
				this->Port->Printf("AT+CIPSTART=%d,\"%s\",\"%s\",%s\r\n", id, "TCP", ip, ComNum);
				break;
			case enumUDP:
				this->Port->Printf("AT+CIPSTART=%d,\"%s\",\"%s\",%s\r\n", id, "UDP", ip, ComNum);
				break;
			default:
				break;
		}    
	}
	else
	{
		switch (enumE)
		{
			case enumTCP:
				this->Port->Printf("AT+CIPSTART=\"%s\",\"%s\",%s\r\n", "TCP", ip, ComNum);
				break;
			case enumUDP:
				this->Port->Printf("AT+CIPSTART=\"%s\",\"%s\",%s\r\n", "UDP", ip, ComNum);
				break;
			default:
				break;
		}		
	}
	this->SetRcv("OK", "ALREAY CONNECT",2);
	this->cmdType = EspCmdType::ELinkServer;
	return true;
}

// �����յ������ݰ�
void Esp8266::Process(){

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
    this->At.SendCmd("AT+CIPSTATUS");
    
	if (strstr(strEsp8266_Fram_Record .RxBuf, "STATUS:2\r\n"))
		return 2;

	else if (strstr(strEsp8266_Fram_Record .RxBuf, "STATUS:3\r\n"))
		return 3;

	else if (strstr(strEsp8266_Fram_Record .RxBuf, "STATUS:4\r\n"))
            return 4;
    
	this->cmdType = EspCmdType::EGetLinkStatus;
    return 0;
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
	this->At.SendCmd("AT+CIPMODE=1");    	
	this->SetRcv("OK", NULL,1);
	this->cmdType = EspCmdType::EUnvarnishSend;
	return true;
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
	this->At.SendCmd("+++");    	
	this->SetRcv("OK", NULL,1);
	this->cmdType = EspCmdType::EExitUnvarnishSend;	
}
