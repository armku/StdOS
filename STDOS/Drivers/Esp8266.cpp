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
 * 函数名：ESP8266_AT_Test
 * 描述  ：对WF-ESP8266模块进行AT测试启动
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
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
// 设置模式。需要重启
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
 * 函数名：ESP8266_Enable_MultipleId
 * 描述  ：WF-ESP8266模块启动多连接
 * 输入  ：enumEnUnvarnishTx，配置是否多连接
 * 返回  : 1，配置成功
 *         0，配置失败
 * 调用  ：被外部调用
 */
bool Esp8266::EnableMultipleId(bool enumEnUnvarnishTx)
{
    char cStr[20];
	this->cmdType=EspCmdType::EnableMultipleId;
    sprintf(cStr, "AT+CIPMUX=%d", (enumEnUnvarnishTx ? 1 : 0));
    return this->SendCmdNew(cStr, "OK", 0, 500);
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
 * 函数名：SendCmd
 * 描述  ：对WF-ESP8266模块发送AT指令
 * 输入  ：cmd，待发送的指令
 *         reply1，reply2，期待的响应，为NULL表不需响应，两者为或逻辑关系
 *         waittime，等待响应的时间
 * 返回  : 1，指令发送成功
 *         0，指令发送失败
 * 调用  ：被外部调用
 */
bool Esp8266::SendCmdNew(char *cmd, char *reply1, char *reply2, int waittime)
{
    strEsp8266_Fram_Record .Length = 0; //从新开始接收新的数据包
	this->Port->Rx.Clear();

    this->USART_printf("%s\r\n", cmd);

    return true;
}

/*
 * 函数名：SendCmd
 * 描述  ：对WF-ESP8266模块发送AT指令
 * 输入  ：cmd，待发送的指令
 *         reply1，reply2，期待的响应，为NULL表不需响应，两者为或逻辑关系
 *         waittime，等待响应的时间
 * 返回  : 1，指令发送成功
 *         0，指令发送失败
 * 调用  ：被外部调用
 */
bool Esp8266::SendCmd(char *cmd, char *reply1, char *reply2, int waittime)
{
    strEsp8266_Fram_Record .Length = 0; //从新开始接收新的数据包

    this->USART_printf("%s\r\n", cmd);

    if ((reply1 == 0) && (reply2 == 0))
    //不需要接收数据
        return true;

    Delay_ms(waittime); //延时

    strEsp8266_Fram_Record .RxBuf[strEsp8266_Fram_Record .Length] = '\0';

    debug_printf("%s", strEsp8266_Fram_Record .RxBuf);

    if ((reply1 != 0) && (reply2 != 0))
        return ((bool)strstr(strEsp8266_Fram_Record .RxBuf, reply1) || (bool)strstr(strEsp8266_Fram_Record .RxBuf, reply2));

    else if (reply1 != 0)
        return ((bool)strstr(strEsp8266_Fram_Record .RxBuf, reply1));

    else
        return ((bool)strstr(strEsp8266_Fram_Record .RxBuf, reply2));
}






// 处理收到的数据包
void Esp8266::Process()
{
	
}
// 数据到达
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

 //#define ApSsid                     "dd-wrt"               //要连接的热点的名称
    #define ApSsid                       "NETGEAR77"        //要连接的热点的名称
    #define ApPwd                        "18353217097"        //要连接的热点的密钥
    //#define TcpServer_IP                 "121.42.164.17"      //要连接的服务器的 IP
    #define TcpServer_IP                 "192.168.0.169"      //要连接的服务器的 IP
    #define TcpServer_Port               "8888"               //要连接的服务器的端口
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
                debug_printf("\r\n正在测试在线 ESP8266 ......\r\n");
                this->Test();
				break;
			case 1:
                this->SetMode(NetworkType::Station);
                break;
			case 2:				
				debug_printf("\r\n正在重连热点和服务器 ......\r\n");
                while (!this->JoinAP(ApSsid, ApPwd))
                    ;
                break;
            case 3:				
				this->EnableMultipleId(false);
                break;
            case 4:
				while (!this->LinkServer(Esp8266::enumTCP, TcpServer_IP, TcpServer_Port, Esp8266::SingleID0))
                    ;
				debug_printf("\r\n重连热点和服务器成功\r\n");
                this->RunStep++;
                break;
            case 5:
				while (!this->UnvarnishSend())
                    ;
                debug_printf("\r\n配置 ESP8266 完毕\r\n");
                this->RunStep++;
                break;
            case 6:				
                sprintf(cStr, "%d hello world!\r\n", ++icnt);
                this->SendString(true, cStr, 0, Esp8266::SingleID0); //发送数据	
                debug_printf("发送数据: %s", cStr);
                Delay_ms(500);
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
				this->RunStep=4;
                break;
			default:
                this->RunStep = 0;
                break;
        }
}

















