#include "stm32f10x.h"
#include "bsp_uart_fifo.h"
#include <stdio.h>

Usart_T usart1;

/* 定义每个串口结构体变量 */
UART_T g_tUart1;
#define UART1_TX_BUF_SIZE	1*1024
#define UART1_RX_BUF_SIZE	1*1024
byte g_TxBuf1[UART1_TX_BUF_SIZE]; /* 发送缓冲区 */
byte g_RxBuf1[UART1_RX_BUF_SIZE]; /* 接收缓冲区 */
void UartVarInit(void);

void InitHardUart(void);
void UartSend(UART_T *_pUart, byte *_ucaBuf, ushort _usLen);
byte UartGetChar(UART_T *_pUart, byte *_pByte);
void UartIRQ(UART_T *_pUart);
void ConfigUartNVIC(void);

void FIFO::SetBuf(byte *buf, int len)
{
    if (len >= 0)
    {
        this->Buf = buf;
        this->BufSize = len;
    }
    this->Clear();
}

void FIFO::Push(byte da)
{
    this->Buf[this->Write] = da;
    if (++this->Write >= this->BufSize)
    {
        this->Write = 0;
    }
    if (this->Count < this->BufSize)
    {
        this->Count++;
    }
}

byte FIFO::Pop()
{
    byte ret = this->Buf[this->Read];
    if (++this->Read >= this->BufSize)
    {
        this->Read = 0;
    }
    if (this->Count)
    {
        this->Count--;
    }
    return ret;
}

void FIFO::Clear()
{
    this->Write = 0;
    this->Read = 0;
    this->Count = 0;
}

//缓冲区满
bool FIFO::Full()
{
    return this->Count >= this->BufSize;
}

//缓冲区空
bool FIFO::Empty()
{
    return this->Count == 0;
}

/*
 *********************************************************************************************************
 *	函 数 名: bsp_InitUart
 *	功能说明: 初始化串口硬件，并对全局变量赋初值.
 *	形    参:  无
 *	返 回 值: 无
 *********************************************************************************************************
 */
void Usart_T::bsp_InitUart(void)
{
    UartVarInit();

    InitHardUart();

    ConfigUartNVIC();
}

/*
 *********************************************************************************************************
 *	函 数 名: ComToUart
 *	功能说明: 将COM端口号转换为UART指针
 *	形    参: _ucPort: 端口号(COM1 - COM6)
 *	返 回 值: uart指针
 *********************************************************************************************************
 */
UART_T *ComToUart(COM _ucPort)
{
    if (_ucPort == COM1)
    {
        return  &g_tUart1;
    }
    else
    {
        /* 不做任何处理 */
        return 0;
    }
}

/*
 *********************************************************************************************************
 *	函 数 名: comSendBuf
 *	功能说明: 向串口发送一组数据。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
 *	形    参: _ucPort: 端口号(COM1 - COM6)
 *			  _ucaBuf: 待发送的数据缓冲区
 *			  _usLen : 数据长度
 *	返 回 值: 无
 *********************************************************************************************************
 */
void comSendBuf(COM _ucPort, byte *_ucaBuf, ushort _usLen)
{
    UART_T *pUart;

    pUart = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return ;
    }

    if (pUart->SendBefor != 0)
    {
        pUart->SendBefor(); /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
    }

    UartSend(pUart, _ucaBuf, _usLen);
}

/*
 *********************************************************************************************************
 *	函 数 名: comSendChar
 *	功能说明: 向串口发送1个字节。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
 *	形    参: _ucPort: 端口号(COM1 - COM6)
 *			  _ucByte: 待发送的数据
 *	返 回 值: 无
 *********************************************************************************************************
 */
void comSendChar(COM _ucPort, byte _ucByte)
{
    comSendBuf(_ucPort, &_ucByte, 1);
}

/*
 *********************************************************************************************************
 *	函 数 名: comGetChar
 *	功能说明: 从串口缓冲区读取1字节，非阻塞。无论有无数据均立即返回
 *	形    参: _ucPort: 端口号(COM1 - COM6)
 *			  _pByte: 接收到的数据存放在这个地址
 *	返 回 值: 0 表示无数据, 1 表示读取到有效字节
 *********************************************************************************************************
 */
byte Usart_T::comGetChar(COM _ucPort, byte *_pByte)
{
    UART_T *pUart;

    pUart = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return 0;
    }

    return UartGetChar(pUart, _pByte);
}

/*
 *********************************************************************************************************
 *	函 数 名: comClearTxFifo
 *	功能说明: 清零串口发送缓冲区
 *	形    参: _ucPort: 端口号(COM1 - COM6)
 *	返 回 值: 无
 *********************************************************************************************************
 */
