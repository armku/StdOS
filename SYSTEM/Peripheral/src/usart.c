#include "stm32f10x.h"
#include "usart.h"
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
    USART1->DR = (byte)ch;
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
    byte USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.
    //接收状态
    //bit15，	接收完成标志
    //bit14，	接收到0x0d
    //bit13~0，	接收到的有效字节数目
    ushort USART_RX_STA = 0; //接收状态标记

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
