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
    #define TcpServer_IP                 "121.42.164.17"      //Ҫ���ӵķ������� IP
    //#define TcpServer_IP                 "192.168.0.120"      //Ҫ���ӵķ������� IP
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

        switch (esp.RunStep)
        {
            case 0:
                debug_printf("\r\n���ڲ������� ESP8266 ......\r\n");
                esp.Test();
				esp.NetModeChoose(Esp8266::STA);
                esp.RunStep++;
                break;
			case 1:				
				debug_printf("\r\n���������ȵ�ͷ����� ......\r\n");
                while (!esp.JoinAP(ApSsid, ApPwd))
                    ;
                esp.EnableMultipleId(DISABLE);
                esp.RunStep++;
                break;
            case 2:
				while (!esp.LinkServer(Esp8266::enumTCP, TcpServer_IP, TcpServer_Port, Esp8266::SingleID0))
                    ;
				debug_printf("\r\n�����ȵ�ͷ������ɹ�\r\n");
                esp.RunStep++;
                break;
            case 3:
				while (!esp.UnvarnishSend())
                    ;
                debug_printf("\r\n���� ESP8266 ���\r\n");
                esp.RunStep++;
                break;
            case 4:
                sprintf(cStr, "%d hello world!\r\n", ++icnt);
                esp.SendString(ENABLE, cStr, 0, Esp8266::SingleID0); //��������	
                debug_printf("��������: %s\r\n", cStr);
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
						//esp.RunStep=1;
                        debug_printf("\r\n���������ȵ�ͷ����� ......\r\n");
                        while (!esp.JoinAP(ApSsid, ApPwd))
                            ;
                        while (!esp.LinkServer(Esp8266::enumTCP, TcpServer_IP, TcpServer_Port, Esp8266::SingleID0))
                            ;
                        debug_printf("\r\n�����ȵ�ͷ������ɹ�\r\n");
                    }
                    while (!esp.UnvarnishSend())
                        ;

                }
                break;
            case 5:
				//��������
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
        debug_printf("�յ�����:\r\n");
        bs.Show(true);
        return 0;
    }
    SerialPort sp3(COM3);
    /**
     * @brief  ESP8266 ��Sta Tcp Client��͸��
     * @param  ��
     * @retval ��
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
