#include "stm32f10x.h"
#include "bsp_uart_fifo.h"
#include <stdio.h>

#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */

#define UART1_TX_BUF_SIZE	1*1024
#define UART1_RX_BUF_SIZE	1*1024

UART_T g_tUart1;
uint8_t g_TxBuf1[UART1_TX_BUF_SIZE]; /* ���ͻ����� */
uint8_t g_RxBuf1[UART1_RX_BUF_SIZE]; /* ���ջ����� */

void InitHardUart(void);
void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, int _usLen);
uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte);
void UartIRQ(UART_T *_pUart);
void ConfigUartNVIC(void);

void FIFO::SetBuf(uint8_t *buf, int len)
{
    if (len >= 0)
    {
        this->pBuf = buf;
        this->BufSize = len;
    }
    this->Clear();
}

void FIFO::Clear()
{
    this->Write = 0;
    this->Read = 0;
    this->Count = 0;
}

int FIFO::Push(uint8_t da)
{
    this->pBuf[this->Write] = da;
    if (++this->Write >= this->BufSize)
    {
        this->Write = 0;
    }
    //if (this->Count < this->BufSize)
    {
        this->Count++;
        return 0;
    }
    //else
    {
        return  - 1;
    }
}

int FIFO::Pop(uint8_t *da)
{
    *da = this->pBuf[this->Read];
    if (++this->Read >= this->BufSize)
    {
        this->Read = 0;
    }
    if (this->Count)
    {
        this->Count--;
        return 0;
    }
    else
    {
        return  - 1;
    }
}

bool FIFO::BufIsEmpty()
{
    return this->Count == 0;
}

bool FIFO::BufIsFull()
{
    return this->Count >= this->BufSize;
}

/*
 *********************************************************************************************************
 *	�� �� ��: bsp_InitUart
 *	����˵��: ��ʼ������Ӳ��������ȫ�ֱ�������ֵ.
 *	��    ��:  ��
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void bsp_InitUart(void)
{
    g_tUart1.uart = USART1; /* STM32 �����豸 */
    g_tUart1.tx.SetBuf(g_TxBuf1, UART1_TX_BUF_SIZE);
    g_tUart1.rx.SetBuf(g_RxBuf1, UART1_RX_BUF_SIZE);
    g_tUart1.tx.Clear();
    g_tUart1.rx.Clear();
    g_tUart1.ReciveNew = 0; /* ���յ������ݺ�Ļص����� */

    InitHardUart(); /* ���ô��ڵ�Ӳ������(�����ʵ�) */

    ConfigUartNVIC(); /* ���ô����ж� */
}

/*
 *********************************************************************************************************
 *	�� �� ��: ComToUart
 *	����˵��: ��COM�˿ں�ת��ΪUARTָ��
 *	��    ��: _ucPort: �˿ں�(COM1 - COM6)
 *	�� �� ֵ: uartָ��
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
void comSendBuf(COM _ucPort, uint8_t *_ucaBuf, int _usLen)
{
    UART_T *pUart;

    pUart = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return ;
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
void comSendChar(COM _ucPort, uint8_t _ucByte)
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
uint8_t comGetChar(COM _ucPort, uint8_t *_pByte)
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
void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, int _usLen)
{
    uint16_t i;

    for (i = 0; i < _usLen; i++)
    {
        /* ������ͻ������Ѿ����ˣ���ȴ��������� */
        #if 0
        #else 
            /* �� _pUart->usTxBufSize == 1 ʱ, ����ĺ���������(������) */
            while (_pUart->tx.BufIsFull())
                ;
        #endif 

        /* �����������뷢�ͻ����� */
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
    #if 1
        uint16_t usCount;

        /* usRxWrite �������жϺ����б���д���������ȡ�ñ���ʱ����������ٽ������� */
        DISABLE_INT();
        usCount = _pUart->rx.Count;
        ENABLE_INT();

        /* �������д������ͬ���򷵻�0 */
        //if (_pUart->usRxRead == usRxWrite)
        if (usCount == 0)
        /* �Ѿ�û������ */
        {
            return 0;
        }
        else
        {
            #if 1
                *_pByte = _pUart->rx.pBuf[_pUart->rx.Read]; /* �Ӵ��ڽ���FIFOȡ1������ */

                /* ��дFIFO������ */
                DISABLE_INT();
                if (++_pUart->rx.Read >= _pUart->rx.BufSize)
                {
                    _pUart->rx.Read = 0;
                }
                _pUart->rx.Count--;
                ENABLE_INT();
            #else 
                _pUart->rx.Pop(_pByte);
            #endif 
            return 1;
        }
    #else 
        if (_pUart->rx.BufIsEmpty())
            return 0;
        _pUart->rx.Pop(_pByte);
        return 1;
    #endif 
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
        //if (_pUart->usRxWrite == _pUart->usRxRead)
        //if (_pUart->usRxCount == 1)
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
        if (_pUart->tx.BufIsEmpty())
        {
            /* ���ͻ�������������ȡ��ʱ�� ��ֹ���ͻ��������ж� ��ע�⣺��ʱ���1�����ݻ�δ����������ϣ�*/
            USART_ITConfig(_pUart->uart, USART_IT_TXE, DISABLE);

            /* ʹ�����ݷ�������ж� */
            USART_ITConfig(_pUart->uart, USART_IT_TC, ENABLE);
        }
        else
        {
            /* �ӷ���FIFOȡ1���ֽ�д�봮�ڷ������ݼĴ��� */
            uint8_t ch;
            if (_pUart->tx.Pop(&ch) == 0)
                USART_SendData(_pUart->uart, ch);
        }

    }
    /* ����bitλȫ��������ϵ��ж� */
    else if (USART_GetITStatus(_pUart->uart, USART_IT_TC) != RESET)
    {
        //if (_pUart->usTxRead == _pUart->usTxWrite)
        if (_pUart->tx.BufIsEmpty())
        {
            /* �������FIFO������ȫ��������ϣ���ֹ���ݷ�������ж� */
            USART_ITConfig(_pUart->uart, USART_IT_TC, DISABLE);
        }
        else
        {
            /* ��������£��������˷�֧ */
            uint8_t ch;
            if (_pUart->tx.Pop(&ch) == 0)
                USART_SendData(_pUart->uart, ch);
        }
    }
}

