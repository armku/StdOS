#include "Sys.h"
#include "stm32f10x.h"
#include <stdarg.h>

void ESP8266_Init(); //��ʼ��WiFiģ��ʹ�õĽӿں�����
void ESP8266_StaTcpClient_UnvarnishTest();
void ESP8266TEST()
{    
    //SysTick_Init(); //���� SysTick Ϊ 1ms �ж�һ�� 
    ESP8266_Init(); //��ʼ��WiFiģ��ʹ�õĽӿں�����


    printf("\r\nҰ�� WF-ESP8266 WiFiģ���������\r\n"); //��ӡ����������ʾ��Ϣ


    ESP8266_StaTcpClient_UnvarnishTest();
}
