#include <stdio.h>
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 0
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
#if 0
    #pragma import(__use_no_semihosting_swi)  
    #pragma import(_main_redirection)  


    //const char __stdin_name[150];  
    //const char __stdout_name[150];  
    //const char __stderr_name[150];  
    typedef int FILEHANDLE;

    //重写标准库函数，这时printf、fopen、fclose等文件操作函数运行时就会调用你的重写函数，这些重写函数只是几个简单的例子，并没有重写所有的文件操作函数  
    void _sys_exit(int status)
    {
        while (1)
            ;
    }
    FILEHANDLE _sys_open(const char *name, int openmode)
    {
        return 0;
    }

    int _sys_close(FILEHANDLE fh)
    {
        return 0;
    }

    int _sys_write(FILEHANDLE fh, const unsigned char *buf, unsigned len, int mode)
    {
        return 0;
    }

    int _sys_read(FILEHANDLE fh, unsigned char *buf, unsigned len, int mode)
    {
        return 0;
    }

    int _sys_istty(FILEHANDLE fh)
    {
        return 0;
    }

    int _sys_seek(FILEHANDLE fh, long pos)
    {
        return 0;
    }

    int _sys_ensure(FILEHANDLE fh)
    {
        return 0;
    }

    long _sys_flen(FILEHANDLE fh)
    {
        return 0;
    }

    int _sys_tmpnam(char *name, int fileno, unsigned maxlength)
    {
        return 0;
    }

    //void _ttywrch(int ch)  
    //{
    //    return 0;  
    //}  
    //int remove(const char *filename)  
    //{  
    //    return 0;  
    //}  
    //  
    char *_sys_command_string(char *cmd, int len)
    {
        return 0;
    }

    //clock_t clock(void)  
    //{  
    //    return 0;  
    //}  	
#endif
