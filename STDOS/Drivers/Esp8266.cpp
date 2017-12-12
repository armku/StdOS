#include <stdio.h>
#include <string.h>  
#include <stdbool.h>
#include "Esp8266.h"
#include "Sys.h"

//#define ApSsid                     "dd-wrt"               //要连接的热点的名称
#define ApSsid                       "NETGEAR77"        //要连接的热点的名称
#define ApPwd                        "18353217097"        //要连接的热点的密钥
//#define TcpServer_IP                 "121.42.164.17"      //要连接的服务器的 IP
#define TcpServer_IP                 "192.168.0.169"      //要连接的服务器的 IP
//#define TcpServer_IP                 "10.0.0.12"      //要连接的服务器的 IP
#define TcpServer_Port               "8888"               //要连接的服务器的端口


Fram_T strEsp8266_Fram_Record = 
{
    0
};

Esp8266::Esp8266(){}
Esp8266::~Esp8266(){}
void Esp8266::Init(ITransport *port){}
char com3bufrx[100];
char com3buftx[100];
	
	
// 数据到达
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

//循环运行
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
            debug_printf("\r\n正在设置工作模式 ......\r\n");
            this->SetMode(NetworkType::Station);            
            break;
        case 2:
            debug_printf("\r\n正在重连热点和服务器 ......\r\n");
            this->JoinAP(ApSsid, ApPwd);            
            break;
        case 3:
            this->EnableMultipleId(false);
            this->cmdType = EspCmdType::EEnableMultipleId;
            break;
        case 4:
            this->LinkServer(Esp8266::enumTCP, TcpServer_IP, TcpServer_Port, Esp8266::SingleID0);
            debug_printf("\r\n重连热点和服务器成功\r\n");
            break;
        case 5:
            this->UnvarnishSend();
            this->cmdType = EspCmdType::EUnvarnishSend;
            debug_printf("\r\n配置 ESP8266 完毕\r\n");
            break;
        case 6:
            this->Port->Printf("%d hello world!\r\n", ++icnt);
            debug_printf("发送数据: %d hello world!\r\n", ++icnt);
            Sys.Sleep(500);
            if (this->FlagTcpClosed)
            //检测是否失去连接
            {
                this->ExitUnvarnishSend(); //退出透传模式			
                do
                    ucStatus = this->GetLinkStatus();
                //获取连接状态
                while (!ucStatus);
                if (ucStatus == 4)
                //确认失去连接后重连
                {
                    //esp.RunStep=1;
                    debug_printf("\r\n正在重连热点和服务器 ......\r\n");
                    while (!this->JoinAP(ApSsid, ApPwd))
                        ;
                    while (!this->LinkServer(Esp8266::enumTCP, TcpServer_IP, TcpServer_Port, Esp8266::SingleID0))
                        ;
                    debug_printf("\r\n重连热点和服务器成功\r\n");
                }
                while (!this->UnvarnishSend())
                    ;
            }
            break;
        case 77:
            //重新连接
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
    this->FlagTcpClosed = 0; //是否断开连接

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

//设置接收的字符串
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
// 设置模式。需要重启
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
 * 函数名：ESP8266_JoinAP
 * 描述  ：WF-ESP8266模块连接外部WiFi
 * 输入  ：pSSID，WiFi名称字符串
 *       ：pPassWord，WiFi密码字符串
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_Enable_MultipleId
 * 描述  ：WF-ESP8266模块启动多连接
 * 输入  ：enumEnUnvarnishTx，配置是否多连接
 * 返回  : 1，配置成功
 *         0，配置失败
 * 调用  ：被外部调用
 */
bool Esp8266::EnableMultipleId(bool enumEnUnvarnishTx)
{    
	this->Port->Printf("AT+CIPMUX=%d\r\n", (enumEnUnvarnishTx ? 1 : 0));
	this->SetRcv("OK", NULL,1);
	this->cmdType = EspCmdType::EJoinAP;
	
	return false;
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

// 处理收到的数据包
void Esp8266::Process(){

}













//设置引脚
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
 * 函数名：GetLinkStatus
 * 描述  ：获取 WF-ESP8266 的连接状态，较适合单端口时使用
 * 输入  ：无
 * 返回  : 2，获得ip
 *         3，建立连接
 *         3，失去连接
 *         0，获取状态失败
 * 调用  ：被外部调用
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
 * 函数名：UnvarnishSend
 * 描述  ：配置WF-ESP8266模块进入透传发送
 * 输入  ：无
 * 返回  : 1，配置成功
 *         0，配置失败
 * 调用  ：被外部调用
 */
bool Esp8266::UnvarnishSend()
{	
	this->At.SendCmd("AT+CIPMODE=1");    	
	this->SetRcv("OK", NULL,1);
	this->cmdType = EspCmdType::EUnvarnishSend;
	return true;
}

/*
 * 函数名：ExitUnvarnishSend
 * 描述  ：配置WF-ESP8266模块退出透传模式
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
void Esp8266::ExitUnvarnishSend()
{	
	this->At.SendCmd("+++");    	
	this->SetRcv("OK", NULL,1);
	this->cmdType = EspCmdType::EExitUnvarnishSend;	
}
