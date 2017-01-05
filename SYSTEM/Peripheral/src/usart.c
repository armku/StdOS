#include <stdio.h>
#include "stm32f10x.h"
#include "Type.h"
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
