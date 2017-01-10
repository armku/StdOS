#include <stdio.h>
#include "SerialPortOld.h"
#include "stm32f10x.h"


SerialPortOld::SerialPortOld(COM_Def index, int baudRate, byte parity, byte dataBits, byte stopBits)
{
    this->_index = index;
    this->_baudRate = baudRate;
    this->_parity = parity;
    this->_dataBits = dataBits;
    this->_stopBits = stopBits;

    this->Name[0] = 'C';
    this->Name[1] = 'O';
    this->Name[2] = 'M';
    this->Name[3] = '1' + index;
    this->Name[4] = 0;

    USART_InitTypeDef usart;
    NVIC_InitTypeDef nvic;

    usart.USART_BaudRate = this->_baudRate;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart.USART_Parity = this->_parity;
    usart.USART_StopBits = this->_stopBits;
    usart.USART_WordLength = this->_dataBits;

    nvic.NVIC_IRQChannelCmd = ENABLE;
    rx.Floating = true;
    //��ʼ���˿�����
    switch (this->_index)
    {
        case COM1:            
                tx.Set(PA9);
            rx.Set(PA10);
            break;
        case COM2:            
                tx.Set(PA2);
           rx.Set(PA3);
            break;
        case COM3:            
                tx.Set(PB10);
            rx.Set(PB11);
            break;
        case COM4:           
                tx.Set(PC10);            
            rx.Set(PC11);
            break;
        case COM5:           
                tx.Set(PC12);
            rx.Set(PD3);
            break;
        default:
            break;
    }

    switch (this->_index)
    {
        case COM1:
            //����1��ʼ��
            //��ʼ��ʱ���ź�
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
            //��ʼ��USART
            USART_Init(USART1, &usart);
            USART_Cmd(USART1, ENABLE);
            USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
            //��ʼ��NVIC
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = USART1_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 0;
            nvic.NVIC_IRQChannelSubPriority = 0;
            NVIC_Init(&nvic);
            break;
        case COM2:
            //����2��ʼ��
            //��ʼ��ʱ���ź�
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
            //��ʼ��USART            
            USART_Init(USART2, &usart);
            USART_Cmd(USART2, ENABLE);
            USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
            //��ʼ��NVIC
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = USART2_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 0;
            nvic.NVIC_IRQChannelSubPriority = 1;
            NVIC_Init(&nvic);
            break;
        case COM3:
            //����3��ʼ��
            //��ʼ��ʱ���ź�   
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
            //��ʼ��USART            
            USART_Init(USART3, &usart);
            USART_Cmd(USART3, ENABLE);
            USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
            //��ʼ��NVIC
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
            nvic.NVIC_IRQChannel = USART3_IRQn;
            nvic.NVIC_IRQChannelPreemptionPriority = 0;
            nvic.NVIC_IRQChannelSubPriority = 2;
            NVIC_Init(&nvic);
            break;
            #if 0
            case COM4:
                //����4��ʼ��
                //��ʼ��ʱ���ź�   
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
                //��ʼ��USART            
                USART_Init(UART4, &usart);
                USART_Cmd(UART4, ENABLE);
                USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
                //��ʼ��NVIC
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannel = UART4_IRQn;
                nvic.NVIC_IRQChannelPreemptionPriority = 0;
                nvic.NVIC_IRQChannelSubPriority = 3;
                NVIC_Init(&nvic);
                break;
            case COM5:
                //����5��ʼ��
                //��ʼ��ʱ���ź�   
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
                //��ʼ��USART            
                USART_Init(UART5, &usart);
                USART_Cmd(UART5, ENABLE);
                USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
                //��ʼ��NVIC
                NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
                nvic.NVIC_IRQChannel = UART5_IRQn;
                nvic.NVIC_IRQChannelPreemptionPriority = 0;
                nvic.NVIC_IRQChannelSubPriority = 2;
                NVIC_Init(&nvic);
                break;
            #endif 
        default:
            break;
    }

}

void SerialPortOld::Open(){

}

CFIFORing com1buf; //����1���ջ�����
uint com1timeidle; //����1����ʱ��

CFIFORing com2buf; //����2���ջ�����
uint com2timeidle; //����2����ʱ��

