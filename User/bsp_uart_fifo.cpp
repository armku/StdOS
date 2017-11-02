#include "stm32f10x.h"
#include "bsp_uart_fifo.h"
#include <stdio.h>

#define UART1_TX_BUF_SIZE	1*1024
#define UART1_RX_BUF_SIZE	1*1024

Usart_T usart1;

/* ����ÿ�����ڽṹ����� */
UART_T g_tUart1;
uint8_t g_TxBuf1[UART1_TX_BUF_SIZE]; /* ���ͻ����� */
uint8_t g_RxBuf1[UART1_RX_BUF_SIZE]; /* ���ջ����� */
void UartVarInit(void);

void InitHardUart(void);
void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen);
uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte);
void UartIRQ(UART_T *_pUart);
void ConfigUartNVIC(void);

void FIFO::SetBuf(uint8_t *buf, int len)
{
    if (len >= 0)
    {
        this->Buf = buf;
        this->BufSize = len;
    }
    this->Clear();
}

void FIFO::Push(uint8_t da)
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

uint8_t FIFO::Pop()
{
    uint8_t ret = this->Buf[this->Read];
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

//��������
bool FIFO::Full()
{
    return this->Count >= this->BufSize;
}

//��������
bool FIFO::Empty()
{
    return this->Count == 0;
}

/*
 *********************************************************************************************************
 *	�� �� ��: bsp_InitUart
 *	����˵��: ��ʼ������Ӳ��������ȫ�ֱ�������ֵ.
 *	��    ��:  ��
 *	�� �� ֵ: ��
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
 *	�� �� ��: ComToUart
 *	����˵��: ��COM�˿ں�ת��ΪUARTָ��
 *	��    ��: _ucPort: �˿ں�(COM1 - COM6)
 *	�� �� ֵ: uartָ��
 *********************************************************************************************************
 */
UART_T *ComToUart(COM_PORT_E _ucPort)
{
    if (_ucPort == COM1)
    {
        return  &g_tUart1;
    }
    else
    {
        /* �����κδ��� */
        return 0;
    }
}

/*
 *********************************************************************************************************
 *	�� �� ��: comSendBuf
 *	����˵��: �򴮿ڷ���һ�����ݡ����ݷŵ����ͻ��������������أ����жϷ�������ں�̨��ɷ���
 *	��    ��: _ucPort: �˿ں�(COM1 - COM6)
 *			  _ucaBuf: �����͵����ݻ�����
 *			  _usLen : ���ݳ���
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen)
{
    UART_T *pUart;

    pUart = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return ;
    }

    if (pUart->SendBefor != 0)
    {
        pUart->SendBefor(); /* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
    }

    UartSend(pUart, _ucaBuf, _usLen);
}

/*
 *********************************************************************************************************
 *	�� �� ��: comSendChar
 *	����˵��: �򴮿ڷ���1���ֽڡ����ݷŵ����ͻ��������������أ����жϷ�������ں�̨��ɷ���
 *	��    ��: _ucPort: �˿ں�(COM1 - COM6)
 *			  _ucByte: �����͵�����
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte)
{
    comSendBuf(_ucPort, &_ucByte, 1);
}

/*
 *********************************************************************************************************
 *	�� �� ��: comGetChar
 *	����˵��: �Ӵ��ڻ�������ȡ1�ֽڣ��������������������ݾ���������
 *	��    ��: _ucPort: �˿ں�(COM1 - COM6)
 *			  _pByte: ���յ������ݴ���������ַ
 *	�� �� ֵ: 0 ��ʾ������, 1 ��ʾ��ȡ����Ч�ֽ�
 *********************************************************************************************************
 */
uint8_t Usart_T::comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte)
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
 *	�� �� ��: comClearTxFifo
 *	����˵��: ���㴮�ڷ��ͻ�����
 *	��    ��: _ucPort: �˿ں�(COM1 - COM6)
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void Usart_T::comClearTxFifo(COM_PORT_E _ucPort)
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
 *	�� �� ��: comClearRxFifo
 *	����˵��: ���㴮�ڽ��ջ�����
 *	��    ��: _ucPort: �˿ں�(COM1 - COM6)
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void Usart_T::comClearRxFifo(COM_PORT_E _ucPort)
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
 *	�� �� ��: bsp_SetUart1Baud
 *	����˵��: �޸�UART1������
 *	��    ��: ��
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void Usart_T::bsp_SetUart1Baud(uint32_t _baud)
{
    USART_InitTypeDef USART_InitStructure;

    /* ��2���� ���ô���Ӳ������ */
    USART_InitStructure.USART_BaudRate = _baud; /* ������ */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
}

