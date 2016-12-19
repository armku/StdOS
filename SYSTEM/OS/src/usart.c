#include "sys.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;

};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}

void sendch(int ch)
{
    while ((USART1->SR &0X40) == 0){}
    //循环发送,直到发送完毕
    USART1->DR = (uint8_t)ch;
}

//重定义fputc函数
int fputc(int ch, FILE *f)
{
    sendch(ch);
    return ch;
}

//c++的
void _ttywrch(int ch)
{
    sendch(ch);
}

int GetKey(void)
{

    while (!(USART1->SR &USART_FLAG_RXNE))
        ;

    return ((int)(USART1->DR &0x1FF));
}

#if EN_USART1_RX   //如果使能了接收
    //串口1中断服务程序
    //注意,读取USARTx->SR能避免莫名其妙的错误
    uint8_t USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.
    //接收状态
    //bit15，	接收完成标志
    //bit14，	接收到0x0d
    //bit13~0，	接收到的有效字节数目
    uint16_t USART_RX_STA = 0; //接收状态标记

    /*
    当接收到从电脑发过来的数据，把接收到的数据保存在 USART_RX_BUF 中，同时在接收
    状态寄存器（ USART_RX_STA）中计数接收到的有效数据个数，当收到回车（回车的表示由 2
    个字节组成： 0X0D 和 0X0A）的第一个字节 0X0D 时，计数器将不再增加，等待 0X0A 的到来，
    而如果 0X0A 没有来到，则认为这次接收失败，重新开始下一次接收。如果顺利接收到 0X0A，
    则标记 USART_RX_STA 的第 15 位，这样完成一次接收，并等待该位被其他程序清除，从而开
    始下一次的接收，而如果迟迟没有收到 0X0D，那么在接收数据超过 USART_REC_LEN 的时候，
    则会丢弃前面的数据，重新接收。
     */

#endif 
//向串口1发送数据
void sendToUsart1(uint8_t ch)
{
    #if 0
        while ((USART1->SR &0X40) == 0){}
        //循环发送,直到发送完毕
        USART1->DR = (uint8_t)ch;
    #else 
        USART_SendData(USART1, ch);

        /* 等待发送完毕 */
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
    #endif 
}

//向串口2发送数据
void sendToUsart2(uint8_t ch)
{
    USART_SendData(USART2, ch);

    /* 等待发送完毕 */
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET){}
}

//向串口3发送数据
void sendToUsart3(uint8_t ch)
{
    USART_SendData(USART3, ch);

    /* 等待发送完毕 */
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){}
}

//向串口4发送数据
void sendToUsart4(uint8_t ch)
{
    USART_SendData(UART4, ch);

    /* 等待发送完毕 */
    while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET){}
}

//向串口5发送数据
void sendToUsart5(uint8_t ch)
{
    USART_SendData(UART5, ch);

    /* 等待发送完毕 */
    while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET){}
}
