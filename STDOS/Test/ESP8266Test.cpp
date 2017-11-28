#include "Drivers\Esp8266.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "stm32f10x.h" 
#include "SerialPort.h"
#include "Sys.h"

#define ESP8266TEST

#ifdef ESP8266TEST
    //#define ApSsid                     "dd-wrt"               //要连接的热点的名称
    #define ApSsid                       "NETGEAR77"        //要连接的热点的名称
    #define ApPwd                        "18353217097"        //要连接的热点的密钥
    #define TcpServer_IP                 "121.42.164.17"      //要连接的服务器的 IP
    //#define TcpServer_IP                 "192.168.0.120"      //要连接的服务器的 IP
    #define TcpServer_Port               "8000"               //要连接的服务器的端口

    Esp8266 esp;

    void espRoutin(void *param)
    {
        uint8_t ucStatus;
        static int icnt = 0;
        char cStr[100] = 
        {
            0
        };

        switch (esp.RunStep)
        {
            case 0:
                debug_printf("\r\n正在测试在线 ESP8266 ......\r\n");
                esp.Test();
				esp.NetModeChoose(Esp8266::STA);
                esp.RunStep++;
                break;
			case 1:				
				debug_printf("\r\n正在重连热点和服务器 ......\r\n");
                while (!esp.JoinAP(ApSsid, ApPwd))
                    ;
                esp.EnableMultipleId(DISABLE);
                esp.RunStep++;
                break;
            case 2:
				while (!esp.LinkServer(Esp8266::enumTCP, TcpServer_IP, TcpServer_Port, Esp8266::SingleID0))
                    ;
				debug_printf("\r\n重连热点和服务器成功\r\n");
                esp.RunStep++;
                break;
            case 3:
				while (!esp.UnvarnishSend())
                    ;
                debug_printf("\r\n配置 ESP8266 完毕\r\n");
                esp.RunStep++;
                break;
            case 4:
                sprintf(cStr, "%d hello world!\r\n", ++icnt);
                esp.SendString(ENABLE, cStr, 0, Esp8266::SingleID0); //发送数据	
                debug_printf("发送数据: %s\r\n", cStr);
                Delay_ms(500);
                if (esp.FlagTcpClosed)
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
						//esp.RunStep=1;
                        debug_printf("\r\n正在重连热点和服务器 ......\r\n");
                        while (!esp.JoinAP(ApSsid, ApPwd))
                            ;
                        while (!esp.LinkServer(Esp8266::enumTCP, TcpServer_IP, TcpServer_Port, Esp8266::SingleID0))
                            ;
                        debug_printf("\r\n重连热点和服务器成功\r\n");
                    }
                    while (!esp.UnvarnishSend())
                        ;

                }
                break;
            case 5:
				//重新连接
				esp.RunStep=4;
                break;
			default:
                esp.RunStep = 0;
                break;
        }
    }
    uint OnUsart3Read(ITransport *transport, Buffer &bs, void *para, void *para2)
    {
        //transport->Write(bs);
        debug_printf("收到数据:\r\n");
        bs.Show(true);
        return 0;
    }
    SerialPort sp3(COM3);
    /**
     * @brief  ESP8266 （Sta Tcp Client）透传
     * @param  无
     * @retval 无
     */
    void ESP8266Test()
    {
        esp.SetPin(PG13, PG14);

        sp3.SetBaudRate(115200);
        sp3.Register(OnUsart3Read);
        sp3.Open();

        esp.SetSerialPort(&sp3);
        esp.Init();

        Sys.AddTask(espRoutin, 0, 0, 500, "espRoutin");

    }
#endif