/*
 *********************************************************************************************************
 *	�� �� ��: RS485_ReciveNew
 *	����˵��: ���յ��µ�����
 *	��    ��: _byte ���յ���������
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
extern void MODBUS_ReciveNew(uint8_t _byte);
void RS485_ReciveNew(uint8_t _byte)
{
    //MODBUS_ReciveNew(_byte);
}

/*
 *********************************************************************************************************
 *	�� �� ��: UartVarInit
 *	����˵��: ��ʼ��������صı���
 *	��    ��: ��
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void UartVarInit(void)
{
    g_tUart1.uart = USART1; /* STM32 �����豸 */
    g_tUart1.tx.SetBuf(g_TxBuf1, UART1_TX_BUF_SIZE);
    g_tUart1.rx.SetBuf(g_RxBuf1, UART1_RX_BUF_SIZE);
    g_tUart1.SendBefor = 0; /* ��������ǰ�Ļص����� */
    g_tUart1.SendOver = 0; /* ������Ϻ�Ļص����� */
    g_tUart1.ReciveNew = 0; /* ���յ������ݺ�Ļص����� */
}

/*
 *********************************************************************************************************
 *	�� �� ��: InitHardUart
 *	����˵��: ���ô��ڵ�Ӳ�������������ʣ�����λ��ֹͣλ����ʼλ��У��λ���ж�ʹ�ܣ��ʺ���STM32-F4������
 *	��    ��: ��
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void InitHardUart(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* ����1 TX = PA9   RX = PA10 �� TX = PB6   RX = PB7*/

    /* ��1������GPIO��USART������ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    /* ��2������USART Tx��GPIO����Ϊ���츴��ģʽ */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ��3������USART Rx��GPIO����Ϊ��������ģʽ
    ����CPU��λ��GPIOȱʡ���Ǹ�������ģʽ���������������費�Ǳ����
    ���ǣ��һ��ǽ�����ϱ����Ķ������ҷ�ֹ�����ط��޸���������ߵ����ò���
     */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ��4���� ���ô���Ӳ������ */
    USART_InitStructure.USART_BaudRate = 256000; /* ������ */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); /* ʹ�ܽ����ж� */
    /*
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    ע��: ��Ҫ�ڴ˴��򿪷����ж�
    �����ж�ʹ����SendUart()������
     */
    USART_Cmd(USART1, ENABLE); /* ʹ�ܴ��� */

    /* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
    �����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
    USART_ClearFlag(USART1, USART_FLAG_TC); /* �巢����ɱ�־��Transmission Complete flag */
}

/*
 *********************************************************************************************************
 *	�� �� ��: ConfigUartNVIC
 *	����˵��: ���ô���Ӳ���ж�.
 *	��    ��:  ��
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void ConfigUartNVIC(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure the NVIC Preemption Priority Bits */
    /*	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  --- �� bsp.c �� bsp_Init() �������ж����ȼ��� */
    /* ʹ�ܴ���1�ж� */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 *********************************************************************************************************
 *	�� �� ��: UartSend
 *	����˵��: ��д���ݵ�UART���ͻ�����,�����������жϡ��жϴ�����������Ϻ��Զ��رշ����ж�
 *	��    ��:  ��
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen)
{
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
}

/*
 *********************************************************************************************************
 *	�� �� ��: UartGetChar
 *	����˵��: �Ӵ��ڽ��ջ�������ȡ1�ֽ����� ��������������ã�
 *	��    ��: _pUart : �����豸
 *			  _pByte : ��Ŷ�ȡ���ݵ�ָ��
 *	�� �� ֵ: 0 ��ʾ������  1��ʾ��ȡ������
 *********************************************************************************************************
 */
uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte)
{
    if (_pUart->rx.Empty())
    {
        return 0;
    }
    else
    {
        *_pByte = _pUart->rx.Pop(); /* �Ӵ��ڽ���FIFOȡ1������ */
        return 1;
    }
}

/*
 *********************************************************************************************************
 *	�� �� ��: UartIRQ
 *	����˵��: ���жϷ��������ã�ͨ�ô����жϴ�����
 *	��    ��: _pUart : �����豸
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void UartIRQ(UART_T *_pUart)
{
    /* ��������ж�  */
    if (USART_GetITStatus(_pUart->uart, USART_IT_RXNE) != RESET)
    {
        /* �Ӵ��ڽ������ݼĴ�����ȡ���ݴ�ŵ�����FIFO */
        uint8_t ch;

        ch = USART_ReceiveData(_pUart->uart);
        _pUart->rx.Push(ch);
        /* �ص�����,֪ͨӦ�ó����յ�������,һ���Ƿ���1����Ϣ��������һ����� */
        {
            if (_pUart->ReciveNew)
            {
                _pUart->ReciveNew(ch);
            }
        }
    }

    /* �����ͻ��������ж� */
    if (USART_GetITStatus(_pUart->uart, USART_IT_TXE) != RESET)
    {
        //if (_pUart->usTxRead == _pUart->usTxWrite)
        if (_pUart->tx.Empty())
        {
            /* ���ͻ�������������ȡ��ʱ�� ��ֹ���ͻ��������ж� ��ע�⣺��ʱ���1�����ݻ�δ����������ϣ�*/
            USART_ITConfig(_pUart->uart, USART_IT_TXE, DISABLE);

            /* ʹ�����ݷ�������ж� */
            USART_ITConfig(_pUart->uart, USART_IT_TC, ENABLE);
        }
        else
        {
            /* �ӷ���FIFOȡ1���ֽ�д�봮�ڷ������ݼĴ��� */
            USART_SendData(_pUart->uart, _pUart->tx.Pop());
        }

    }
    /* ����bitλȫ��������ϵ��ж� */
    else if (USART_GetITStatus(_pUart->uart, USART_IT_TC) != RESET)
    {
        if (_pUart->tx.Empty())
        {
            /* �������FIFO������ȫ��������ϣ���ֹ���ݷ�������ж� */
            USART_ITConfig(_pUart->uart, USART_IT_TC, DISABLE);

            /* �ص�����, һ����������RS485ͨ�ţ���RS485оƬ����Ϊ����ģʽ��������ռ���� */
            if (_pUart->SendOver)
            {
                _pUart->SendOver();
            }
        }
        else
        {
            /* ��������£��������˷�֧ */
            /* �������FIFO�����ݻ�δ��ϣ���ӷ���FIFOȡ1������д�뷢�����ݼĴ��� */
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
     *	�� �� ��: USART1_IRQHandler  USART2_IRQHandler USART3_IRQHandler UART4_IRQHandler UART5_IRQHandler
     *	����˵��: USART�жϷ������
     *	��    ��: ��
     *	�� �� ֵ: ��
     *********************************************************************************************************
     */
    void USART1_IRQHandler(void)
    {
        UartIRQ(&g_tUart1);
    }
    /*
     *********************************************************************************************************
     *	�� �� ��: fputc
     *	����˵��: �ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
     *	��    ��: ��
     *	�� �� ֵ: ��
     *********************************************************************************************************
     */
    int fputc(int ch, FILE *f)
    {
        comSendChar(COM1, ch);
        return ch;
    }

    /*
     *********************************************************************************************************
     *	�� �� ��: fgetc
     *	����˵��: �ض���getc��������������ʹ��getchar�����Ӵ���1��������
     *	��    ��: ��
     *	�� �� ֵ: ��
     *********************************************************************************************************
     */
    int fgetc(FILE *f)
    {
        /* �Ӵ��ڽ���FIFO��ȡ1������, ֻ��ȡ�����ݲŷ��� */
        uint8_t ucData;

        while (usart1.comGetChar(COM1, &ucData) == 0)
            ;
        return ucData;
    }
    #ifdef __cplusplus
    }
#endif
