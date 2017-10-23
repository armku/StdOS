#include "Sys.h"
#include "stm32f10x.h"
#include <stdarg.h>

void USARTx_Config(); //初始化串口1
void ESP8266_Init(); //初始化WiFi模块使用的接口和外设

void ESP8266_StaTcpClient_UnvarnishTest();

void ESP8266TEST()
{
    USARTx_Config(); //初始化串口1
    //SysTick_Init(); //配置 SysTick 为 1ms 中断一次 
    ESP8266_Init(); //初始化WiFi模块使用的接口和外设


    printf("\r\n野火 WF-ESP8266 WiFi模块测试例程\r\n"); //打印测试例程提示信息


    ESP8266_StaTcpClient_UnvarnishTest();
}