/**
 * @brief  ESP8266初始化函数
 * @param  无
 * @retval 无
 */
void Esp8266::Init()
{
    this->_Reset=1;
    this->ChipEnable(false);

    this->RunStep = 0;
    this->FlagTcpClosed = 0; //是否断开连接

    this->ChipEnable();
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

//设置芯片有效
void Esp8266::ChipEnable(bool en)
{
    this->portEnable = en;
}

/*
 * 函数名：ESP8266_BuildAP
 * 描述  ：WF-ESP8266模块创建WiFi热点
 * 输入  ：pSSID，WiFi名称字符串
 *       ：pPassWord，WiFi密码字符串
 *       ：enunPsdMode，WiFi加密方式代号字符串
 * 返回  : 1，创建成功
 *         0，创建失败
 * 调用  ：被外部调用
 */
bool Esp8266::BuildAP(char *pSSID, char *pPassWord, ENUMAPPsdModeTypeDef enunPsdMode)
{
    char cCmd[120];
    sprintf(cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode);
    return this->SendCmd(cCmd, "OK", 0, 1000);
}





/*
 * 函数名：StartOrShutServer
 * 描述  ：WF-ESP8266模块开启或关闭服务器模式
 * 输入  ：enumMode，开启/关闭
 *       ：pPortNum，服务器端口号字符串
 *       ：pTimeOver，服务器超时时间字符串，单位：秒
 * 返回  : 1，操作成功
 *         0，操作失败
 * 调用  ：被外部调用
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
 * 函数名：ESP8266_Get_IdLinkStatus
 * 描述  ：获取 WF-ESP8266 的端口（Id）连接状态，较适合多端口时使用
 * 输入  ：无
 * 返回  : 端口（Id）的连接状态，低5位为有效位，分别对应Id5~0，某位若置1表该Id建立了连接，若被清0表该Id未建立连接
 * 调用  ：被外部调用
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
 * 函数名：InquireApIp
 * 描述  ：获取 F-ESP8266 的 AP IP
 * 输入  ：pApIp，存放 AP IP 的数组的首地址
 *         ucArrayLength，存放 AP IP 的数组的长度
 * 返回  : 0，获取失败
 *         1，获取成功
 * 调用  ：被外部调用
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
 * 函数名：UnvarnishSend
 * 描述  ：配置WF-ESP8266模块进入透传发送
 * 输入  ：无
 * 返回  : 1，配置成功
 *         0，配置失败
 * 调用  ：被外部调用
 */
bool Esp8266::UnvarnishSend()
{
    if (!this->SendCmd("AT+CIPMODE=1", "OK", 0, 500))
        return false;
    return this->SendCmd("AT+CIPSEND", "OK", ">", 500);
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
    Delay_ms(1000);
    this->USART_printf("+++");
    Delay_ms(500);
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
 * 函数名：ESP8266_ReceiveString
 * 描述  ：WF-ESP8266模块接收字符串
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 * 返回  : 接收到的字符串首地址
 * 调用  ：被外部调用
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
 * 函数名：USART2_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口2，即USART2
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART2_printf( USART2, "\r\n this is a demo \r\n" );
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
    // 判断是否到达字符串结束符
    {
        if (*Data == 0x5c)
        //'\'
        {
            switch (*++Data)
            {
                case 'r':
                    //回车符
                    bufSend[bufSendPos++] = 0X0D;
                    Data++;
                    break;

                case 'n':
                    //换行符
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
                    //字符串
                    s = va_arg(ap, const char*);
                    for (;  *s; s++)
                    {
                        bufSend[bufSendPos++] =  *s;
                    }
                    Data++;
                    break;
                case 'd':
                    //十进制
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
//发送数据
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