CFIFORing com3buf; //����3���ջ�����
uint com3timeidle; //����3����ʱ��
#ifdef __cplusplus
    extern "C"
    {
    #endif 

    //����1�жϷ������
    //ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
    /*
     */
    void USART1_IRQHandler(void) //����1�жϷ������
    {
        if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
        //���յ�һ�ֽ�
        {
            USART_ClearITPendingBit(USART1, USART_IT_RXNE);
            byte inch = USART1->DR; //��ȡ���յ�������

            com1buf.Push(inch);
            com1timeidle = 0; //���м�ʱ������
        }
    }

    void USART2_IRQHandler(void) //����1�жϷ������
    {
        if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
        {
            USART_ClearITPendingBit(USART2, USART_IT_RXNE);
            byte inch = USART_ReceiveData(USART2); //��ȡ���յ�������

            com2buf.Push(inch);
            com2timeidle = 0; //���м�ʱ������
        }
    }
    void USART3_IRQHandler(void) //����1�жϷ������
    {
        if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
        {
            USART_ClearITPendingBit(USART3, USART_IT_RXNE);
            byte inch = USART_ReceiveData(USART3); //��ȡ���յ�������

            com3buf.Push(inch);
            com3timeidle = 0; //���м�ʱ������
        }
    }
    void USART4_IRQHandler(void) //����1�жϷ������
    {
        if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
        {
            USART_ClearITPendingBit(UART4, USART_IT_RXNE);
            byte inch = USART_ReceiveData(UART4); //��ȡ���յ�������
        }
    }
    void USART5_IRQHandler(void) //����1�жϷ������
    {
        if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
        //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
        {
            USART_ClearITPendingBit(UART5, USART_IT_RXNE);
            byte inch = USART_ReceiveData(UART5); //��ȡ���յ�������
        }
    }


    //�򴮿�1��������
    void sendToUsart1(byte ch)
    {
        #if 0
            while ((USART1->SR &0X40) == 0){}
            //ѭ������,ֱ���������
            USART1->DR = (byte)ch;
        #else 
            USART_SendData(USART1, ch);

            /* �ȴ�������� */
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
        #endif 
    }

    //�򴮿�2��������
    void sendToUsart2(byte ch)
    {
        USART_SendData(USART2, ch);

        /* �ȴ�������� */
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET){}
    }

    //�򴮿�3��������
    void sendToUsart3(byte ch)
    {
        USART_SendData(USART3, ch);

        /* �ȴ�������� */
        while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){}
    }

    //�򴮿�4��������
    void sendToUsart4(byte ch)
    {
        USART_SendData(UART4, ch);

        /* �ȴ�������� */
        while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET){}
    }

    //�򴮿�5��������
    void sendToUsart5(byte ch)
    {
        USART_SendData(UART5, ch);

        /* �ȴ�������� */
        while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET){}
    }
    void sendch(int ch)
    {
        switch (Sys.MessagePort)
        {
            case COM1:
                sendToUsart1(ch);
                break;
            case COM2:
                sendToUsart2(ch);
                break;
            case COM3:
                sendToUsart3(ch);
                break;
            case COM4:
                sendToUsart4(ch);
                break;
            case COM5:
                sendToUsart5(ch);
                break;
            default:
                break;
        }
    }

    //�ض���fputc����
    int fputc(int ch, FILE *f)
    {
        sendch(ch);
        return ch;
    }
    #ifdef __cplusplus
    }
#endif 
void SerialPortOld::SendData(byte data)
{
    switch (this->_index)
    {
        case COM1:
            sendToUsart1(data);
            break;
        case COM2:
            sendToUsart2(data);
            break;
        case COM3:
            sendToUsart3(data);
            break;
        case COM4:
            sendToUsart4(data);
            break;
        case COM5:
            sendToUsart5(data);
            break;
        default:
            break;
    }
}

//��������
void SerialPortOld::SendBuffer(byte *buff, int length)
{
    if (length ==  - 1)
    {
        while (*buff)
        {
            this->SendData(*buff);
            buff++;
        }
        return ;
    }
    if (length <= 0)
    {
        return ;
    }
    for (int i = 0; i < length; i++)
    {
        this->SendData(buff[i]);
    }
}

//��������
void SerialPortOld::SendBuffer(char *buff, int length)
{
    this->SendBuffer((byte*)buff, length);
}

// ע�����ݵ����¼�
void SerialPortOld::Register(IOnUsartRead handler, SerialPortOld *sp)
{
    this->OnRcv = handler;
}

byte buftmp[40];
//�Ӵ����յ�����
void SerialPortOld::OnUsartReceive(byte *buf, ushort length)
{
    Buffer bs(buftmp, length);
    for (int i = 0; i < length; i++)
    {
        buftmp[i] = buf[i];
    }
    if (this->OnRcv)
    {
        this->OnRcv(this, bs, this);
    }
}