void Usart_T::comClearTxFifo(COM _ucPort)
{
    UART_T *pUart;

    pUart = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return ;
    }

    pUart->tx.Clear();
}

/*
 *********************************************************************************************************
 *	函 数 名: comClearRxFifo
 *	功能说明: 清零串口接收缓冲区
 *	形    参: _ucPort: 端口号(COM1 - COM6)
 *	返 回 值: 无
 *********************************************************************************************************
 */
void Usart_T::comClearRxFifo(COM _ucPort)
{
    UART_T *pUart;

    pUart = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return ;
    }

    pUart->rx.Clear();
}
/*
 *********************************************************************************************************
 *	函 数 名: RS485_ReciveNew
 *	功能说明: 接收到新的数据
 *	形    参: _byte 接收到的新数据
 *	返 回 值: 无
 *********************************************************************************************************
 */
extern void MODBUS_ReciveNew(byte _byte);
void RS485_ReciveNew(byte _byte)
{
    //MODBUS_ReciveNew(_byte);
}

/*
 *********************************************************************************************************
 *	函 数 名: UartVarInit
 *	功能说明: 初始化串口相关的变量
 *	形    参: 无
 *	返 回 值: 无
 *********************************************************************************************************
 */
void UartVarInit(void)
{
    g_tUart1.uart = USART1; /* STM32 串口设备 */
    g_tUart1.tx.SetBuf(g_TxBuf1, UART1_TX_BUF_SIZE);
    g_tUart1.rx.SetBuf(g_RxBuf1, UART1_RX_BUF_SIZE);
    g_tUart1.SendBefor = 0; /* 发送数据前的回调函数 */
    g_tUart1.SendOver = 0; /* 发送完毕后的回调函数 */
    g_tUart1.ReciveNew = 0; /* 接收到新数据后的回调函数 */
}

/*
 *********************************************************************************************************
 *	函 数 名: InitHardUart
 *	功能说明: 配置串口的硬件参数（波特率，数据位，停止位，起始位，校验位，中断使能）适合于STM32-F4开发板
 *	形    参: 无
 *	返 回 值: 无
 *********************************************************************************************************
 */
void InitHardUart(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* 串口1 TX = PA9   RX = PA10 或 TX = PB6   RX = PB7*/

    /* 第1步：打开GPIO和USART部件的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    /* 第2步：将USART Tx的GPIO配置为推挽复用模式 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 第3步：将USART Rx的GPIO配置为浮空输入模式
    由于CPU复位后，GPIO缺省都是浮空输入模式，因此下面这个步骤不是必须的
    但是，我还是建议加上便于阅读，并且防止其它地方修改了这个口线的设置参数
     */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 第4步： 配置串口硬件参数 */
    USART_InitStructure.USART_BaudRate = 256000; /* 波特率 */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); /* 使能接收中断 */
    /*
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    注意: 不要在此处打开发送中断
    发送中断使能在SendUart()函数打开
     */
    USART_Cmd(USART1, ENABLE); /* 使能串口 */

    /* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
    如下语句解决第1个字节无法正确发送出去的问题 */
    USART_ClearFlag(USART1, USART_FLAG_TC); /* 清发送完成标志，Transmission Complete flag */
}

/*
 *********************************************************************************************************
 *	函 数 名: ConfigUartNVIC
 *	功能说明: 配置串口硬件中断.
 *	形    参:  无
 *	返 回 值: 无
 *********************************************************************************************************
 */