/*
 *********************************************************************************************************
 *	�� �� ��: USART1_IRQHandler  USART2_IRQHandler USART3_IRQHandler UART4_IRQHandler UART5_IRQHandler
 *	����˵��: USART�жϷ������
 *	��    ��: ��
 *	�� �� ֵ: ��
 *********************************************************************************************************
 */
void USART1_IRQHandlerCOM1(void)
{
    UartIRQ(&g_tUart1);
}

#ifdef __cplusplus
    extern "C"
    {
    #endif 
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
        #if 0	/* ����Ҫprintf���ַ�ͨ�������ж�FIFO���ͳ�ȥ��printf�������������� */
            comSendChar(COM1, ch);
            return ch;
        #else /* ����������ʽ����ÿ���ַ�,�ȴ����ݷ������ */
            /* дһ���ֽڵ�USART1 */
            USART_SendData(USART1, (uint8_t)ch);

            /* �ȴ����ͽ��� */
            while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}

            return ch;
        #endif 
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

        #if 1	/* �Ӵ��ڽ���FIFO��ȡ1������, ֻ��ȡ�����ݲŷ��� */
            uint8_t ucData;

            while (comGetChar(COM1, &ucData) == 0)
                ;

            return ucData;
        #else 
            /* �ȴ�����1�������� */
            while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
                ;

            return (int)USART_ReceiveData(USART1);
        #endif 
    }
    #ifdef __cplusplus
    }
#endif 

#ifdef DEBUG
    /* ���������������̷������� */
    #define EXAMPLE_NAME	"V4-003_���ں�PC��ͨ�ţ������жϡ�FIFO���ƣ�"
    #define EXAMPLE_DATE	"2015-08-30"
    #define DEMO_VER		"1.0"
    void PrintfLogo()
    {
        printf("*************************************************************\n\r");
        printf("* ��������   : %s\r\n", EXAMPLE_NAME); /* ��ӡ�������� */
        printf("* ���̰汾   : %s\r\n", DEMO_VER); /* ��ӡ���̰汾 */
        printf("* ��������   : %s\r\n", EXAMPLE_DATE); /* ��ӡ�������� */

        /* ��ӡST�̼���汾���궨���� stm32f4xx.h �ļ� */
        printf("* �̼���汾 : %d.%d.%d\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN, __STM32F10X_STDPERIPH_VERSION_SUB1, __STM32F10X_STDPERIPH_VERSION_SUB2);

        /* ��ӡ CMSIS �汾. �궨���� core_cm4.h �ļ� */
        printf("* CMSIS�汾  : %X.%02X\r\n", __CM3_CMSIS_VERSION_MAIN, __CM3_CMSIS_VERSION_SUB);

        printf("* \n\r"); /* ��ӡһ�пո� */
        printf("* QQ    : 1295744630 \r\n");
        printf("* Email : armfly@qq.com \r\n");
        printf("* Copyright www.armfly.com ����������\r\n");
        printf("*************************************************************\n\r");
    }
    void fiforoutin(void *param)
    {
        uint8_t read;
        if (comGetChar(COM1, &read))
        {
            switch (read)
            {
                case '1':
                    printf("���գ�1\n\r");
                    break;
                case '2':
                    printf("���գ�2\n\r");
                    break;
                case '3':
                    printf("���գ�3\n\r");
                    break;
                case '4':
                    printf("���գ�4\n\r");
                    break;
            }
        }
    }
    void FifoTest()
    {
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
        bsp_InitUart(); /* ��ʼ���������� */
        PrintfLogo();
        Sys.AddTask(fiforoutin, 0, 0, 200, "fiforoutin");
    }
#endif
