#include "Drivers\Esp8266.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "stm32f10x.h" 
#include "SerialPort.h"
#include "Sys.h"

#define ESP8266TEST

#ifdef ESP8266TEST
    //#define ApSsid                     "dd-wrt"               //Ҫ���ӵ��ȵ������
    #define ApSsid                       "NETGEAR77"        //Ҫ���ӵ��ȵ������
    #define ApPwd                        "18353217097"        //Ҫ���ӵ��ȵ����Կ
    //#define TcpServer_IP                 "121.42.164.17"      //Ҫ���ӵķ������� IP
    #define TcpServer_IP                 "192.168.0.120"      //Ҫ���ӵķ������� IP
    #define TcpServer_Port               "8000"               //Ҫ���ӵķ������Ķ˿�

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
        esp.SendString(ENABLE, cStr, 0, Esp8266::SingleID0); //��������	
        printf("��������: %s\r\n", cStr);
        Delay_ms(500);
        if (esp.FlagTcpClosed)
        //����Ƿ�ʧȥ����
        {
            esp.ExitUnvarnishSend(); //�˳�͸��ģʽ			
            do
                ucStatus = esp.GetLinkStatus();
            //��ȡ����״̬
            while (!ucStatus);
            if (ucStatus == 4)
            //ȷ��ʧȥ���Ӻ�����
            {
                printf("\r\n���������ȵ�ͷ����� ......\r\n");
                while (!esp.JoinAP(ApSsid, ApPwd))
                    ;
                while (!esp.LinkServer(Esp8266::enumTCP, TcpServer_IP, TcpServer_Port, Esp8266::SingleID0))
                    ;
                printf("\r\n�����ȵ�ͷ������ɹ�\r\n");
            }
            while (!esp.UnvarnishSend())
                ;

        }
    }
	uint OnUsart3Read(ITransport *transport, Buffer &bs, void *para, void *para2)
	{
		//transport->Write(bs);
		debug_printf("�յ�����:\r\n");
		bs.Show(true);
		return 0;
	}
    void USARTConfig();
	SerialPort sp3(COM3);
    /**
     * @brief  ESP8266 ��Sta Tcp Client��͸��
     * @param  ��
     * @retval ��
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
        esp.Init(); //��ʼ��WiFiģ��ʹ�õĽӿں�����
        printf("\r\nҰ�� WF-ESP8266 WiFiģ���������\r\n"); //��ӡ����������ʾ��Ϣ

        uint8_t ucStatus;
        char cStr[100] = 
        {
            0
        };
        printf("\r\n�������� ESP8266 ......\r\n");

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
        printf("\r\n���� ESP8266 ���\r\n");
        Sys.AddTask(espRoutin, 0, 0, 500, "espRoutin");

    }

    /**
     * @brief  ��ʼ��ESP8266�õ��� USART
     * @param  ��
     * @retval ��
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


        /* �ж����� */
        USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //ʹ�ܴ��ڽ����ж� 
        USART_ITConfig(USART3, USART_IT_IDLE, ENABLE); //ʹ�ܴ������߿����ж� 	

        USART_Cmd(USART3, ENABLE);
    }
#endif
