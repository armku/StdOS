#include <stdio.h>
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
    #pragma import(__use_no_semihosting)
    //��׼����Ҫ��֧�ֺ���
    struct __FILE
    {
        int handle;

    };

    FILE __stdout;

    //����_sys_exit()�Ա���ʹ�ð�����ģʽ
    void _sys_exit(int x)
    {
        x = x;
    }

    //c++��
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
