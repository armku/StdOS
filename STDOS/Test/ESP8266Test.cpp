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
    //#define TcpServer_IP                 "121.42.164.17"      //要连接的服务器的 IP
    #define TcpServer_IP                 "192.168.0.120"      //要连接的服务器的 IP
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
        sprintf(cStr, "%d hello world!\r\n", ++icnt);
        esp.SendString(ENABLE, cStr, 0, Esp8266::SingleID0); //发送数据	
        printf("发送数据: %s\r\n", cStr);
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
	uint OnUsart3Read(ITransport *transport, Buffer &bs, void *para, void *para2)
	{
		//transport->Write(bs);
		debug_printf("收到数据:\r\n");
		bs.Show(true);
		return 0;
	}
    void USARTConfig();
	SerialPort sp3(COM3);
    /**
     * @brief  ESP8266 （Sta Tcp Client）透传
     * @param  无
     * @retval 无
     */
    void ESP8266Test()
    {
        static int icnt = 0;
        esp.SetPin(PG13, PG14);
        USARTConfig();
		
		sp3.SetBaudRate(115200);
		sp3.Register(OnUsart3Read);
		sp3.Open();
		
		esp.SetSerialPort(&sp3);
        esp.Init(); //初始化WiFi模块使用的接口和外设
        printf("\r\n野火 WF-ESP8266 WiFi模块测试例程\r\n"); //打印测试例程提示信息

        uint8_t ucStatus;
        char cStr[100] = 
        {
            0
        };
        printf("\r\n正在配置 ESP8266 ......\r\n");

        esp.ChipEnable();
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
        Sys.AddTask(espRoutin, 0, 0, 500, "espRoutin");

    }

    /**
     * @brief  初始化ESP8266用到的 USART
     * @param  无
     * @retval 无
     */
    void USARTConfig()
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;


        /* config USART clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

        /* USART GPIO config */
        /* Configure USART Tx as alternate function push-pull */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        /* Configure USART Rx as input floating */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        /* USART1 mode config */
        USART_InitStructure.USART_BaudRate = 115200;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        USART_Init(USART3, &USART_InitStructure);


        /* 中断配置 */
        USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //使能串口接收中断 
        USART_ITConfig(USART3, USART_IT_IDLE, ENABLE); //使能串口总线空闲中断 	

        USART_Cmd(USART3, ENABLE);
    }
#endif
