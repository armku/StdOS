#include "Sys.h"
#include "stm32f10x.h"
#include <stdarg.h>
#include "bsp_esp8266.h"

#define macESP8266_CH_ENABLE()                 GPIO_SetBits ( GPIOG, GPIO_Pin_13 )

#define      macUser_ESP8266_ApSsid                       "NETGEAR77"                //Ҫ���ӵ��ȵ������
#define      macUser_ESP8266_ApPwd                        "18353217097"           //Ҫ���ӵ��ȵ����Կ
#define      macUser_ESP8266_TcpServer_IP                 "121.42.164.17"      //Ҫ���ӵķ������� IP
#define      macUser_ESP8266_TcpServer_Port               "8000"               //Ҫ���ӵķ������Ķ˿�

volatile uint8_t ucTcpClosedFlag = 0;

char cStr[1500] = 
{
    0
};
ESP8266 esp;
void ESP8266TEST()
{    
    esp.ESP8266_Init(); //��ʼ��WiFiģ��ʹ�õĽӿں�����
    printf("\r\nҰ�� WF-ESP8266 WiFiģ���������\r\n"); //��ӡ����������ʾ��Ϣ
    
	uint8_t ucStatus;

    char cStr[100] = 
    {
        0
    };
    printf("\r\n�������� ESP8266 ......\r\n");
    macESP8266_CH_ENABLE();
    esp.ESP8266_AT_Test();
    esp.ESP8266_Net_Mode_Choose(STA);
    while (!esp.ESP8266_JoinAP(macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd))
        ;
    esp.ESP8266_Enable_MultipleId(DISABLE);
    while (!esp.ESP8266_Link_Server(enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0))
        ;    while (!esp.ESP8266_UnvarnishSend())
        ;    printf("\r\n���� ESP8266 ���\r\n");
    while (1)
    {
        sprintf(cStr, "ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n");

        esp.ESP8266_SendString(ENABLE, cStr, 0, Single_ID_0); //��������
        Sys.Sleep(100);
        if (ucTcpClosedFlag)
        //����Ƿ�ʧȥ����
        {
            esp.ESP8266_ExitUnvarnishSend(); //�˳�͸��ģʽ
            do
                ucStatus = esp.ESP8266_Get_LinkStatus();
            //��ȡ����״̬
            while (!ucStatus);
            if (ucStatus == 4)
            //ȷ��ʧȥ���Ӻ�����
            {
                printf("\r\n���������ȵ�ͷ����� ......\r\n");
                while (!esp.ESP8266_JoinAP(macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd))
                    ;
                while (!esp.ESP8266_Link_Server(enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0))
                    ;
                printf("\r\n�����ȵ�ͷ������ɹ�\r\n");
            }
            while (!esp.ESP8266_UnvarnishSend());
        }
    }
}
