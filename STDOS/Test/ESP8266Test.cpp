#include "Sys.h"
#include "stm32f10x.h"
#include <stdarg.h>

void USARTx_Config(); //��ʼ������1
void ESP8266_Init(); //��ʼ��WiFiģ��ʹ�õĽӿں�����

void ESP8266_StaTcpClient_UnvarnishTest();

void ESP8266TEST()
{
    USARTx_Config(); //��ʼ������1
    //SysTick_Init(); //���� SysTick Ϊ 1ms �ж�һ�� 
    ESP8266_Init(); //��ʼ��WiFiģ��ʹ�õĽӿں�����


    printf("\r\nҰ�� WF-ESP8266 WiFiģ���������\r\n"); //��ӡ����������ʾ��Ϣ


    ESP8266_StaTcpClient_UnvarnishTest();
}