void ConfigUartNVIC(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure the NVIC Preemption Priority Bits */
    /*	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  --- 在 bsp.c 中 bsp_Init() 中配置中断优先级组 */
    /* 使能串口1中断 */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 *********************************************************************************************************
 *	函 数 名: UartSend
 *	功能说明: 填写数据到UART发送缓冲区,并启动发送中断。中断处理函数发送完毕后，自动关闭发送中断
 *	形    参:  无
 *	返 回 值: 无
 *********************************************************************************************************
 */
void UartSend(UART_T *_pUart, byte *_ucaBuf, ushort _usLen)
{
    #if 1	
        for (int i = 0; i < _usLen; i++)
        {
            //DISABLE_INT();
            while (1)
            {
                if (!_pUart->tx.Full())
                {
                    break;
                }
            }
            //ENABLE_INT();
            _pUart->tx.Push(_ucaBuf[i]);
        }

        USART_ITConfig(_pUart->uart, USART_IT_TXE, ENABLE);
    #else 
        USART_SendData(_pUart->uart, _ucaBuf[0]);
        while (USART_GetFlagStatus(_pUart->uart, USART_FLAG_TXE) == RESET)
            ;
    #endif 
}

/*
 *********************************************************************************************************
 *	函 数 名: UartGetChar
 *	功能说明: 从串口接收缓冲区读取1字节数据 （用于主程序调用）
 *	形    参: _pUart : 串口设备
 *			  _pByte : 存放读取数据的指针
 *	返 回 值: 0 表示无数据  1表示读取到数据
 *********************************************************************************************************
 */
byte UartGetChar(UART_T *_pUart, byte *_pByte)
{
    if (_pUart->rx.Empty())
    {
        return 0;
    }
    else
    {
        *_pByte = _pUart->rx.Pop(); /* 从串口接收FIFO取1个数据 */
        return 1;
    }
}

/*
 *********************************************************************************************************
 *	函 数 名: UartIRQ
 *	功能说明: 供中断服务程序调用，通用串口中断处理函数
 *	形    参: _pUart : 串口设备
 *	返 回 值: 无
 *********************************************************************************************************
 */
void UartIRQ(UART_T *_pUart)
{	
    /* 处理接收中断  */
    if (USART_GetITStatus(_pUart->uart, USART_IT_RXNE) != RESET)
    {
        /* 从串口接收数据寄存器读取数据存放到接收FIFO */
        byte ch;

        ch = USART_ReceiveData(_pUart->uart);
        _pUart->rx.Push(ch);
        /* 回调函数,通知应用程序收到新数据,一般是发送1个消息或者设置一个标记 */
        {
            if (_pUart->ReciveNew)
            {
                _pUart->ReciveNew(ch);
            }
        }
    }

    /* 处理发送缓冲区空中断 */
    if (USART_GetITStatus(_pUart->uart, USART_IT_TXE) != RESET)
    {
        //if (_pUart->usTxRead == _pUart->usTxWrite)
        if (_pUart->tx.Empty())
        {
            /* 发送缓冲区的数据已取完时， 禁止发送缓冲区空中断 （注意：此时最后1个数据还未真正发送完毕）*/
            USART_ITConfig(_pUart->uart, USART_IT_TXE, DISABLE);

            /* 使能数据发送完毕中断 */
            USART_ITConfig(_pUart->uart, USART_IT_TC, ENABLE);
        }
        else
        {
            /* 从发送FIFO取1个字节写入串口发送数据寄存器 */
            USART_SendData(_pUart->uart, _pUart->tx.Pop());
        }

    }
    /* 数据bit位全部发送完毕的中断 */
    else if (USART_GetITStatus(_pUart->uart, USART_IT_TC) != RESET)
    {
        if (_pUart->tx.Empty())
        {
            /* 如果发送FIFO的数据全部发送完毕，禁止数据发送完毕中断 */
            USART_ITConfig(_pUart->uart, USART_IT_TC, DISABLE);

            /* 回调函数, 一般用来处理RS485通信，将RS485芯片设置为接收模式，避免抢占总线 */
            if (_pUart->SendOver)
            {
                _pUart->SendOver();
            }
        }
        else
        {
            /* 正常情况下，不会进入此分支 */
            /* 如果发送FIFO的数据还未完毕，则从发送FIFO取1个数据写入发送数据寄存器 */
            USART_SendData(_pUart->uart, _pUart->tx.Pop());
        }
    }
}

#ifdef __cplusplus
    extern "C"
    {
    #endif 
    /*
     *********************************************************************************************************
     *	函 数 名: USART1_IRQHandler  USART2_IRQHandler USART3_IRQHandler UART4_IRQHandler UART5_IRQHandler
     *	功能说明: USART中断服务程序
     *	形    参: 无
     *	返 回 值: 无
     *********************************************************************************************************
     */
    void USART1_IRQHandler()
    {
        UartIRQ(&g_tUart1);
    }
    /*
     *********************************************************************************************************
     *	函 数 名: fputc
     *	功能说明: 重定义putc函数，这样可以使用printf函数从串口1打印输出
     *	形    参: 无
     *	返 回 值: 无
     *********************************************************************************************************
     */
    int fputc(int ch, FILE *f)
    {
        comSendChar(COM1, ch);
        return ch;
    }

    /*
     *********************************************************************************************************
     *	函 数 名: fgetc
     *	功能说明: 重定义getc函数，这样可以使用getchar函数从串口1输入数据
     *	形    参: 无
     *	返 回 值: 无
     *********************************************************************************************************
     */
    int fgetc(FILE *f)
    {
        /* 从串口接收FIFO中取1个数据, 只有取到数据才返回 */
        byte ucData;

        while (usart1.comGetChar(COM1, &ucData) == 0)
            ;
        return ucData;
    }
    #ifdef __cplusplus
    }
#endif 
#ifdef DEBUG
    void FifoTest()
    {
		ConfigUartNVIC();
        usart1.bsp_InitUart(); /* 初始化串口驱动 */
    }

#endif
