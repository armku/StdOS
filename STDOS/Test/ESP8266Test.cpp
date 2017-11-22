#include "Drivers\Esp8266.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>

void Delay_ms(int ms);

#define ESP8266TEST

#ifdef ESP8266TEST
    #define ApSsid                     "dd-wrt"               //要连接的热点的名称
    //#define ApSsid                       "NETGEAR77"        //要连接的热点的名称
    #define ApPwd                        "18353217097"        //要连接的热点的密钥
    #define TcpServer_IP                 "121.42.164.17"      //要连接的服务器的 IP
    #define TcpServer_Port               "8000"               //要连接的服务器的端口

    volatile uint8_t ucTcpClosedFlag = 0;
    Esp8266 esp;
    char cStr[1500] = 
    {
        0
    };
    /**
     * @brief  ESP8266 （Sta Tcp Client）透传
     * @param  无
     * @retval 无
     */
    void ESP8266Test()
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
        esp.Test();
        esp.NetModeChoose(Esp8266::STA);
        while (!esp.JoinAP(ApSsid, ApPwd))
            ;
        esp.EnableMultipleId(DISABLE);
        while (!esp.LinkServer(Esp8266::enumTCP, TcpServer_IP, TcpServer_Port, Esp8266::SingleID0))
            ;
        while (!esp.UnvarnishSend())
            ;
        printf("\r\n配置 ESP8266 完毕\r\n");
        while (1)
        {
            sprintf(cStr, "hello world!\r\n");
            esp.SendString(ENABLE, cStr, 0, Esp8266::SingleID0); //发送数据		
            Delay_ms(10000);
            if (ucTcpClosedFlag)
            //检测是否失去连接
            {
                esp.ExitUnvarnishSend(); //退出透传模式			
                do
                    ucStatus = esp.GetLinkStatus();
                //获取连接状态
                while (!ucStatus);
                if (ucStatus == 4)
                //确认失去连接后重连
                {
                    printf("\r\n正在重连热点和服务器 ......\r\n");
                    while (!esp.JoinAP(ApSsid, ApPwd))
                        ;
                    while (!esp.LinkServer(Esp8266::enumTCP, TcpServer_IP, TcpServer_Port, Esp8266::SingleID0))
                        ;
                    printf("\r\n重连热点和服务器成功\r\n");
                }
                while (!esp.UnvarnishSend())
                    ;

            }
        }
    }
    #ifdef __cplusplus
        extern "C"
        {
        #endif 
        void USART3_IRQHandler(void)
        {
            uint8_t ucCh;

            if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
            {
                ucCh = USART_ReceiveData(USART3);

                if (strEsp8266_Fram_Record .InfBit .FramLength < (RX_BUF_MAX_LEN - 1))
                //预留1个字节写结束符
                    strEsp8266_Fram_Record .Data_RX_BUF[strEsp8266_Fram_Record .InfBit .FramLength++] = ucCh;
            }

            if (USART_GetITStatus(USART3, USART_IT_IDLE) == SET)
            //数据帧接收完毕
            {
                strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;

                ucCh = USART_ReceiveData(USART3); //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)
                ucTcpClosedFlag = strstr(strEsp8266_Fram_Record .Data_RX_BUF, "CLOSED\r\n") ? 1 : 0;
            }
        }
        #ifdef __cplusplus
        }
    #endif 
#endif
