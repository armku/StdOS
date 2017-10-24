#include "Sys.h"
#include "stm32f10x.h"
#include <stdarg.h>
#include "bsp_esp8266.h"

#define macESP8266_CH_ENABLE()                 GPIO_SetBits ( GPIOG, GPIO_Pin_13 )

#define      macUser_ESP8266_ApSsid                       "NETGEAR77"                //要连接的热点的名称
#define      macUser_ESP8266_ApPwd                        "18353217097"           //要连接的热点的密钥
#define      macUser_ESP8266_TcpServer_IP                 "121.42.164.17"      //要连接的服务器的 IP
#define      macUser_ESP8266_TcpServer_Port               "8000"               //要连接的服务器的端口

volatile uint8_t ucTcpClosedFlag = 0;

char cStr[1500] = 
{
    0
};
ESP8266 esp;
void ESP8266TEST()
{    
    esp.Init(); //初始化WiFi模块使用的接口和外设
    printf("\r\n野火 WF-ESP8266 WiFi模块测试例程\r\n"); //打印测试例程提示信息
    
	uint8_t ucStatus;

    char cStr[100] = 
    {
        0
    };
    printf("\r\n正在配置 ESP8266 ......\r\n");
    macESP8266_CH_ENABLE();
    esp.AT_Test();
    esp.Net_Mode_Choose(STA);
    while (!esp.JoinAP(macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd))
        ;
    esp.Enable_MultipleId(DISABLE);
    while (!esp.Link_Server(enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0))
        ;    while (!esp.UnvarnishSend())
        ;    printf("\r\n配置 ESP8266 完毕\r\n");
    while (1)
    {
        sprintf(cStr, "Hello world!\r\n");

        esp.SendString(ENABLE, cStr, 0, Single_ID_0); //发送数据
        Sys.Sleep(100);
        if (ucTcpClosedFlag)
        //检测是否失去连接
        {
            esp.ExitUnvarnishSend(); //退出透传模式
            do
                ucStatus = esp.Get_LinkStatus();
            //获取连接状态
            while (!ucStatus);
            if (ucStatus == 4)
            //确认失去连接后重连
            {
                printf("\r\n正在重连热点和服务器 ......\r\n");
                while (!esp.JoinAP(macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd))
                    ;
                while (!esp.Link_Server(enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0))
                    ;
                printf("\r\n重连热点和服务器成功\r\n");
            }
            while (!esp.UnvarnishSend());
        }
    }
}
