#include <stdio.h>
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
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

    //c++的
    void _ttywrch(int ch)
    {
        FILE *fp;
        fputc(ch, fp);
    }

    int GetKey(void)
    {

        while (!(USART1->SR &USART_FLAG_RXNE))
            ;

        return ((int)(USART1->DR &0x1FF));
    }
#endif
