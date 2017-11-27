#include "Drivers\Esp8266.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "stm32f10x.h" 

#define ESP8266TEST

#ifdef ESP8266TEST
    //#define ApSsid                     "dd-wrt"               //Ҫ���ӵ��ȵ������
    #define ApSsid                       "NETGEAR77"        //Ҫ���ӵ��ȵ������
    #define ApPwd                        "18353217097"        //Ҫ���ӵ��ȵ����Կ
    //#define TcpServer_IP                 "121.42.164.17"      //Ҫ���ӵķ������� IP
    #define TcpServer_IP                 "192.168.0.120"      //Ҫ���ӵķ������� IP
    #define TcpServer_Port               "8000"               //Ҫ���ӵķ������Ķ˿�
	
	#define macESP8266_CH_ENABLE()                 GPIO_SetBits ( GPIOG, GPIO_Pin_13 )

    Esp8266 esp;
	
    /**
     * @brief  ESP8266 ��Sta Tcp Client��͸��
     * @param  ��
     * @retval ��
     */
    void ESP8266Test()
    {
		static int icnt=0;
        esp.Init(); //��ʼ��WiFiģ��ʹ�õĽӿں�����
        printf("\r\nҰ�� WF-ESP8266 WiFiģ���������\r\n"); //��ӡ����������ʾ��Ϣ

        uint8_t ucStatus;
        char cStr[100] = 
        {
            0
        };
        printf("\r\n�������� ESP8266 ......\r\n");
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
        printf("\r\n���� ESP8266 ���\r\n");
        while (1)
        {
            sprintf(cStr, "%d hello world!\r\n",++icnt);
            esp.SendString(ENABLE, cStr, 0, Esp8266::SingleID0); //��������	
			printf("��������: %s\r\n",cStr);
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

                if (strEsp8266_Fram_Record .Length < (RX_BUF_MAX_LEN - 1))
                //Ԥ��1���ֽ�д������
                    strEsp8266_Fram_Record .RxBuf[strEsp8266_Fram_Record .Length++] = ucCh;
            }

            if (USART_GetITStatus(USART3, USART_IT_IDLE) == SET)
            //����֡�������
            {
                strEsp8266_Fram_Record .FlagFinish = 1;

                ucCh = USART_ReceiveData(USART3); //�������������жϱ�־λ(�ȶ�USART_SR��Ȼ���USART_DR)
                esp.FlagTcpClosed = strstr(strEsp8266_Fram_Record .RxBuf, "CLOSED\r\n") ? 1 : 0;
            }
        }
        #ifdef __cplusplus
        }
    #endif 
#endif
